#include "resource_manager.hpp"

#include "runtime/render/render_device.hpp"
#include "asset_loader/image/image_loader.hpp"
#include "asset_loader/model/model_loader.hpp"

namespace engine {

    uint32_t ResourceManager::ImportTexture(std::filesystem::path path) {
        for (uint32_t i = 0; i < textureResources.size(); i++) {
            auto& texture = textureResources[i];
            if (!texture.isCubemap && texture.path == path) {
                return i;
            }
        }

        TextureResource texture;
        texture.path = path;
        auto id = static_cast<uint32_t>(textureResources.size());
        textureResources.emplace_back(texture);
        return id;
    }

    uint32_t ResourceManager::ImportCubemap(std::filesystem::path path) {
        for (uint32_t i = 0; i < textureResources.size(); i++) {
            auto& texture = textureResources[i];
            if (texture.isCubemap && texture.path == path) {
                return i;
            }
        }

        TextureResource texture;
        texture.isCubemap = true;
        texture.path = path;
        auto id = static_cast<uint32_t>(textureResources.size());
        textureResources.emplace_back(texture);
        return id;
    }

    TextureResource& ResourceManager::GetTexture(uint32_t id) {
        return textureResources[id];
    }

    uint32_t ResourceManager::CreateMaterial(const MaterialData& data) {
        MaterialResource material;
        material.data = data;
        auto id = static_cast<uint32_t>(materialResources.size());
        materialResources.emplace_back(material);
        return id;
    }

    MaterialResource& ResourceManager::GetMaterial(uint32_t id) {
        return materialResources[id];
    }

    void ResourceManager::LoadResources() {

        // Create material

        for (auto& material : materialResources) {
            if (!material.preLoad) continue;
            if (material.uniformBuffer) material.uniformBuffer->Destroy();
            if (material.sampler) material.sampler->Destroy();
            material.preLoad = false;

            auto uniformBufferInfo = rhi::BufferCreateInfo()
                .SetSize(sizeof(MaterialConstants))
                .SetUsage(rhi::BufferUsage::UniformBuffer);
            material.uniformBuffer = RenderDevice::GetDevice()->CreateBuffer(uniformBufferInfo);

            MaterialConstants materialConstants{
                material.data.diffuseAlbedo,
                material.data.fresnelR0,
                material.data.roughness
            };

            auto p = reinterpret_cast<MaterialConstants*>(material.uniformBuffer->Map());
            *p = materialConstants;
            material.uniformBuffer->Unmap();

            material.sampler = RenderDevice::GetDevice()->CreateSampler(material.data.samplerInfo);

            material.diffuseMapResource = ImportTexture(material.data.diffuseMap);
            material.normalMapResource = ImportTexture(material.data.normalMap);
        }

        // Load pixel data from file

        std::vector<std::pair<uint32_t, rhi::Buffer>> uploaderBuffers;

        for (uint32_t i = 0; i < textureResources.size(); i++) {
            auto& texture = textureResources[i];
            if (!texture.preLoad) continue;
            if (texture.image) texture.image->Destroy();
            texture.preLoad = false;
            
            if (!texture.isCubemap) {
                auto rawImage = asset::ImageLoader::Load(texture.path, 32);

                auto imageInfo = rhi::ImageCreateInfo()
                    .SetFormat(rhi::Format::R8G8B8A8Unorm)
                    .SetImageType(rhi::ImageType::Image2D)
                    .SetExtent(rhi::Extent3D().SetWidth(rawImage.width).SetHeight(rawImage.height).SetDepth(1))
                    .SetArrayLayers(1)
                    .SetMipLevels(1)
                    .SetUsage(rhi::ImageUsage::SampledAndTransferDst);
                texture.image = RenderDevice::GetDevice()->CreateImage(imageInfo);

                auto footprint = texture.image->GetCopyableFootprint();

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

                uploaderBuffers.emplace_back(std::make_pair(i, uploaderBuffer));
            }
            else {
                auto rawCubemap = asset::ImageLoader::LoadCubemap(texture.path, 32);

                auto imageInfo = rhi::ImageCreateInfo()
                    .SetFormat(rhi::Format::R8G8B8A8Unorm)
                    .SetImageType(rhi::ImageType::Image2D)
                    .SetExtent(rhi::Extent3D().SetWidth(rawCubemap.faceWidth).SetHeight(rawCubemap.faceHeight).SetDepth(1))
                    .SetArrayLayers(6)
                    .SetMipLevels(1)
                    .SetUsage(rhi::ImageUsage::SampledAndTransferDst);
                texture.image = RenderDevice::GetDevice()->CreateImage(imageInfo);

                auto footprint = texture.image->GetCopyableFootprint();

                auto bufferInfo = rhi::BufferCreateInfo()
                    .SetSize(footprint.size)
                    .SetUsage(rhi::BufferUsage::TransferSrc);
                auto uploaderBuffer = RenderDevice::GetDevice()->CreateBuffer(bufferInfo);

                auto pUploaderBuffer = reinterpret_cast<uint8_t*>(uploaderBuffer->Map());
                if (footprint.subresources.empty()) {
                    std::copy(rawCubemap.right.begin(), rawCubemap.right.end(), pUploaderBuffer);
                    std::copy(rawCubemap.left.begin(), rawCubemap.left.end(), pUploaderBuffer + rawCubemap.faceSize);
                    std::copy(rawCubemap.top.begin(), rawCubemap.top.end(), pUploaderBuffer + 2 * rawCubemap.faceSize);
                    std::copy(rawCubemap.bottom.begin(), rawCubemap.bottom.end(), pUploaderBuffer + 3 * rawCubemap.faceSize);
                    std::copy(rawCubemap.front.begin(), rawCubemap.front.end(), pUploaderBuffer + 4 * rawCubemap.faceSize);
                    std::copy(rawCubemap.back.begin(), rawCubemap.back.end(), pUploaderBuffer + 5 * rawCubemap.faceSize);
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

                uploaderBuffers.emplace_back(std::make_pair(i, uploaderBuffer));
            }
        }

        // Prepare command buffer

        auto commandPoolInfo = rhi::CommandPoolCreateInfo()
            .SetQueue(RenderDevice::GetQueue());
        auto commandPool = RenderDevice::GetDevice()->CreateCommandPool(commandPoolInfo);

        auto commandBuffer = commandPool->AllocateCommandBuffers(1)[0];
        commandBuffer->Begin(rhi::CommandBufferBeginInfo());

        // Copy pixel data to images

        for (auto [id, uploaderBuffer] : uploaderBuffers) {
            auto& texture = textureResources[id];

            if (!texture.isCubemap) {
                auto imageMemoryBarrier = rhi::ImageMemoryBarrierInfo()
                    .SetImage(texture.image)
                    .SetOldLayout(rhi::ImageLayout::Undefined)
                    .SetNewLayout(rhi::ImageLayout::TransferDst)
                    .SetSubresourceRange(rhi::ImageSubresourceRange()
                        .SetImageAspect(rhi::ImageAspect::Color)
                        .SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(1).SetMipLevelCount(1));
                commandBuffer->ResourceBarrier(imageMemoryBarrier);

                auto bufferRegion = rhi::BufferCopyRegion()
                    .SetBuffer(uploaderBuffer);

                auto imageRegion = rhi::ImageCopyRegion()
                    .SetImage(texture.image)
                    .SetExtent(texture.image->GetExtent())
                    .SetSubresourceIndex(rhi::ImageSubresource()
                        .SetImageAspect(rhi::ImageAspect::Color)
                        .SetArrayLayer(0)
                        .SetMipLevel(0));

                commandBuffer->CopyBufferToImage(bufferRegion, imageRegion);

                imageMemoryBarrier.SetOldLayout(rhi::ImageLayout::TransferDst);
                imageMemoryBarrier.SetNewLayout(rhi::ImageLayout::ShaderReadOnly);

                commandBuffer->ResourceBarrier(imageMemoryBarrier);
            }
            else {
                auto imageMemoryBarrier = rhi::ImageMemoryBarrierInfo()
                    .SetImage(texture.image)
                    .SetOldLayout(rhi::ImageLayout::Undefined)
                    .SetNewLayout(rhi::ImageLayout::TransferDst)
                    .SetSubresourceRange(rhi::ImageSubresourceRange()
                        .SetImageAspect(rhi::ImageAspect::Color)
                        .SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(6).SetMipLevelCount(1));
                commandBuffer->ResourceBarrier(imageMemoryBarrier);

                auto bufferOffset = texture.image->GetCopyableFootprint().size / 6;

                for (uint32_t i = 0; i < 6; i++) {
                    auto bufferRegion = rhi::BufferCopyRegion()
                        .SetBuffer(uploaderBuffer)
                        .SetOffset(bufferOffset * i);

                    auto imageRegion = rhi::ImageCopyRegion()
                        .SetImage(texture.image)
                        .SetExtent(texture.image->GetExtent())
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
        }

        // Finish copying and clean useless buffers

        commandBuffer->End();

        auto fence = RenderDevice::GetDevice()->CreateFence(rhi::FenceCreateInfo());
        RenderDevice::GetQueue()->Submit(1, &commandBuffer, fence);
        fence->Wait();

        fence->Destroy();
        commandPool->Destroy();

        for (auto [id, uploaderBuffer] : uploaderBuffers) {
            uploaderBuffer->Destroy();
        }
    }

    void ResourceManager::Clear() {
        for (auto& texture : textureResources) {
            if (texture.image) {
                texture.image->Destroy();
                texture.image = nullptr;
            }
        }
        for (auto& material : materialResources) {
            if (material.uniformBuffer) {
                material.uniformBuffer->Destroy();
                material.uniformBuffer = nullptr;
            }
            if (material.sampler) {
                material.sampler->Destroy();
                material.sampler = nullptr;
            }
        }
    }

}