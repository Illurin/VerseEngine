#pragma once

#include "rhi_struct.hpp"

#include "render/shader/shader_compiler.hpp"

namespace engine {
	namespace rhi {

		class Instance_T {
		public:
			virtual void Init(const InstanceInitInfo&) = 0;
			virtual void Destroy() = 0;

			virtual std::vector<PhysicalDeviceInfo> GetPhysicalDeviceInfo() const = 0;

			virtual Device CreateDevice(const DeviceCreateInfo&) const = 0;
			virtual ShaderCompiler CreateShaderCompiler() const = 0;
		};

		class Device_T {
		public:
			virtual void Destroy() = 0;

			virtual Queue GetQueue(uint32_t queueIndex) const = 0;

			virtual Swapchain CreateSwapchain(const SwapchainCreateInfo&) const = 0;
			virtual CommandPool CreateCommandPool(const CommandPoolCreateInfo&) const = 0;
			virtual Buffer CreateBuffer(const BufferCreateInfo&) const = 0;
			virtual Image CreateImage(const ImageCreateInfo&) const = 0;
			//virtual DescriptorPool CreateDescriptorPool(const DescriptorPoolCreateInfo&) const = 0;
			virtual ShaderModule CreateShaderModule(const ShaderModuleCreateInfo&) const = 0;
			virtual Pipeline CreateGraphicsPipeline(const GraphicsPipelineCreateInfo&) const = 0;
			virtual Framebuffer CreateFramebuffer(const FramebufferCreateInfo&) const = 0;
			virtual Fence CreateFence(const FenceCreateInfo&) const = 0;
		};

		class Queue_T {
		public:
			
		};
		
		class Swapchain_T {
		public:
			virtual void Destroy() = 0;

			virtual std::vector<rhi::Image> GetImages() const = 0;
			virtual uint32_t AcquireNextImage() = 0;
		};

		class CommandPool_T {
		public:
			virtual void Destroy() = 0;

			virtual void Reset() = 0;

			virtual std::vector<rhi::CommandBuffer> AllocateCommandBuffers(uint32_t bufferCount) = 0;
		};

		class CommandBuffer_T {
		public:
			virtual void Reset() = 0;
			virtual void Begin(const CommandBufferBeginInfo&) = 0;
			virtual void End() = 0;
			
			virtual void BeginRenderPass(const RenderPassBeginInfo&) = 0;
			virtual void EndRenderPass() = 0;
			virtual void BindPipeline(const Pipeline&) = 0;
			virtual void BindVertexBuffer(uint32_t bindingCount, Buffer* pBuffer, uint64_t* pOffsets) = 0;
			virtual void BindIndexBuffer(Buffer& buffer, uint64_t offset, IndexType indexType) = 0;

			virtual void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstCount, uint32_t firstInstance) const = 0;
			virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstCount, int32_t vertexOffset, uint32_t firstInstance) const = 0;
		};

		class Buffer_T {
		public:
			virtual void Destroy() = 0;
		};

		class Image_T {
		public:
			virtual void Destroy() = 0;
		};

		class ShaderModule_T {
		public:
			virtual void Destroy() = 0;

		};

		class Pipeline_T {
		public:
			virtual void Destroy() = 0;


		};

		class Framebuffer_T {
		public:
			virtual void Destroy() = 0;


		};

		class Fence_T {
		public:
			virtual void Destroy() = 0;

			virtual void Reset() = 0;
			virtual void Wait() = 0;

			virtual uint32_t GetCurrentStatus() const = 0;
		};

	}
}