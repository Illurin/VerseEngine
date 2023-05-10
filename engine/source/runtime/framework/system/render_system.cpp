#include "render_system.hpp"

#include "runtime/resource/resource_manager.hpp"
#include "runtime/render/geometry.hpp"

#include "runtime/framework/component/transform_component.hpp"
#include "runtime/framework/component/material_component.hpp"
#include "runtime/framework/component/renderer_component.hpp"
#include "runtime/framework/component/scene_component.hpp"

namespace engine {

    RenderSystem::RenderSystem() {
        auto commandPoolInfo = rhi::CommandPoolCreateInfo()
            .SetQueue(RenderDevice::GetQueue());
        commandPool = RenderDevice::GetDevice()->CreateCommandPool(commandPoolInfo);
        
        commandBuffers = commandPool->AllocateCommandBuffers(static_cast<uint32_t>(RenderDevice::GetSwapchain()->GetImages().size()));

        fence = RenderDevice::GetDevice()->CreateFence(rhi::FenceCreateInfo());

        pScenePass = std::make_unique<ScenePass>();
        pSkyboxPass = std::make_unique<SkyboxPass>(pScenePass->GetDescriptorSetLayouts()[2], pScenePass->GetRenderPass());
        pPostProcessingPass = std::make_unique<PostProcessingPass>(pScenePass->GetRenderTarget());
        pScreenQuadPass = std::make_unique<ScreenQuadPass>(pPostProcessingPass->GetRenderTarget());
    }

    RenderSystem::~RenderSystem() {
        ResourceManager::Clear();
        commandPool->Destroy();
        fence->Destroy();
        if (vertexBuffer) vertexBuffer->Destroy();
        if (indexBuffer) indexBuffer->Destroy();
        if (passUniformBuffer) passUniformBuffer->Destroy();
        if (descriptorPool) descriptorPool->Destroy();
        for (auto buffer : transformUniformBuffers) buffer->Destroy();
    }

    void RenderSystem::Start() {
        BuildMeshes();
        ResourceManager::LoadResources();
        BuildDescriptors();
        PrepareRendering();
    }

    void RenderSystem::Update() {
        UpdateFrameResource();
        UpdateCommandBuffers();
        UpdateRendering();
    }

    void RenderSystem::BuildMeshes() {
        std::vector<ObjectVertexData> vertices;
        std::vector<uint32_t> indices;

        for (auto [entity, meshRenderer] : GetEntities<MeshRendererComponent>().each()) {
            meshRenderer.vertexOffset = static_cast<uint32_t>(vertices.size());
            meshRenderer.firstIndex = static_cast<uint32_t>(indices.size());
            meshRenderer.indexCount = static_cast<uint32_t>(meshRenderer.indices.size());
            vertices.insert(vertices.end(), meshRenderer.vertices.begin(), meshRenderer.vertices.end());
            indices.insert(indices.end(), meshRenderer.indices.begin(), meshRenderer.indices.end());
            decltype(meshRenderer.vertices) temp0; temp0.swap(meshRenderer.vertices);
            decltype(meshRenderer.indices) temp1; temp1.swap(meshRenderer.indices);
        }

        // Skybox meshes

        auto& skyboxRenderer = Entity::AddSingletonComponent<SkyboxRendererComponent>();
        skyboxRenderer.vertexOffset = static_cast<uint32_t>(vertices.size());
        skyboxRenderer.firstIndex = static_cast<uint32_t>(indices.size());
        auto skyboxGeosphere = Geometry::CreateGeosphere(1.0f, 0);
        vertices.insert(vertices.end(), skyboxGeosphere.vertices.begin(), skyboxGeosphere.vertices.end());
        indices.insert(indices.end(), skyboxGeosphere.indices.begin(), skyboxGeosphere.indices.end());
        skyboxRenderer.indexCount = static_cast<uint32_t>(skyboxGeosphere.indices.size());

        auto vertexBufferInfo = rhi::BufferCreateInfo()
            .SetSize(sizeof(ObjectVertexData) * vertices.size())
            .SetUsage(rhi::BufferUsage::VertexBuffer);
        vertexBuffer = RenderDevice::GetDevice()->CreateBuffer(vertexBufferInfo);

        auto indexBufferInfo = rhi::BufferCreateInfo()
            .SetSize(sizeof(uint32_t) * indices.size())
            .SetUsage(rhi::BufferUsage::IndexBuffer);
        indexBuffer = RenderDevice::GetDevice()->CreateBuffer(indexBufferInfo);

        auto pVertexBuffer = reinterpret_cast<ObjectVertexData*>(vertexBuffer->Map());
        std::copy(vertices.begin(), vertices.end(), pVertexBuffer);
        vertexBuffer->Unmap();

        auto pIndexBuffer = reinterpret_cast<uint32_t*>(indexBuffer->Map());
        std::copy(indices.begin(), indices.end(), pIndexBuffer);
        indexBuffer->Unmap();
    }

    void RenderSystem::BuildDescriptors() {
        auto transformEntities = GetEntities<TransformComponent>();
        auto materialEntities = GetEntities<MaterialComponent>();
        auto cameraEntities = GetEntities<CameraComponent>();
        
        std::vector<rhi::DescriptorPoolSize> descriptorPoolSizes = {
            { rhi::DescriptorType::UniformBuffer,
              static_cast<uint32_t>(transformEntities.size()) + static_cast<uint32_t>(materialEntities.size()) + 1 },
            { rhi::DescriptorType::SampledImage, 2 * static_cast<uint32_t>(materialEntities.size()) + 1 },
            { rhi::DescriptorType::Sampler, 2 * static_cast<uint32_t>(materialEntities.size()) + 1 },
        };

        auto maxSetCount = static_cast<uint32_t>(transformEntities.size()) + static_cast<uint32_t>(materialEntities.size()) + 1;

        auto descriptorPoolInfo = rhi::DescriptorPoolCreateInfo()
            .SetPoolSizes(descriptorPoolSizes)
            .SetMaxSetCount(maxSetCount);
        descriptorPool = RenderDevice::GetDevice()->CreateDescriptorPool(descriptorPoolInfo);

        for (auto [entity, transform] : transformEntities.each()) {

            // Object constants

            transform.id = static_cast<uint32_t>(transformUniformBuffers.size());

            auto uniformBufferInfo = rhi::BufferCreateInfo()
                .SetSize(sizeof(ObjectConstants))
                .SetUsage(rhi::BufferUsage::UniformBuffer);
            auto uniformBuffer = RenderDevice::GetDevice()->CreateBuffer(uniformBufferInfo);

            std::vector allocateLayouts = { pScenePass->GetDescriptorSetLayouts()[0] };

            auto allocateInfo = rhi::DescriptorSetAllocateInfo()
                .SetDescriptorSetLayouts(allocateLayouts);
            auto desriptorSets = descriptorPool->AllocateDescriptorSets(allocateInfo);

            desriptorSets[0]->Write(0, rhi::DescriptorType::UniformBuffer, uniformBuffer);
            
            transformUniformBuffers.emplace_back(uniformBuffer);
            transformDescriptorSets.emplace_back(desriptorSets[0]);
        }

        for (auto [entity, material] : materialEntities.each()) {
            
            auto& materialResource = ResourceManager::GetMaterial(material.resource);

            // Material constants

            std::vector allocateLayouts = { pScenePass->GetDescriptorSetLayouts()[1] };

            auto allocateInfo = rhi::DescriptorSetAllocateInfo()
                .SetDescriptorSetLayouts(allocateLayouts);
            auto desriptorSets = descriptorPool->AllocateDescriptorSets(allocateInfo);

            desriptorSets[0]->Write(0, rhi::DescriptorType::UniformBuffer, materialResource.uniformBuffer);

            // Material texture and sampler

            auto diffuseMapResource = ResourceManager::GetTexture(materialResource.diffuseMap);
            auto normalMapResource = ResourceManager::GetTexture(materialResource.normalMap);

            auto imageViewInfo = rhi::ImageViewInfo()
                .SetImage(diffuseMapResource.image)
                .SetFormat(rhi::Format::R8G8B8A8Unorm)
                .SetViewType(rhi::ImageViewType::Image2D)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(1).SetMipLevelCount(1));

            desriptorSets[0]->Write(1, rhi::DescriptorType::SampledImage, rhi::ImageLayout::ShaderReadOnly, imageViewInfo);
            desriptorSets[0]->Write(2, rhi::DescriptorType::Sampler, diffuseMapResource.sampler);

            imageViewInfo.SetImage(normalMapResource.image);

            desriptorSets[0]->Write(3, rhi::DescriptorType::SampledImage, rhi::ImageLayout::ShaderReadOnly, imageViewInfo);
            desriptorSets[0]->Write(4, rhi::DescriptorType::Sampler, normalMapResource.sampler);

            materialResource.descriptorSet = desriptorSets[0];
        }

        {
            // Pass constants

            auto uniformBufferInfo = rhi::BufferCreateInfo()
                .SetSize(sizeof(PassConstants))
                .SetUsage(rhi::BufferUsage::UniformBuffer);
            passUniformBuffer = RenderDevice::GetDevice()->CreateBuffer(uniformBufferInfo);

            std::vector allocateLayouts = { pScenePass->GetDescriptorSetLayouts()[2] };

            auto allocateInfo = rhi::DescriptorSetAllocateInfo()
                .SetDescriptorSetLayouts(allocateLayouts);
            auto desriptorSets = descriptorPool->AllocateDescriptorSets(allocateInfo);

            desriptorSets[0]->Write(0, rhi::DescriptorType::UniformBuffer, passUniformBuffer);

            // Skybox texture and sampler

            auto& environment = Entity::GetSingletonComponents<EnvironmentComponent>();
            auto skyboxTextureResource = ResourceManager::GetTexture(environment.skyboxTextureResource);

            auto imageViewInfo = rhi::ImageViewInfo()
                .SetImage(skyboxTextureResource.image)
                .SetFormat(rhi::Format::R8G8B8A8Unorm)
                .SetViewType(rhi::ImageViewType::ImageCube)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(6).SetMipLevelCount(1));

            desriptorSets[0]->Write(1, rhi::DescriptorType::SampledImage, rhi::ImageLayout::ShaderReadOnly, imageViewInfo);
            desriptorSets[0]->Write(2, rhi::DescriptorType::Sampler, skyboxTextureResource.sampler);

            passDescriptorSet = desriptorSets[0];

            // Post processing constants

            
        }
    }

    void RenderSystem::UpdateFrameResource() {
        for (auto [entity, transform] : GetEntities<TransformComponent>().each()) {
            if (!transform.dirty) continue;

            auto s = math::AlignedScaling3f(transform.scale);
            auto lr = math::AngleAxisf(transform.localEulerAngle.z() / 180.0f * math::Pi<float>(), math::Vector3f::UnitZ())
                    * math::AngleAxisf(transform.localEulerAngle.y() / 180.0f * math::Pi<float>(), math::Vector3f::UnitY())
                    * math::AngleAxisf(transform.localEulerAngle.x() / 180.0f * math::Pi<float>(), math::Vector3f::UnitX());
            auto t = math::Translation3f(transform.position);
            auto gr = math::AngleAxisf(transform.globalEulerAngle.z() / 180.0f * math::Pi<float>(), math::Vector3f::UnitZ())
                    * math::AngleAxisf(transform.globalEulerAngle.y() / 180.0f * math::Pi<float>(), math::Vector3f::UnitY())
                    * math::AngleAxisf(transform.globalEulerAngle.x() / 180.0f * math::Pi<float>(), math::Vector3f::UnitX());
            math::Affine3f affine(gr * t * lr * s);
            
            auto uniformBuffer = transformUniformBuffers[transform.id];
            auto p = reinterpret_cast<ObjectConstants*>(uniformBuffer->Map());
            p->worldMatrix = affine.matrix();
            uniformBuffer->Unmap();

            transform.dirty = false;
        }

        auto p = reinterpret_cast<PassConstants*>(passUniformBuffer->Map());

        auto& environment = Entity::GetSingletonComponents<EnvironmentComponent>();
        if (environment.dirty) {
            p->ambientLight << environment.ambientLight.x(),
                               environment.ambientLight.y(),
                               environment.ambientLight.z(),
                               0.0f;
        }

        for (auto [entity, camera] : GetEntities<CameraComponent>().each()) {
            if (!camera.active) continue;
            if (!camera.dirty) continue;

            camera.camera.UpdateViewMatrix();
            p->viewMatrix = camera.camera.GetViewMatrix();
            p->projMatrix = camera.camera.GetProjMatrix();
            p->eyePos << camera.camera.GetPosition(), 1.0f;
            camera.dirty = false;
        }

        for (auto [entity, transform, light] : GetEntities<TransformComponent, LightComponent>().each()) {
            if (!light.dirty) continue;
            
            if (light.id == ~0u) {
                uint32_t type = static_cast<uint32_t>(light.type);
                auto& index = lightCounts[type];
                if (index >= typedLightCounts[type])
                    continue;
                light.id = type > 0 ? index + typedLightCounts[type - 1] : index;
                index++;
            }

            auto& lightConstant       = p->lights[light.id];
            lightConstant.color       = light.color;
            lightConstant.direction   = light.direction;
            lightConstant.position    = transform.position;
            lightConstant.intensity   = light.intensity;
            lightConstant.attenuation = light.attenuation;
            lightConstant.spotPower   = light.spotPower;

            light.dirty = false;
        }

        passUniformBuffer->Unmap();
    }

    void RenderSystem::UpdateCommandBuffers() {
        if (prepared) return;
        prepared = true;

        for (uint32_t i = 0; i < RenderDevice::GetSwapchain()->GetImages().size(); i++) {
            auto& commandBuffer = commandBuffers[i];

            commandBuffer->Reset();
            commandBuffer->Begin(rhi::CommandBufferBeginInfo());

            // Pass 0

            pScenePass->Begin(commandBuffer);

            commandBuffer->BindVertexBuffer(0, 1, &vertexBuffer);
            commandBuffer->BindIndexBuffer(indexBuffer, 0, rhi::IndexType::Uint32);

            commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pScenePass->GetPipelineLayout(), 2, 1, &passDescriptorSet);

            for (auto [entity, transform, meshRenderer, material] : GetEntities<const TransformComponent, const MeshRendererComponent, const MaterialComponent>().each()) {
                commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pScenePass->GetPipelineLayout(), 0, 1, &transformDescriptorSets[transform.id]);
                commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pScenePass->GetPipelineLayout(), 1, 1, &ResourceManager::GetMaterial(material.resource).descriptorSet);

                commandBuffer->DrawIndexed(meshRenderer.indexCount, 1, meshRenderer.firstIndex, meshRenderer.vertexOffset, 0);
            }

            commandBuffer->BindPipeline(pSkyboxPass->GetPipeline());
            commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pSkyboxPass->GetPipelineLayout(), 0, 1, &passDescriptorSet);

            auto& skyboxRenderer = Entity::GetSingletonComponents<SkyboxRendererComponent>();
            commandBuffer->DrawIndexed(skyboxRenderer.indexCount, 1, skyboxRenderer.firstIndex, skyboxRenderer.vertexOffset, 0);

            commandBuffer->EndRenderPass();

            // Pass 1

            pPostProcessingPass->Execute(commandBuffer);

            // Pass 2

            pScreenQuadPass->Execute(i, commandBuffer);

            commandBuffer->End();
        }
    }

    void RenderSystem::PrepareRendering() {
        auto& commandBuffer = commandBuffers[0];

        commandBuffer->Reset();
        commandBuffer->Begin(rhi::CommandBufferBeginInfo());
        pScenePass->Prepare(commandBuffer);
        pPostProcessingPass->Prepare(commandBuffer);
        commandBuffer->End();

        fence->Reset();
        RenderDevice::GetQueue()->Submit(1, &commandBuffer, fence);
        fence->Wait();
    }

    void RenderSystem::UpdateRendering() {
        uint32_t swapchainImageIndex = RenderDevice::GetQueue()->AcquireNextImage(RenderDevice::GetSwapchain());

        fence->Reset();
        RenderDevice::GetQueue()->Submit(1, &commandBuffers[swapchainImageIndex], fence);
        fence->Wait();
        RenderDevice::GetQueue()->Present(RenderDevice::GetSwapchain(), swapchainImageIndex);
    }

}
