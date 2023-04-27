#include "render_system.hpp"

#include "runtime/render/geometry.hpp"

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
        commandPool->Destroy();
        fence->Destroy();
        if (vertexBuffer) vertexBuffer->Destroy();
        if (indexBuffer) indexBuffer->Destroy();
        if (passUniformBuffer) passUniformBuffer->Destroy();
        if (descriptorPool) descriptorPool->Destroy();
    }

    void RenderSystem::Start(ecs::registry& registry) {
        BuildMeshes(registry);
        BuildDescriptors(registry);
        PrepareRendering();
    }

    void RenderSystem::Update(ecs::registry& registry) {
        UpdateFrameResource(registry);
        UpdateCommandBuffers(registry);
        UpdateRendering();
    }

    void RenderSystem::BuildMeshes(ecs::registry& registry) {
        auto view = registry.view<MeshRendererComponent>();

        std::vector<ObjectVertexData> vertices;
        std::vector<uint32_t> indices;

        view.each([&](MeshRendererComponent& meshRenderer) {
            meshRenderer.vertexOffset = static_cast<uint32_t>(vertices.size());
            meshRenderer.firstIndex = static_cast<uint32_t>(indices.size());
            vertices.insert(vertices.end(), meshRenderer.vertices.begin(), meshRenderer.vertices.end());
            indices.insert(indices.end(), meshRenderer.indices.begin(), meshRenderer.indices.end());
        });

        // Skybox meshes

        auto& skyboxRenderer = registry.ctx().emplace<SkyboxRendererComponent>();
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

    void RenderSystem::BuildDescriptors(ecs::registry& registry) {
        auto transformView = registry.view<TransformComponent>();
        auto materialView = registry.view<MaterialComponent>();
        auto cameraView = registry.view<CameraComponent>();
        
        std::vector<rhi::DescriptorPoolSize> descriptorPoolSizes = {
            { rhi::DescriptorType::UniformBuffer,
              static_cast<uint32_t>(transformView.size()) + static_cast<uint32_t>(materialView.size()) + 1 },
            { rhi::DescriptorType::SampledImage, static_cast<uint32_t>(materialView.size()) + 1 },
            { rhi::DescriptorType::Sampler, static_cast<uint32_t>(materialView.size()) + 1 },
        };

        auto maxSetCount = static_cast<uint32_t>(transformView.size()) + static_cast<uint32_t>(materialView.size()) + 1;

        auto descriptorPoolInfo = rhi::DescriptorPoolCreateInfo()
            .SetPoolSizes(descriptorPoolSizes)
            .SetMaxSetCount(maxSetCount);
        descriptorPool = RenderDevice::GetDevice()->CreateDescriptorPool(descriptorPoolInfo);

        transformView.each([&](TransformComponent& transform) {
            
            // Object constants

            auto uniformBufferInfo = rhi::BufferCreateInfo()
                .SetSize(sizeof(ObjectConstants))
                .SetUsage(rhi::BufferUsage::UniformBuffer);
            transform.uniformBuffer = RenderDevice::GetDevice()->CreateBuffer(uniformBufferInfo);

            std::vector allocateLayouts = { pScenePass->GetDescriptorSetLayouts()[0] };

            auto allocateInfo = rhi::DescriptorSetAllocateInfo()
                .SetDescriptorSetLayouts(allocateLayouts);
            auto desriptorSets = descriptorPool->AllocateDescriptorSets(allocateInfo);

            desriptorSets[0]->Write(0, rhi::DescriptorType::UniformBuffer, transform.uniformBuffer);
            
            transform.descriptorSet = desriptorSets[0];
        });

        materialView.each([&](MaterialComponent& material) {

            // Material constants

            auto uniformBufferInfo = rhi::BufferCreateInfo()
                .SetSize(sizeof(MaterialConstants))
                .SetUsage(rhi::BufferUsage::UniformBuffer);
            material.uniformBuffer = RenderDevice::GetDevice()->CreateBuffer(uniformBufferInfo);

            auto p = reinterpret_cast<MaterialConstants*>(material.uniformBuffer->Map());
            *p = material.constants;
            material.uniformBuffer->Unmap();

            std::vector allocateLayouts = { pScenePass->GetDescriptorSetLayouts()[1] };

            auto allocateInfo = rhi::DescriptorSetAllocateInfo()
                .SetDescriptorSetLayouts(allocateLayouts);
            auto desriptorSets = descriptorPool->AllocateDescriptorSets(allocateInfo);

            desriptorSets[0]->Write(0, rhi::DescriptorType::UniformBuffer, material.uniformBuffer);

            // Material texture and sampler

            auto imageViewInfo = rhi::ImageViewInfo()
                .SetImage(material.texture)
                .SetFormat(rhi::Format::R8G8B8A8Unorm)
                .SetViewType(rhi::ImageViewType::Image2D)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(1).SetMipLevelCount(1));

            desriptorSets[0]->Write(1, rhi::DescriptorType::SampledImage, rhi::ImageLayout::ShaderReadOnly, imageViewInfo);
            desriptorSets[0]->Write(2, rhi::DescriptorType::Sampler, material.sampler);

            material.descriptorSet = desriptorSets[0];
        });

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

            auto environment = registry.ctx().get<EnvironmentComponent>();

            auto imageViewInfo = rhi::ImageViewInfo()
                .SetImage(environment.skyboxTexture)
                .SetFormat(rhi::Format::R8G8B8A8Unorm)
                .SetViewType(rhi::ImageViewType::ImageCube)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(6).SetMipLevelCount(1));

            desriptorSets[0]->Write(1, rhi::DescriptorType::SampledImage, rhi::ImageLayout::ShaderReadOnly, imageViewInfo);
            desriptorSets[0]->Write(2, rhi::DescriptorType::Sampler, environment.skyboxSampler);

            passDescriptorSet = desriptorSets[0];

            // Post processing constants

            
        }
    }

    void RenderSystem::UpdateFrameResource(ecs::registry& registry) {
        auto transformUpdateView = registry.view<TransformComponent>();

        transformUpdateView.each([&](TransformComponent& transform) {
            if (!transform.dirty) return;

            auto s = math::AlignedScaling3f(transform.scale);
            auto lr = math::AngleAxisf(transform.localEulerAngle.z() / 180.0f * math::Pi<float>(), math::Vector3f::UnitZ())
                    * math::AngleAxisf(transform.localEulerAngle.y() / 180.0f * math::Pi<float>(), math::Vector3f::UnitY())
                    * math::AngleAxisf(transform.localEulerAngle.x() / 180.0f * math::Pi<float>(), math::Vector3f::UnitX());
            auto t = math::Translation3f(transform.position);
            auto gr = math::AngleAxisf(transform.globalEulerAngle.z() / 180.0f * math::Pi<float>(), math::Vector3f::UnitZ())
                    * math::AngleAxisf(transform.globalEulerAngle.y() / 180.0f * math::Pi<float>(), math::Vector3f::UnitY())
                    * math::AngleAxisf(transform.globalEulerAngle.x() / 180.0f * math::Pi<float>(), math::Vector3f::UnitX());
            math::Affine3f affine(gr * t * lr * s);
            
            auto p = reinterpret_cast<ObjectConstants*>(transform.uniformBuffer->Map());
            p->worldMatrix = affine.matrix();
            transform.uniformBuffer->Unmap();

            transform.dirty = false;
        });

        auto cameraUpdateView = registry.view<CameraComponent>();

        auto p = reinterpret_cast<PassConstants*>(passUniformBuffer->Map());

        auto& environment = registry.ctx().get<EnvironmentComponent>();
        if (environment.dirty) {
            p->ambientLight = environment.ambientLight;
        }

        cameraUpdateView.each([&](CameraComponent& camera) {
            if (!camera.active) return;
            if (!camera.dirty) return;

            camera.camera.UpdataViewMatrix();
            p->viewMatrix = camera.camera.GetViewMatrix();
            p->projMatrix = camera.camera.GetProjMatrix();
            p->eyePos << camera.camera.GetPosition(), 1.0f;
            camera.dirty = false;
        });

        passUniformBuffer->Unmap();
    }

    void RenderSystem::UpdateCommandBuffers(ecs::registry& registry) {
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

            auto renderView = registry.view<const TransformComponent, const MeshRendererComponent, const MaterialComponent>();

            renderView.each([&](const TransformComponent& transform, const MeshRendererComponent& meshRenderer, const MaterialComponent& material) {
                commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pScenePass->GetPipelineLayout(), 0, 1, &transform.descriptorSet);
                commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pScenePass->GetPipelineLayout(), 1, 1, &material.descriptorSet);

                commandBuffer->DrawIndexed(static_cast<uint32_t>(meshRenderer.indices.size()), 1, meshRenderer.firstIndex, meshRenderer.vertexOffset, 0);
            });

            commandBuffer->BindPipeline(pSkyboxPass->GetPipeline());
            commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pSkyboxPass->GetPipelineLayout(), 0, 1, &passDescriptorSet);

            auto& skyboxRenderer = registry.ctx().get<SkyboxRendererComponent>();
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
