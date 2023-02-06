#pragma once

#include "rhi_struct.hpp"

#include "runtime/render/shader/shader_compiler.hpp"

namespace engine {
	namespace rhi {

		//
		// The Instance handle is a specific graphics hardware instance of RHI.
		// Specify the graphics backend to use by creating an Instance.
		//
		class Instance_T {
		public:
			virtual void Init(const InstanceCreateInfo&) = 0;
			virtual void Destroy() = 0;

			virtual std::vector<PhysicalDeviceInfo> GetPhysicalDeviceInfo() const = 0;

			virtual Device CreateDevice(const DeviceCreateInfo&) const = 0;
			virtual ShaderCompiler CreateShaderCompiler() const = 0;
		};

		//
		// The Device handle is associated with specific graphics physical hardware.
		// Most of RHI handles can be created from a device.
		// The lifetime of a device should be global.
		// Don not destroy it until you destroy all the handles created from it.
		//
		class Device_T {
		public:
			virtual void Destroy() = 0;

			virtual Queue GetQueue(uint32_t queueIndex) const = 0;

			virtual Swapchain CreateSwapchain(const SwapchainCreateInfo&) const = 0;
			virtual CommandPool CreateCommandPool(const CommandPoolCreateInfo&) const = 0;
			virtual Buffer CreateBuffer(const BufferCreateInfo&) const = 0;
			virtual Image CreateImage(const ImageCreateInfo&) const = 0;
			virtual Sampler CreateSampler(const SamplerCreateInfo&) const = 0;
			virtual DescriptorPool CreateDescriptorPool(const DescriptorPoolCreateInfo&) const = 0;
			virtual DescriptorSetLayout CreateDescriptorSetLayout(const DescriptorSetLayoutCreateInfo&) const = 0;
			virtual PipelineLayout CreatePipelineLayout(const PipelineLayoutCreateInfo&) const = 0;
			virtual RenderPass CreateRenderPass(const RenderPassCreateInfo&) const = 0;
			virtual ShaderModule CreateShaderModule(const ShaderModuleCreateInfo&) const = 0;
			virtual Pipeline CreateGraphicsPipeline(const GraphicsPipelineCreateInfo&) const = 0;
			virtual Framebuffer CreateFramebuffer(const FramebufferCreateInfo&) const = 0;
			virtual Fence CreateFence(const FenceCreateInfo&) const = 0;
		};


		//
		// The Queue handle is created at the same time when the Device handle is created.
		// Specific graphics hardware operations will be executed on the queue.
		//
		class Queue_T {
		public:
			virtual void SubmitCommandBuffers(uint32_t commandBufferCount, CommandBuffer* commandBuffers, rhi::Fence fence) = 0;
			virtual void PresentSwapchain(Swapchain swapchain, uint32_t imageIndex) = 0;
		};
		
		//
		// The Swapchain handle is used for present the rendering result to the display screen.
		// Platform information is needed for creating a platform dependent swapchain.
		//
		class Swapchain_T {
		public:
			virtual void Destroy() = 0;

			virtual rhi::Extent2D GetImageExtent() const = 0;
			virtual std::vector<rhi::Image> GetImages() const = 0;

			virtual uint32_t AcquireNextImage(Fence) = 0;
		};

		//
		// The CommandPool handle is a container for storing and managing command buffers.
		// If command pool is destroyed or reset, the command buffers in it will also be destroyed.
		//
		class CommandPool_T {
		public:
			virtual void Destroy() = 0;

			virtual void Reset() = 0;
			virtual void Free(uint32_t commandBufferCount, rhi::CommandBuffer* pCommandBuffers) = 0;

			virtual std::vector<rhi::CommandBuffer> AllocateCommandBuffers(uint32_t bufferCount) = 0;
		};

		//
		// The CommandBuffer handle is created from a CommandPool handle.
		// Rendering commands will be stored in the command buffer, until it is reset.
		// Submit command buffers to the queue to execute rendering commands.
		//
		class CommandBuffer_T {
		public:
			virtual void Reset() = 0;
			virtual void Begin(const CommandBufferBeginInfo&) = 0;
			virtual void End() = 0;
			
			virtual void BeginRenderPass(const RenderPassBeginInfo&) = 0;
			virtual void EndRenderPass() = 0;
			virtual void BindPipeline(const Pipeline&) = 0;
			virtual void BindVertexBuffer(uint32_t firstBinding, uint32_t bindingCount, const Buffer* pBuffers) = 0;
			virtual void BindIndexBuffer(Buffer& buffer, uint64_t offset, IndexType indexType) = 0;

			virtual void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) = 0;
			virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) = 0;
		};

		//
		// The Buffer handle represents for a storage space that can be used for GPU reading and writing.
		// Use the Map and Unmap function to read and write it via the CPU side.
		//
		class Buffer_T {
		public:
			virtual void Destroy() = 0;

			virtual void* Map() = 0;
			virtual void* Map(uint64_t offset, uint64_t size) = 0;
			virtual void Unmap() = 0;
		};

		//
		// The Image handle represents for another from of storage space which stores continuous image data.
		// Use an intermediate buffer to transfer data to image.
		//
		class Image_T {
		public:
			virtual void Destroy() = 0;
		};

		//
		// The Sampler handle is used for sample textures in the fragment shader.
		// It can be binded to descriptor set or used as a static sampler.
		//
		class Sampler_T {
		public:
			virtual void Destroy() = 0;
		};

		//
		// The DescriptorPool handle represents for a space used to contain descriptors.
		// If descriptor pool is destroyed or reset, the descriptors in it will also be destroyed.
		//
		class DescriptorPool_T {
		public:
			virtual void Destroy() = 0;

			virtual void Reset() = 0;
			virtual void Free(uint32_t descriptorCount, rhi::DescriptorSet* pDescriptorSets) = 0;

			virtual std::vector<DescriptorSet> AllocateDescriptorSets(const DescriptorSetAllocateInfo&) = 0;
		};

		//
		// The DescriptorSet handle is used to bind resources to shader stages.
		// Use descriptor sets to specify pipeline layout and apply to the pipeline.
		//
		class DescriptorSet_T {
		public:
			virtual void Write(uint32_t dstSet, uint32_t dstBinding, DescriptorType, Buffer) = 0;
			virtual void Write(uint32_t dstSet, uint32_t dstBinding, DescriptorType, ImageViewInfo) = 0;
		};

		//
		// The DescriptorSetLayout handle is a template for creating the descriptor set.
		// Combine multiple descriptor set layouts to create a pipeline layout.
		//
		class DescriptorSetLayout_T {
		public:
			virtual void Destroy() = 0;
		};

		//
		// The PipelineLayout handle is used to describe how resources are used in shader stages.
		// It must be binded to the Pipeline handle and compatible with the descriptor sets.
		//
		class PipelineLayout_T {
		public:
			virtual void Destroy() = 0;
		};

		//
		// The RenderPass handle is used to specify the specific information during the render pass.
		// Framebuffer and Pipeline handle can be binded with RenderPass.
		//
		class RenderPass_T {
		public:
			virtual void Destroy() = 0;
		};

		//
		// The ShaderModule handle is created from compiled binary source code.
		// It represents a specific pipeline stage in the pipeline.
		//
		class ShaderModule_T {
		public:
			virtual void Destroy() = 0;
		};
		
		//
		// The Pipeline handle specifies every detail in the pipeline.
		// It is used inside a render pass and is associated with a RenderPass handle.
		//
		class Pipeline_T {
		public:
			virtual void Destroy() = 0;
		};

		//
		// The Framebuffer handle specify color attachments and depth stencil attachment for a render pass.
		// It must be compatible with the RenderPass and the Pipeline.
		//
		class Framebuffer_T {
		public:
			virtual void Destroy() = 0;
		};

		//
		// The Fence handle is used for synchronization between CPU and GPU.
		// It has two states: signaled and unsignaled.
		//
		class Fence_T {
		public:
			virtual void Destroy() = 0;

			virtual void Reset() = 0;
			virtual void Wait() = 0;

			virtual bool GetCurrentStatus() const = 0;
		};

	}
}