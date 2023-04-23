#include "resource_system.hpp"

#include "runtime/resource/asset_loader/image/image_loader.hpp"
#include "runtime/render/render_device.hpp"

namespace engine {

    ResourceSystem::~ResourceSystem() {
        for (auto& [key, image] : images) {
            image->Destroy();
        }
        for (auto& [key, cubemap] : cubemaps) {
            cubemap->Destroy();
        }
        for (auto sampler : samplers) {
            sampler->Destroy();
        }
    }

    void ResourceSystem::Start(ecs::registry& registry) {
        BuildMaterials(registry);
    }

    void ResourceSystem::Update(ecs::registry& registry) {
        
    }

    void ResourceSystem::BuildMaterials(ecs::registry& registry) {
        std::unordered_map<std::string, rhi::Buffer> uploaderBuffers;

        // Prepare object textures

        auto materialView = registry.view<MaterialComponent>();

        materialView.each([&](MaterialComponent& material) {

            // Load image

            auto key = material.texturePath.string();
            if (images.find(key) == images.end()) {
                auto rawImage = asset::ImageLoader::Load(material.texturePath, 32);

                auto imageInfo = rhi::ImageCreateInfo()
                    .SetFormat(rhi::Format::R8G8B8A8Unorm)
                    .SetImageType(rhi::ImageType::Image2D)
                    .SetExtent(rhi::Extent3D().SetWidth(rawImage.width).SetHeight(rawImage.height).SetDepth(1))
                    .SetArrayLayers(1)
                    .SetMipLevels(1)
                    .SetUsage(rhi::ImageUsage::SampledAndTransferDst);
                auto image = RenderDevice::GetDevice()->CreateImage(imageInfo);

                auto footprint = image->GetCopyableFootprint();

                auto bufferInfo = rhi::BufferCreateInfo()
                    .SetSize(footprint.size)
                    .SetUsage(rhi::BufferUsage::TransferSrc);
                auto uploaderBuffer = RenderDevice::GetDevice()->CreateBuffer(bufferInfo);

                auto pUploaderBuffer = reinterpret_cast<uint8_t*>(uploaderBuffer->Map());
                if (footprint.subresources.empty()) {
                    std::copy(rawImage.source.begin(), rawImage.source.end(), pUploaderBuffer);
                }
                else {
                    for (uint32_t i = 0; i < rawImage.height; i++) {
                        auto rowBegin = rawImage.source.begin() + i * rawImage.rowPitch;
                        std::copy(rowBegin, rowBegin + rawImage.rowPitch, pUploaderBuffer + i * footprint.subresources[0].rowPitch);
                    }
                }
                uploaderBuffer->Unmap();

                images[key] = image;
                uploaderBuffers[key] = uploaderBuffer;
            }
            material.texture = images.at(key);

            // Build sampler

            material.sampler = RenderDevice::GetDevice()->CreateSampler(material.samplerInfo);
            samplers.emplace_back(material.sampler);
        });

        // Prepare cubemaps

        auto& environment = registry.ctx().get<EnvironmentComponent>();

        {
            // Load image

            auto key = environment.skyboxTexturePath.string();
            if (cubemaps.find(key) == cubemaps.end()) {
                auto rawCubemap = asset::ImageLoader::LoadCubemap(environment.skyboxTexturePath, 32);

                auto imageInfo = rhi::ImageCreateInfo()
                    .SetFormat(rhi::Format::R8G8B8A8Unorm)
                    .SetImageType(rhi::ImageType::Image2D)
                    .SetExtent(rhi::Extent3D().SetWidth(rawCubemap.faceWidth).SetHeight(rawCubemap.faceHeight).SetDepth(1))
                    .SetArrayLayers(6)
                    .SetMipLevels(1)
                    .SetUsage(rhi::ImageUsage::SampledAndTransferDst);
                auto cubemap = RenderDevice::GetDevice()->CreateImage(imageInfo);

                auto footprint = cubemap->GetCopyableFootprint();

                auto bufferInfo = rhi::BufferCreateInfo()
                    .SetSize(footprint.size)
                    .SetUsage(rhi::BufferUsage::TransferSrc);
                auto uploaderBuffer = RenderDevice::GetDevice()->CreateBuffer(bufferInfo);

                auto pUploaderBuffer = reinterpret_cast<uint8_t*>(uploaderBuffer->Map());
                if (footprint.subresources.empty()) {
                    std::copy(rawCubemap.right.begin(),  rawCubemap.right.end(),  pUploaderBuffer);
                    std::copy(rawCubemap.left.begin(),   rawCubemap.left.end(),   pUploaderBuffer +     rawCubemap.faceSize);
                    std::copy(rawCubemap.top.begin(),    rawCubemap.top.end(),    pUploaderBuffer + 2 * rawCubemap.faceSize);
                    std::copy(rawCubemap.bottom.begin(), rawCubemap.bottom.end(), pUploaderBuffer + 3 * rawCubemap.faceSize);
                    std::copy(rawCubemap.front.begin(),  rawCubemap.front.end(),  pUploaderBuffer + 4 * rawCubemap.faceSize);
                    std::copy(rawCubemap.back.begin(),   rawCubemap.back.end(),   pUploaderBuffer + 5 * rawCubemap.faceSize);
                }
                else {
                    for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                        auto rowBegin = rawCubemap.right.begin() + i * rawCubemap.faceRowPitch;
                        auto copyDst = pUploaderBuffer + footprint.subresources[0].offset + i * footprint.subresources[0].rowPitch;
                        std::copy(rowBegin, rowBegin + rawCubemap.faceRowPitch, copyDst);
                    }
                    for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                        auto rowBegin = rawCubemap.left.begin() + i * rawCubemap.faceRowPitch;
                        auto copyDst = pUploaderBuffer + footprint.subresources[1].offset + i * footprint.subresources[1].rowPitch;
                        std::copy(rowBegin, rowBegin + rawCubemap.faceRowPitch, copyDst);
                    }
                    for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                        auto rowBegin = rawCubemap.top.begin() + i * rawCubemap.faceRowPitch;
                        auto copyDst = pUploaderBuffer + footprint.subresources[2].offset + i * footprint.subresources[2].rowPitch;
                        std::copy(rowBegin, rowBegin + rawCubemap.faceRowPitch, copyDst);
                    }
                    for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                        auto rowBegin = rawCubemap.bottom.begin() + i * rawCubemap.faceRowPitch;
                        auto copyDst = pUploaderBuffer + footprint.subresources[3].offset + i * footprint.subresources[3].rowPitch;
                        std::copy(rowBegin, rowBegin + rawCubemap.faceRowPitch, copyDst);
                    }
                    for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                        auto rowBegin = rawCubemap.front.begin() + i * rawCubemap.faceRowPitch;
                        auto copyDst = pUploaderBuffer + footprint.subresources[4].offset + i * footprint.subresources[4].rowPitch;
                        std::copy(rowBegin, rowBegin + rawCubemap.faceRowPitch, copyDst);
                    }
                    for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                        auto rowBegin = rawCubemap.back.begin() + i * rawCubemap.faceRowPitch;
                        auto copyDst = pUploaderBuffer + footprint.subresources[5].offset + i * footprint.subresources[5].rowPitch;
                        std::copy(rowBegin, rowBegin + rawCubemap.faceRowPitch, copyDst);
                    }
                }
                uploaderBuffer->Unmap();

                cubemaps[key] = cubemap;
                uploaderBuffers[key] = uploaderBuffer;
            }
            environment.skyboxTexture = cubemaps.at(key);

            // Build sampler

            auto samplerInfo = rhi::SamplerCreateInfo()
                .SetMinFilter(rhi::Filter::Linear)
                .SetMagFilter(rhi::Filter::Linear)
                .SetMipFilter(rhi::Filter::Linear)
                .SetAddressModeU(rhi::SamplerAddressMode::Repeat)
                .SetAddressModeV(rhi::SamplerAddressMode::Repeat)
                .SetAddressModeW(rhi::SamplerAddressMode::Repeat)
                .SetMinLod(0.0f)
                .SetMaxLod(1.0f);
            environment.skyboxSampler = RenderDevice::GetDevice()->CreateSampler(samplerInfo);
            samplers.emplace_back(environment.skyboxSampler);
        }

        // Copy pixel data to the images

        auto commandPoolInfo = rhi::CommandPoolCreateInfo()
            .SetQueue(RenderDevice::GetQueue());
        auto commandPool = RenderDevice::GetDevice()->CreateCommandPool(commandPoolInfo);

        auto commandBuffer = commandPool->AllocateCommandBuffers(1)[0];
        commandBuffer->Begin(rhi::CommandBufferBeginInfo());

        // Object textures

        for (auto& [key, image] : images) {
            auto imageMemoryBarrier = rhi::ImageMemoryBarrierInfo()
                .SetImage(image)
                .SetOldLayout(rhi::ImageLayout::Undefined)
                .SetNewLayout(rhi::ImageLayout::TransferDst)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(1).SetMipLevelCount(1));
            commandBuffer->ResourceBarrier(imageMemoryBarrier);

            auto bufferRegion = rhi::BufferCopyRegion()
                .SetBuffer(uploaderBuffers[key]);

            auto imageRegion = rhi::ImageCopyRegion()
                .SetImage(image)
                .SetExtent(image->GetExtent())
                .SetSubresourceIndex(rhi::ImageSubresource()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetArrayLayer(0)
                    .SetMipLevel(0));

            commandBuffer->CopyBufferToImage(bufferRegion, imageRegion);

            imageMemoryBarrier.SetOldLayout(rhi::ImageLayout::TransferDst);
            imageMemoryBarrier.SetNewLayout(rhi::ImageLayout::ShaderReadOnly);

            commandBuffer->ResourceBarrier(imageMemoryBarrier);
        }

        // Cubemaps

        for (auto& [key, cubemap] : cubemaps) {
            auto imageMemoryBarrier = rhi::ImageMemoryBarrierInfo()
                .SetImage(cubemap)
                .SetOldLayout(rhi::ImageLayout::Undefined)
                .SetNewLayout(rhi::ImageLayout::TransferDst)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(6).SetMipLevelCount(1));
            commandBuffer->ResourceBarrier(imageMemoryBarrier);

            auto bufferOffset = cubemap->GetCopyableFootprint().size / 6;

            for (uint32_t i = 0; i < 6; i++) {
                auto bufferRegion = rhi::BufferCopyRegion()
                    .SetBuffer(uploaderBuffers[key])
                    .SetOffset(bufferOffset * i);

                auto imageRegion = rhi::ImageCopyRegion()
                    .SetImage(cubemap)
                    .SetExtent(cubemap->GetExtent())
                    .SetSubresourceIndex(rhi::ImageSubresource()
                        .SetImageAspect(rhi::ImageAspect::Color)
                        .SetArrayLayer(i)
                        .SetMipLevel(0));

                commandBuffer->CopyBufferToImage(bufferRegion, imageRegion);
            }

            imageMemoryBarrier.SetOldLayout(rhi::ImageLayout::TransferDst);
            imageMemoryBarrier.SetNewLayout(rhi::ImageLayout::ShaderReadOnly);

            commandBuffer->ResourceBarrier(imageMemoryBarrier);
        }

        commandBuffer->End();

        auto fence = RenderDevice::GetDevice()->CreateFence(rhi::FenceCreateInfo());
        RenderDevice::GetQueue()->Submit(1, &commandBuffer, fence);
        fence->Wait();

        fence->Destroy();
        commandPool->Destroy();

        // Clear useless buffers

        for (auto& [key, buffer] : uploaderBuffers) {
            buffer->Destroy();
        }
    }

}