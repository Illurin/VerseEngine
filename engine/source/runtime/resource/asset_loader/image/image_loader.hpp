#pragma once

#include "stb_loader.hpp"
#include "runtime/render/interface/rhi.hpp"

namespace engine {
	namespace asset {

		struct RawImage {
			uint32_t width, height, bpp;
			uint64_t rowPitch;
			uint64_t size;
			std::vector<uint8_t> source;
		};

		class ImageLoader {
		public:
			static RawImage Load(const char* path, uint32_t bpp) {
				RawImage rawImage;

				StbImageLoader loader(path);
				rawImage.width = static_cast<uint32_t>(loader.GetWidth());
				rawImage.height = static_cast<uint32_t>(loader.GetHeight());
				rawImage.bpp = bpp;

				rawImage.rowPitch = (static_cast<uint64_t>(rawImage.width) * static_cast<uint64_t>(bpp) + 7) / 8;
				rawImage.size = rawImage.rowPitch * static_cast<uint64_t>(rawImage.height);

				rawImage.source.resize(rawImage.size);
				auto source = loader.GetSource();
				std::copy(source, source + rawImage.size, rawImage.source.data());

				return rawImage;
			}

			static rhi::Image SetupImage(const RawImage& rawImage, rhi::Device device, rhi::Queue queue) {
				auto imageInfo = rhi::ImageCreateInfo()
					.SetFormat(rhi::Format::R8G8B8A8Unorm)
					.SetImageType(rhi::ImageType::Image2D)
					.SetExtent(rhi::Extent3D().SetWidth(rawImage.width).SetHeight(rawImage.height).SetDepth(1))
					.SetArrayLayers(1)
					.SetMipLevels(1)
					.SetUsage(rhi::ImageUsage::SampledAndTransferDst);
				auto image = device->CreateImage(imageInfo);

				auto footprint = image->GetCopyableFootprint();

				auto bufferInfo = rhi::BufferCreateInfo()
					.SetSize(footprint.size)
					.SetUsage(rhi::BufferUsage::TransferSrc);
				auto uploaderBuffer = device->CreateBuffer(bufferInfo);

				auto pUploaderBuffer = reinterpret_cast<uint8_t*>(uploaderBuffer->Map());
				if (footprint.rowPitch == 0) {
					std::copy(rawImage.source.begin(), rawImage.source.end(), pUploaderBuffer);
				}
				else {
					for (uint32_t i = 0; i < rawImage.height; i++) {
						auto rowBegin = rawImage.source.begin() + i * rawImage.rowPitch;
						std::copy(rowBegin, rowBegin + rawImage.rowPitch, pUploaderBuffer + i * footprint.rowPitch);
					}
				}
				uploaderBuffer->Unmap();

				auto cmdPool = device->CreateCommandPool(rhi::CommandPoolCreateInfo().SetQueue(queue));
				auto cmd = cmdPool->AllocateCommandBuffers(1)[0];
				cmd->Begin(rhi::CommandBufferBeginInfo());

				auto imageMemoryBarrier = rhi::ImageMemoryBarrierInfo()
					.SetImage(image)
					.SetOldLayout(rhi::ImageLayout::Undefined)
					.SetNewLayout(rhi::ImageLayout::TransferDst)
					.SetSubresourceRange(rhi::ImageSubresourceRange()
						.SetImageAspect(rhi::ImageAspect::Color)
						.SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(1).SetMipLevelCount(1));

				cmd->ResourceBarrier(imageMemoryBarrier);

				auto bufferRegion = rhi::BufferCopyRegion()
					.SetBuffer(uploaderBuffer);

				auto imageRegion = rhi::ImageCopyRegion()
					.SetImage(image)
					.SetExtent(image->GetExtent())
					.SetSubresourceIndex(rhi::ImageSubresource()
						.SetImageAspect(rhi::ImageAspect::Color)
						.SetMipLevel(0)
						.SetArrayLayer(0));

				cmd->CopyBufferToImage(bufferRegion, imageRegion);

				imageMemoryBarrier.SetOldLayout(rhi::ImageLayout::TransferDst);
				imageMemoryBarrier.SetNewLayout(rhi::ImageLayout::ShaderReadOnly);

				cmd->ResourceBarrier(imageMemoryBarrier);

				cmd->End();

				auto fence = device->CreateFence(rhi::FenceCreateInfo());
				queue->Submit(1, &cmd, fence);
				fence->Wait();

				cmdPool->Destroy();
				fence->Destroy();
				uploaderBuffer->Destroy();

				return image;
			}
		};

	}
}