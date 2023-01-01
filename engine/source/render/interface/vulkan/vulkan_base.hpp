#pragma once

#include "vulkan_util.hpp"

namespace engine {

	class VkEnumFormat final {
	public:
		VkEnumFormat(RHIFormat format) {
			switch (format) {
			case RHIFormat::Undefined: format_ = vk::Format::eUndefined; break;
			case RHIFormat::R8Unorm: format_ = vk::Format::eR8Unorm; break;
			case RHIFormat::R8G8Unorm: format_ = vk::Format::eR8G8Unorm; break;
			case RHIFormat::R8G8B8Unorm: format_ = vk::Format::eR8G8B8Unorm; break;
			case RHIFormat::R8G8B8A8Unorm: format_ = vk::Format::eR8G8B8A8Unorm; break;
			case RHIFormat::R8Snorm: format_ = vk::Format::eR8Snorm; break;
			case RHIFormat::R8G8Snorm: format_ = vk::Format::eR8G8Snorm; break;
			case RHIFormat::R8G8B8Snorm: format_ = vk::Format::eR8G8B8Snorm; break;
			case RHIFormat::R8G8B8A8Snorm: format_ = vk::Format::eR8G8B8A8Snorm; break;
			case RHIFormat::R8Uint: format_ = vk::Format::eR8Uint; break;
			case RHIFormat::R8G8Uint: format_ = vk::Format::eR8G8Uint; break;
			case RHIFormat::R8G8B8Uint: format_ = vk::Format::eR8G8B8Uint; break;
			case RHIFormat::R8G8B8A8Uint: format_ = vk::Format::eR8G8B8A8Uint; break;
			case RHIFormat::R8Sint: format_ = vk::Format::eR8Sint; break;
			case RHIFormat::R8G8Sint: format_ = vk::Format::eR8G8Sint; break;
			case RHIFormat::R8G8B8Sint: format_ = vk::Format::eR8G8B8Sint; break;
			case RHIFormat::R8G8B8A8Sint: format_ = vk::Format::eR8G8B8A8Sint; break;
			case RHIFormat::R16Unorm: format_ = vk::Format::eR16Unorm; break;
			case RHIFormat::R16G16Unorm: format_ = vk::Format::eR16G16Unorm; break;
			case RHIFormat::R16G16B16Unorm: format_ = vk::Format::eR16G16B16Unorm; break;
			case RHIFormat::R16G16B16A16Unorm: format_ = vk::Format::eR16G16B16A16Unorm; break;
			case RHIFormat::R16Snorm: format_ = vk::Format::eR16Snorm; break;
			case RHIFormat::R16G16Snorm: format_ = vk::Format::eR16G16Snorm; break;
			case RHIFormat::R16G16B16Snorm: format_ = vk::Format::eR16G16B16Snorm; break;
			case RHIFormat::R16G16B16A16Snorm: format_ = vk::Format::eR16G16B16A16Snorm; break;
			case RHIFormat::R16Uint: format_ = vk::Format::eR16Uint; break;
			case RHIFormat::R16G16Uint: format_ = vk::Format::eR16G16Uint; break;
			case RHIFormat::R16G16B16Uint: format_ = vk::Format::eR16G16B16Uint; break;
			case RHIFormat::R16G16B16A16Uint: format_ = vk::Format::eR16G16B16A16Uint; break;
			case RHIFormat::R16Sint: format_ = vk::Format::eR16Sint; break;
			case RHIFormat::R16G16Sint: format_ = vk::Format::eR16G16Sint; break;
			case RHIFormat::R16G16B16Sint: format_ = vk::Format::eR16G16B16Sint; break;
			case RHIFormat::R16G16B16A16Sint: format_ = vk::Format::eR16G16B16A16Sint; break;
			case RHIFormat::R16Sfloat: format_ = vk::Format::eR16Sfloat; break;
			case RHIFormat::R16G16Sfloat: format_ = vk::Format::eR16G16Sfloat; break;
			case RHIFormat::R16G16B16Sfloat: format_ = vk::Format::eR16G16B16Sfloat; break;
			case RHIFormat::R16G16B16A16Sfloat: format_ = vk::Format::eR16G16B16A16Sfloat; break;
			case RHIFormat::R32Uint: format_ = vk::Format::eR32Uint; break;
			case RHIFormat::R32G32Uint: format_ = vk::Format::eR32G32Uint; break;
			case RHIFormat::R32G32B32Uint: format_ = vk::Format::eR32G32B32Uint; break;
			case RHIFormat::R32G32B32A32Uint: format_ = vk::Format::eR32G32B32A32Uint; break;
			case RHIFormat::R32Sint: format_ = vk::Format::eR32Sint; break;
			case RHIFormat::R32G32Sint: format_ = vk::Format::eR32G32Sint; break;
			case RHIFormat::R32G32B32Sint: format_ = vk::Format::eR32G32B32Sint; break;
			case RHIFormat::R32G32B32A32Sint: format_ = vk::Format::eR32G32B32A32Sint; break;
			case RHIFormat::R32Sfloat: format_ = vk::Format::eR32Sfloat; break;
			case RHIFormat::R32G32Sfloat: format_ = vk::Format::eR32G32Sfloat; break;
			case RHIFormat::R32G32B32Sfloat: format_ = vk::Format::eR32G32B32Sfloat; break;
			case RHIFormat::R32G32B32A32Sfloat: format_ = vk::Format::eR32G32B32A32Sfloat; break;
			case RHIFormat::R64Uint: format_ = vk::Format::eR64Uint; break;
			case RHIFormat::R64G64Uint: format_ = vk::Format::eR64G64Uint; break;
			case RHIFormat::R64G64B64Uint: format_ = vk::Format::eR64G64B64Uint; break;
			case RHIFormat::R64G64B64A64Uint: format_ = vk::Format::eR64G64B64A64Uint; break;
			case RHIFormat::R64Sint: format_ = vk::Format::eR64Sint; break;
			case RHIFormat::R64G64Sint: format_ = vk::Format::eR64G64Sint; break;
			case RHIFormat::R64G64B64Sint: format_ = vk::Format::eR64G64B64Sint; break;
			case RHIFormat::R64G64B64A64Sint: format_ = vk::Format::eR64G64B64A64Sint; break;
			case RHIFormat::R64Sfloat: format_ = vk::Format::eR64Sfloat; break;
			case RHIFormat::R64G64Sfloat: format_ = vk::Format::eR64G64Sfloat; break;
			case RHIFormat::R64G64B64Sfloat: format_ = vk::Format::eR64G64B64Sfloat; break;
			case RHIFormat::R64G64B64A64Sfloat: format_ = vk::Format::eR64G64B64A64Sfloat; break;
			case RHIFormat::D16Unorm: format_ = vk::Format::eD16Unorm; break;
			case RHIFormat::D32Sfloat: format_ = vk::Format::eD32Sfloat; break;
			case RHIFormat::S8Uint: format_ = vk::Format::eS8Uint; break;
			case RHIFormat::D16UnormS8Uint: format_ = vk::Format::eD16UnormS8Uint; break;
			case RHIFormat::D24UnormS8Uint: format_ = vk::Format::eD24UnormS8Uint; break;
			case RHIFormat::D32SfloatS8Uint: format_ = vk::Format::eD32SfloatS8Uint; break;
			}
		}

		vk::Format Get() const { return format_; }

	private:
		vk::Format format_{ vk::Format::eUndefined };
	};

	class VkEnumQueueType final {
	public:
		VkEnumQueueType(RHIQueueType queueType) {
			switch (queueType) {
			case RHIQueueType::Graphics: queueType_ = vk::QueueFlagBits::eGraphics; break;
			case RHIQueueType::Compute: queueType_ = vk::QueueFlagBits::eCompute; break;
			case RHIQueueType::Transfer: queueType_ = vk::QueueFlagBits::eTransfer; break;
			case RHIQueueType::VideoDecode: queueType_ = vk::QueueFlagBits::eVideoDecodeKHR; break;
			case RHIQueueType::VideoEncode: queueType_ = vk::QueueFlagBits::eVideoEncodeKHR; break;
			}
		}

		vk::QueueFlagBits Get() const { return queueType_; }

	private:
		vk::QueueFlagBits queueType_{ vk::QueueFlagBits::eGraphics };
	};

	class VkEnumAlphaMode final {
	public:
		VkEnumAlphaMode(RHIAlphaMode alphaMode) {
			switch (alphaMode) {
			case RHIAlphaMode::Undefined: alphaMode_ = vk::CompositeAlphaFlagBitsKHR::eInherit; break;
			case RHIAlphaMode::Opaque: alphaMode_ = vk::CompositeAlphaFlagBitsKHR::eOpaque; break;
			case RHIAlphaMode::PreMultiplied: alphaMode_ = vk::CompositeAlphaFlagBitsKHR::ePreMultiplied; break;
			case RHIAlphaMode::PostMultiplied: alphaMode_ = vk::CompositeAlphaFlagBitsKHR::ePostMultiplied; break;
			}
		}

		vk::CompositeAlphaFlagBitsKHR Get() const { return alphaMode_; }

	private:
		vk::CompositeAlphaFlagBitsKHR alphaMode_{ vk::CompositeAlphaFlagBitsKHR::eInherit };
	};

	class VkEnumImageUsage final {
	public:
		VkEnumImageUsage(RHIImageUsage imageUsage) {
			switch (imageUsage) {
			case RHIImageUsage::ColorAttachment: imageUsage_ = vk::ImageUsageFlagBits::eColorAttachment; break;
			case RHIImageUsage::DepthStencilAttachment: imageUsage_ = vk::ImageUsageFlagBits::eDepthStencilAttachment; break;
			case RHIImageUsage::Sampled: imageUsage_ = vk::ImageUsageFlagBits::eSampled; break;
			case RHIImageUsage::TransferSrc: imageUsage_ = vk::ImageUsageFlagBits::eTransferSrc; break;
			case RHIImageUsage::TransferDst: imageUsage_ = vk::ImageUsageFlagBits::eTransferDst; break;
			}
		}

		vk::ImageUsageFlagBits Get() const { return imageUsage_; }

	private:
		vk::ImageUsageFlagBits imageUsage_{ vk::ImageUsageFlagBits::eColorAttachment };
	};

	class VkWrapperDevice final : public RHIDevice {
	public:
		VkWrapperDevice& SetPhysicalDevice(vk::PhysicalDevice physicalDevice) { this->physicalDevice = physicalDevice; return *this; }
		VkWrapperDevice& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperDevice& SetQueues(std::vector<RHIQueue*> queues) { this->queues = queues; return *this; }

		vk::PhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
		vk::Device GetDevice() const { return device; }
		std::vector<RHIQueue*> GetQueues() const { return queues; }

	private:
		vk::PhysicalDevice physicalDevice;
		vk::Device device;
		std::vector<RHIQueue*> queues;
	};

	class VkWrapperQueue final : public RHIQueue {
	public:
		VkWrapperQueue& SetQueueFamilyIndex(uint32_t queueFamilyIndex) { this->queueFamilyIndex = queueFamilyIndex; return *this; }
		VkWrapperQueue& SetQueue(vk::Queue queue) { this->queue = queue; return *this; }

		uint32_t GetQueueFamilyIndex() const { return queueFamilyIndex; }
		vk::Queue GetQueue() const { return queue; }

	private:
		uint32_t queueFamilyIndex;
		vk::Queue queue;
	};

	class VkWrapperSwapchain final : public RHISwapchain {
	public:
		VkWrapperSwapchain& SetSurface(vk::SurfaceKHR surface) { this->surface = surface; return *this; }
		VkWrapperSwapchain& SetSwapchain(vk::SwapchainKHR swapchain) { this->swapchain = swapchain; return *this; }
		VkWrapperSwapchain& SetImageViews(std::vector<vk::ImageView> imageViews) { this->imageViews = imageViews; return *this; }

		vk::SurfaceKHR GetSurface() const { return surface; }
		vk::SwapchainKHR GetSwapchain() const { return swapchain; }
		std::vector<vk::ImageView> GetImageViews() const { return imageViews; }

	private:
		vk::SurfaceKHR surface;
		vk::SwapchainKHR swapchain;
		std::vector<vk::ImageView> imageViews;
	};

	class VkWrapperCommandPool final : public RHICommandPool {
	public:
		VkWrapperCommandPool& SetCommandPool(vk::CommandPool commandPool) { this->commandPool = commandPool; return *this; }
		VkWrapperCommandPool& SetCommandBuffers(std::vector<RHICommandBuffer*> commandBuffers) { this->commandBuffers = commandBuffers; return *this; }
		
		vk::CommandPool GetCommandPool() const { return commandPool; }
		std::vector<RHICommandBuffer*> GetCommandBuffers() const { return commandBuffers; }

	private:
		vk::CommandPool commandPool;
		std::vector<RHICommandBuffer*> commandBuffers;
	};

	class VkWrapperCommandBuffer final : public RHICommandBuffer {
	public:
		VkWrapperCommandBuffer& SetCommandBuffer(vk::CommandBuffer commandBuffer) { this->commandBuffer = commandBuffer; return *this; }
		vk::CommandBuffer GetCommandBuffer() const { return commandBuffer; }

	private:
		vk::CommandBuffer commandBuffer;
	};

}