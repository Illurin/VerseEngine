#include "resource_system.hpp"

#include "runtime/resource/asset_loader/image/image_loader.hpp"
#include "runtime/render/render_device.hpp"

namespace engine {

	ResourceSystem::~ResourceSystem() {
		for (auto& [key, image] : images) {
			image->Destroy();
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
				images[key] = RenderDevice::GetDevice()->CreateImage(imageInfo);

				auto footprint = images[key]->GetCopyableFootprint();

				auto bufferInfo = rhi::BufferCreateInfo()
					.SetSize(footprint.size)
					.SetUsage(rhi::BufferUsage::TransferSrc);
				uploaderBuffers[key] = RenderDevice::GetDevice()->CreateBuffer(bufferInfo);

				auto pUploaderBuffer = reinterpret_cast<uint8_t*>(uploaderBuffers[key]->Map());
				if (footprint.rowPitch == 0) {
					std::copy(rawImage.source.begin(), rawImage.source.end(), pUploaderBuffer);
				}
				else {
					for (uint32_t i = 0; i < rawImage.height; i++) {
						auto rowBegin = rawImage.source.begin() + i * rawImage.rowPitch;
						std::copy(rowBegin, rowBegin + rawImage.rowPitch, pUploaderBuffer + i * footprint.rowPitch);
					}
				}
				uploaderBuffers[key]->Unmap();
			}
			material.texture = images.at(key);

			// Build sampler

			material.sampler = RenderDevice::GetDevice()->CreateSampler(material.samplerInfo);
			samplers.emplace_back(material.sampler);
		});

		// Copy pixel data to the images

		auto commandPoolInfo = rhi::CommandPoolCreateInfo()
			.SetQueue(RenderDevice::GetQueue());
		auto commandPool = RenderDevice::GetDevice()->CreateCommandPool(commandPoolInfo);

		auto commandBuffer = commandPool->AllocateCommandBuffers(1)[0];
		commandBuffer->Begin(rhi::CommandBufferBeginInfo());

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
					.SetMipLevel(0)
					.SetArrayLayer(0));

			commandBuffer->CopyBufferToImage(bufferRegion, imageRegion);

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