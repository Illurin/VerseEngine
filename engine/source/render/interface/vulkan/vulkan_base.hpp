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

	class VkEnumSampleCount final {
	public:
		VkEnumSampleCount(RHISampleCount sampleCount) {
			switch (sampleCount) {
			case RHISampleCount::Count1: sampleCount_ = vk::SampleCountFlagBits::e1; break;
			case RHISampleCount::Count2: sampleCount_ = vk::SampleCountFlagBits::e2; break;
			case RHISampleCount::Count4: sampleCount_ = vk::SampleCountFlagBits::e4; break;
			case RHISampleCount::Count8: sampleCount_ = vk::SampleCountFlagBits::e8; break;
			case RHISampleCount::Count16: sampleCount_ = vk::SampleCountFlagBits::e16; break;
			case RHISampleCount::Count32: sampleCount_ = vk::SampleCountFlagBits::e32; break;
			case RHISampleCount::Count64: sampleCount_ = vk::SampleCountFlagBits::e64; break;
			}
		}

		vk::SampleCountFlagBits Get() const { return sampleCount_; }

	private:
		vk::SampleCountFlagBits sampleCount_{ vk::SampleCountFlagBits::e1 };
	};

	class VkEnumBufferUsage final {
	public:
		VkEnumBufferUsage(RHIBufferUsage bufferUsage) {
			switch (bufferUsage) {
			case RHIBufferUsage::UniformBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eUniformBuffer; break;
			case RHIBufferUsage::StorageBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eStorageBuffer; break;
			case RHIBufferUsage::UniformTexelBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eUniformTexelBuffer; break;
			case RHIBufferUsage::StorageTexelBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eStorageTexelBuffer; break;
			case RHIBufferUsage::VertexBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eVertexBuffer; break;
			case RHIBufferUsage::IndexBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eIndexBuffer; break;
			case RHIBufferUsage::TransferSrc: bufferUsage_ = vk::BufferUsageFlagBits::eTransferSrc; break;
			case RHIBufferUsage::TransferDst: bufferUsage_ = vk::BufferUsageFlagBits::eTransferDst; break;
			}
		}

		vk::BufferUsageFlagBits Get() const { return bufferUsage_; }

	private:
		vk::BufferUsageFlagBits bufferUsage_{ vk::BufferUsageFlagBits::eUniformBuffer };
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

	class VkEnumImageType final {
	public:
		VkEnumImageType(RHIImageType imageType) {
			switch (imageType) {
			case RHIImageType::Image1D: imageType_ = vk::ImageType::e1D; break;
			case RHIImageType::Image2D: imageType_ = vk::ImageType::e2D; break;
			case RHIImageType::Image3D: imageType_ = vk::ImageType::e3D; break;
			}
		}

		vk::ImageType Get() const { return imageType_; }

	private:
		vk::ImageType imageType_{ vk::ImageType::e1D };
	};

	class VkEnumImageViewType final {
	public:
		VkEnumImageViewType(RHIImageViewType imageViewType) {
			switch (imageViewType) {
			case RHIImageViewType::Image1D: imageViewType_ = vk::ImageViewType::e1D; break;
			case RHIImageViewType::Image2D: imageViewType_ = vk::ImageViewType::e2D; break;
			case RHIImageViewType::Image3D: imageViewType_ = vk::ImageViewType::e3D; break;
			case RHIImageViewType::ImageCube: imageViewType_ = vk::ImageViewType::eCube; break;
			case RHIImageViewType::ImageArray1D: imageViewType_ = vk::ImageViewType::e1DArray; break;
			case RHIImageViewType::ImageArray2D: imageViewType_ = vk::ImageViewType::e2DArray; break;
			case RHIImageViewType::ImageArrayCube: imageViewType_ = vk::ImageViewType::eCubeArray; break;
			}
		}
		
		vk::ImageViewType Get() const { return imageViewType_; }

	private:
		vk::ImageViewType imageViewType_{ vk::ImageViewType::e1D };
	};

	class VkEnumDescriptorType final {
	public:
		VkEnumDescriptorType(RHIDescriptorType descriptorType) {
			switch (descriptorType) {
			case RHIDescriptorType::Sampler: descriptorType_ = vk::DescriptorType::eSampler; break;
			case RHIDescriptorType::SampledImage: descriptorType_ = vk::DescriptorType::eSampledImage; break;
			case RHIDescriptorType::StorageImage: descriptorType_ = vk::DescriptorType::eStorageImage; break;
			case RHIDescriptorType::UniformBuffer: descriptorType_ = vk::DescriptorType::eUniformBuffer; break;
			case RHIDescriptorType::StorageBuffer: descriptorType_ = vk::DescriptorType::eStorageBuffer; break;
			case RHIDescriptorType::UniformTexelBuffer: descriptorType_ = vk::DescriptorType::eUniformTexelBuffer; break;
			case RHIDescriptorType::StorageTexelBuffer: descriptorType_ = vk::DescriptorType::eStorageTexelBuffer; break;
			}
		}

		vk::DescriptorType Get() const { return descriptorType_; }

	private:
		vk::DescriptorType descriptorType_{ vk::DescriptorType::eSampler };
	};

	class VkWrapperDevice final : public RHIDevice_T {
	public:
		VkWrapperDevice& SetPhysicalDevice(vk::PhysicalDevice physicalDevice) { this->physicalDevice = physicalDevice; return *this; }
		VkWrapperDevice& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperDevice& SetQueues(std::vector<RHIQueue> queues) { this->queues = queues; return *this; }

		vk::PhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
		vk::Device GetDevice() const { return device; }
		std::vector<RHIQueue> GetQueues() const { return queues; }
		
	private:
		vk::PhysicalDevice physicalDevice;
		vk::Device device;
		std::vector<RHIQueue> queues;
	};

	class VkWrapperQueue final : public RHIQueue_T {
	public:
		VkWrapperQueue& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperQueue& SetQueueFamilyIndex(uint32_t queueFamilyIndex) { this->queueFamilyIndex = queueFamilyIndex; return *this; }
		VkWrapperQueue& SetQueue(vk::Queue queue) { this->queue = queue; return *this; }

		vk::Device GetDevice() const { return device; }
		uint32_t GetQueueFamilyIndex() const { return queueFamilyIndex; }
		vk::Queue GetQueue() const { return queue; }

	private:
		vk::Device device;
		uint32_t queueFamilyIndex;
		vk::Queue queue;
	};

	class VkWrapperSwapchain final : public RHISwapchain_T {
	public:
		VkWrapperSwapchain& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperSwapchain& SetSurface(vk::SurfaceKHR surface) { this->surface = surface; return *this; }
		VkWrapperSwapchain& SetSwapchain(vk::SwapchainKHR swapchain) { this->swapchain = swapchain; return *this; }
		VkWrapperSwapchain& SetImageViews(std::vector<vk::ImageView> imageViews) { this->imageViews = imageViews; return *this; }

		vk::Device GetDevice() const { return device; }
		vk::SurfaceKHR GetSurface() const { return surface; }
		vk::SwapchainKHR GetSwapchain() const { return swapchain; }
		std::vector<vk::ImageView> GetImageViews() const { return imageViews; }

	private:
		vk::Device device;
		vk::SurfaceKHR surface;
		vk::SwapchainKHR swapchain;
		std::vector<vk::ImageView> imageViews;
	};

	class VkWrapperCommandPool final : public RHICommandPool_T {
	public:
		VkWrapperCommandPool& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperCommandPool& SetCommandPool(vk::CommandPool commandPool) { this->commandPool = commandPool; return *this; }
		VkWrapperCommandPool& SetCommandBuffers(std::vector<RHICommandBuffer> commandBuffers) { this->commandBuffers = commandBuffers; return *this; }
		
		vk::Device GetDevice() const { return device; }
		vk::CommandPool GetCommandPool() const { return commandPool; }
		std::vector<RHICommandBuffer> GetCommandBuffers() const { return commandBuffers; }

	private:
		vk::Device device;
		vk::CommandPool commandPool;
		std::vector<RHICommandBuffer> commandBuffers;
	};

	class VkWrapperCommandBuffer final : public RHICommandBuffer_T {
	public:
		VkWrapperCommandBuffer& SetCommandBuffer(vk::CommandBuffer commandBuffer) { this->commandBuffer = commandBuffer; return *this; }
		vk::CommandBuffer GetCommandBuffer() const { return commandBuffer; }

	private:
		vk::CommandBuffer commandBuffer;
	};

	class VkWrapperBuffer final : public RHIBuffer_T {
	public:
		VkWrapperBuffer& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperBuffer& SetBuffer(vk::Buffer buffer) { this->buffer = buffer; return *this; }
		VkWrapperBuffer& SetMemory(vk::DeviceMemory memory) { this->memory = memory; return *this; }

		vk::Device GetDevice() const { return device; }
		vk::Buffer GetBuffer() const { return buffer; }
		vk::DeviceMemory GetMemory() const { return memory; }

	private:
		vk::Device device;
		vk::Buffer buffer;
		vk::DeviceMemory memory;
	};

	class VkWrapperBufferView final : public RHIBufferView_T {
	public:
		VkWrapperBufferView& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperBufferView& SetBufferView(vk::BufferView bufferView) { this->bufferView = bufferView; return *this; }

		vk::Device GetDevice() const { return device; }
		vk::BufferView GetBufferView() const { return bufferView; }

	private:
		vk::Device device;
		vk::BufferView bufferView;
	};

	class VkWrapperImage final : public RHIImage_T {
	public:
		VkWrapperImage& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperImage& SetImage(vk::Image image) { this->image = image; return *this; }
		VkWrapperImage& SetMemory(vk::DeviceMemory memory) { this->memory = memory; return *this; }

		vk::Device GetDevice() const { return device; }
		vk::Image GetImage() const { return image; }
		vk::DeviceMemory GetMemory() const { return memory; }

	private:
		vk::Device device;
		vk::Image image;
		vk::DeviceMemory memory;
	};

	class VkWrapperImageView final : public RHIImageView_T {
	public:
		VkWrapperImageView& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperImageView& SetImageView(vk::ImageView imageView) { this->imageView = imageView; return *this; }

		vk::Device GetDevice() const { return device; }
		vk::ImageView GetImageView() const { return imageView; }

	private:
		vk::Device device;
		vk::ImageView imageView;
	};

	class VkWrapperDescriptorPool final : public RHIDescriptorPool_T {
	public:
		VkWrapperDescriptorPool& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperDescriptorPool& SetDescriptorPool(vk::DescriptorPool descriptorPool) { this->descriptorPool = descriptorPool; return *this; }

		vk::Device GetDevice() const { return device; }
		vk::DescriptorPool GetDescriptorPool() const { return descriptorPool; }

	private:
		vk::Device device;
		vk::DescriptorPool descriptorPool;
	};

	class VkWrapperShaderModule final : public RHIShaderModule_T {
	public:
		VkWrapperShaderModule& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperShaderModule& SetShaderModule(vk::ShaderModule shaderModule) { this->shaderModule = shaderModule; return *this; }

		vk::Device GetDevice() const { return device; }
		vk::ShaderModule GetShaderModule() const { return shaderModule; }

	private:
		vk::Device device;
		vk::ShaderModule shaderModule;
	};

	class VkWrapperPipeline final : public RHIPipeline_T {
	public:
		VkWrapperPipeline& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperPipeline& SetPipeline(vk::Pipeline pipeline) { this->pipeline = pipeline; return *this; }

		vk::Device GetDevice() const { return device; }
		vk::Pipeline GetPipeline() const { return pipeline; }

	private:
		vk::Device device;
		vk::Pipeline pipeline;
	};

}