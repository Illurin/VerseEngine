#pragma once

#include "vulkan_util.hpp"

#include "render/interface/rhi.hpp"

#ifdef _VULKAN

namespace engine {
	
	class VkEnumFormat final {
	public:
		VkEnumFormat(rhi::Format format) {
			switch (format) {
			case rhi::Format::Undefined: format_ = vk::Format::eUndefined; break;
			case rhi::Format::R8Unorm: format_ = vk::Format::eR8Unorm; break;
			case rhi::Format::R8Snorm: format_ = vk::Format::eR8Snorm; break;
			case rhi::Format::R8Uint: format_ = vk::Format::eR8Uint; break;
			case rhi::Format::R8Sint: format_ = vk::Format::eR8Sint; break;
			case rhi::Format::R8G8Unorm: format_ = vk::Format::eR8G8Unorm; break;
			case rhi::Format::R8G8Snorm: format_ = vk::Format::eR8G8Snorm; break;
			case rhi::Format::R8G8Uint: format_ = vk::Format::eR8G8Uint; break;
			case rhi::Format::R8G8Sint: format_ = vk::Format::eR8G8Sint; break;
			case rhi::Format::R8G8B8A8Unorm: format_ = vk::Format::eR8G8B8A8Unorm; break;
			case rhi::Format::R8G8B8A8Snorm: format_ = vk::Format::eR8G8B8A8Snorm; break;
			case rhi::Format::R8G8B8A8Uint: format_ = vk::Format::eR8G8B8A8Uint; break;
			case rhi::Format::R8G8B8A8Sint: format_ = vk::Format::eR8G8B8A8Sint; break;
			case rhi::Format::R8G8B8A8Srgb: format_ = vk::Format::eR8G8B8A8Srgb; break;
			case rhi::Format::B8G8R8A8Unorm: format_ = vk::Format::eB8G8R8A8Unorm; break;
			case rhi::Format::B8G8R8A8Srgb: format_ = vk::Format::eB8G8R8A8Srgb; break;
			case rhi::Format::R16Unorm: format_ = vk::Format::eR16Unorm; break;
			case rhi::Format::R16Snorm: format_ = vk::Format::eR16Snorm; break;
			case rhi::Format::R16Uint: format_ = vk::Format::eR16Uint; break;
			case rhi::Format::R16Sint: format_ = vk::Format::eR16Sint; break;
			case rhi::Format::R16Sfloat: format_ = vk::Format::eR16Sfloat; break;
			case rhi::Format::R16G16Unorm: format_ = vk::Format::eR16G16Unorm; break;
			case rhi::Format::R16G16Snorm: format_ = vk::Format::eR16G16Snorm; break;
			case rhi::Format::R16G16Uint: format_ = vk::Format::eR16G16Uint; break;
			case rhi::Format::R16G16Sint: format_ = vk::Format::eR16G16Sint; break;
			case rhi::Format::R16G16Sfloat: format_ = vk::Format::eR16G16Sfloat; break;
			case rhi::Format::R16G16B16A16Unorm: format_ = vk::Format::eR16G16B16A16Unorm; break;
			case rhi::Format::R16G16B16A16Snorm: format_ = vk::Format::eR16G16B16A16Snorm; break;
			case rhi::Format::R16G16B16A16Uint: format_ = vk::Format::eR16G16B16A16Uint; break;
			case rhi::Format::R16G16B16A16Sint: format_ = vk::Format::eR16G16B16A16Sint; break;
			case rhi::Format::R16G16B16A16Sfloat: format_ = vk::Format::eR16G16B16A16Sfloat; break;
			case rhi::Format::R32Uint: format_ = vk::Format::eR32Uint; break;
			case rhi::Format::R32Sint: format_ = vk::Format::eR32Sint; break;
			case rhi::Format::R32Sfloat: format_ = vk::Format::eR32Sfloat; break;
			case rhi::Format::R32G32Uint: format_ = vk::Format::eR32G32Uint; break;
			case rhi::Format::R32G32Sint: format_ = vk::Format::eR32G32Sint; break;
			case rhi::Format::R32G32Sfloat: format_ = vk::Format::eR32G32Sfloat; break;
			case rhi::Format::R32G32B32Uint: format_ = vk::Format::eR32G32B32Uint; break;
			case rhi::Format::R32G32B32Sint: format_ = vk::Format::eR32G32B32Sint; break;
			case rhi::Format::R32G32B32Sfloat: format_ = vk::Format::eR32G32B32Sfloat; break;
			case rhi::Format::R32G32B32A32Uint: format_ = vk::Format::eR32G32B32A32Uint; break;
			case rhi::Format::R32G32B32A32Sint: format_ = vk::Format::eR32G32B32A32Sint; break;
			case rhi::Format::R32G32B32A32Sfloat: format_ = vk::Format::eR32G32B32A32Sfloat; break;
			case rhi::Format::D16Unorm: format_ = vk::Format::eD16Unorm; break;
			case rhi::Format::D32Sfloat: format_ = vk::Format::eD32Sfloat; break;
			case rhi::Format::S8Uint: format_ = vk::Format::eS8Uint; break;
			case rhi::Format::D16UnormS8Uint: format_ = vk::Format::eD16UnormS8Uint; break;
			case rhi::Format::D24UnormS8Uint: format_ = vk::Format::eD24UnormS8Uint; break;
			case rhi::Format::D32SfloatS8Uint: format_ = vk::Format::eD32SfloatS8Uint; break;
			}
		}

		vk::Format Get() const { return format_; }

	private:
		vk::Format format_{ 0 };
	};

	class VkEnumQueueType final {
	public:
		VkEnumQueueType(rhi::QueueType queueType) {
			switch (queueType) {
			case rhi::QueueType::Graphics: queueType_ = vk::QueueFlagBits::eGraphics; break;
			case rhi::QueueType::Compute: queueType_ = vk::QueueFlagBits::eCompute; break;
			case rhi::QueueType::Transfer: queueType_ = vk::QueueFlagBits::eTransfer; break;
			case rhi::QueueType::VideoDecode: queueType_ = vk::QueueFlagBits::eVideoDecodeKHR; break;
			case rhi::QueueType::VideoEncode: queueType_ = vk::QueueFlagBits::eVideoEncodeKHR; break;
			}
		}

		vk::QueueFlagBits Get() const { return queueType_; }

	private:
		vk::QueueFlagBits queueType_{ 0 };
	};

	class VkEnumAlphaMode final {
	public:
		VkEnumAlphaMode(rhi::AlphaMode alphaMode) {
			switch (alphaMode) {
			case rhi::AlphaMode::Undefined: alphaMode_ = vk::CompositeAlphaFlagBitsKHR::eInherit; break;
			case rhi::AlphaMode::Opaque: alphaMode_ = vk::CompositeAlphaFlagBitsKHR::eOpaque; break;
			case rhi::AlphaMode::PreMultiplied: alphaMode_ = vk::CompositeAlphaFlagBitsKHR::ePreMultiplied; break;
			case rhi::AlphaMode::PostMultiplied: alphaMode_ = vk::CompositeAlphaFlagBitsKHR::ePostMultiplied; break;
			}
		}

		vk::CompositeAlphaFlagBitsKHR Get() const { return alphaMode_; }

	private:
		vk::CompositeAlphaFlagBitsKHR alphaMode_{ 0 };
	};

	class VkEnumSampleCount final {
	public:
		VkEnumSampleCount(rhi::SampleCount sampleCount) {
			switch (sampleCount) {
			case rhi::SampleCount::Count1: sampleCount_ = vk::SampleCountFlagBits::e1; break;
			case rhi::SampleCount::Count2: sampleCount_ = vk::SampleCountFlagBits::e2; break;
			case rhi::SampleCount::Count4: sampleCount_ = vk::SampleCountFlagBits::e4; break;
			case rhi::SampleCount::Count8: sampleCount_ = vk::SampleCountFlagBits::e8; break;
			case rhi::SampleCount::Count16: sampleCount_ = vk::SampleCountFlagBits::e16; break;
			case rhi::SampleCount::Count32: sampleCount_ = vk::SampleCountFlagBits::e32; break;
			case rhi::SampleCount::Count64: sampleCount_ = vk::SampleCountFlagBits::e64; break;
			}
		}

		vk::SampleCountFlagBits Get() const { return sampleCount_; }

	private:
		vk::SampleCountFlagBits sampleCount_{ 0 };
	};

	class VkEnumBufferUsage final {
	public:
		VkEnumBufferUsage(rhi::BufferUsage bufferUsage) {
			switch (bufferUsage) {
			case rhi::BufferUsage::UniformBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eUniformBuffer; break;
			case rhi::BufferUsage::StorageBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eStorageBuffer; break;
			case rhi::BufferUsage::UniformTexelBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eUniformTexelBuffer; break;
			case rhi::BufferUsage::StorageTexelBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eStorageTexelBuffer; break;
			case rhi::BufferUsage::VertexBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eVertexBuffer; break;
			case rhi::BufferUsage::IndexBuffer: bufferUsage_ = vk::BufferUsageFlagBits::eIndexBuffer; break;
			case rhi::BufferUsage::TransferSrc: bufferUsage_ = vk::BufferUsageFlagBits::eTransferSrc; break;
			case rhi::BufferUsage::TransferDst: bufferUsage_ = vk::BufferUsageFlagBits::eTransferDst; break;
			}
		}

		vk::BufferUsageFlagBits Get() const { return bufferUsage_; }

	private:
		vk::BufferUsageFlagBits bufferUsage_{ 0 };
	};

	class VkEnumImageUsage final {
	public:
		VkEnumImageUsage(rhi::ImageUsage imageUsage) {
			switch (imageUsage) {
			case rhi::ImageUsage::ColorAttachment: imageUsage_ = vk::ImageUsageFlagBits::eColorAttachment; break;
			case rhi::ImageUsage::DepthStencilAttachment: imageUsage_ = vk::ImageUsageFlagBits::eDepthStencilAttachment; break;
			case rhi::ImageUsage::Sampled: imageUsage_ = vk::ImageUsageFlagBits::eSampled; break;
			case rhi::ImageUsage::TransferSrc: imageUsage_ = vk::ImageUsageFlagBits::eTransferSrc; break;
			case rhi::ImageUsage::TransferDst: imageUsage_ = vk::ImageUsageFlagBits::eTransferDst; break;
			}
		}

		vk::ImageUsageFlagBits Get() const { return imageUsage_; }

	private:
		vk::ImageUsageFlagBits imageUsage_{ 0 };
	};

	class VkEnumImageLayout final {
	public:
		VkEnumImageLayout(rhi::ImageLayout imageLayout) {
			switch (imageLayout) {
			case rhi::ImageLayout::Undefined: imageLayout_ = vk::ImageLayout::eUndefined; break;
			case rhi::ImageLayout::ColorAttachment: imageLayout_ = vk::ImageLayout::eColorAttachmentOptimal; break;
			case rhi::ImageLayout::DepthStencilAttachment: imageLayout_ = vk::ImageLayout::eDepthStencilAttachmentOptimal; break;
			case rhi::ImageLayout::ShaderRead: imageLayout_ = vk::ImageLayout::eShaderReadOnlyOptimal; break;
			case rhi::ImageLayout::TransferSrc: imageLayout_ = vk::ImageLayout::eTransferSrcOptimal; break;
			case rhi::ImageLayout::TransferDst: imageLayout_ = vk::ImageLayout::eTransferDstOptimal; break;
			case rhi::ImageLayout::SwapchainPresent: imageLayout_ = vk::ImageLayout::ePresentSrcKHR; break;
			}
		}

		vk::ImageLayout Get() const { return imageLayout_; }

	private:
		vk::ImageLayout imageLayout_{ 0 };
	};

	class VkEnumImageType final {
	public:
		VkEnumImageType(rhi::ImageType imageType) {
			switch (imageType) {
			case rhi::ImageType::Image1D: imageType_ = vk::ImageType::e1D; break;
			case rhi::ImageType::Image2D: imageType_ = vk::ImageType::e2D; break;
			case rhi::ImageType::Image3D: imageType_ = vk::ImageType::e3D; break;
			}
		}

		vk::ImageType Get() const { return imageType_; }

	private:
		vk::ImageType imageType_{ 0 };
	};

	class VkEnumImageViewType final {
	public:
		VkEnumImageViewType(rhi::ImageViewType imageViewType) {
			switch (imageViewType) {
			case rhi::ImageViewType::Image1D: imageViewType_ = vk::ImageViewType::e1D; break;
			case rhi::ImageViewType::Image2D: imageViewType_ = vk::ImageViewType::e2D; break;
			case rhi::ImageViewType::Image3D: imageViewType_ = vk::ImageViewType::e3D; break;
			case rhi::ImageViewType::ImageCube: imageViewType_ = vk::ImageViewType::eCube; break;
			case rhi::ImageViewType::ImageArray1D: imageViewType_ = vk::ImageViewType::e1DArray; break;
			case rhi::ImageViewType::ImageArray2D: imageViewType_ = vk::ImageViewType::e2DArray; break;
			case rhi::ImageViewType::ImageArrayCube: imageViewType_ = vk::ImageViewType::eCubeArray; break;
			}
		}
		
		vk::ImageViewType Get() const { return imageViewType_; }

	private:
		vk::ImageViewType imageViewType_{ 0 };
	};

	class VkEnumComponentSwizzle final {
	public:
		VkEnumComponentSwizzle(rhi::ComponentSwizzle componentSwizzle) {
			switch (componentSwizzle) {
			case rhi::ComponentSwizzle::Identity: componentSwizzle_ = vk::ComponentSwizzle::eIdentity; break;
			case rhi::ComponentSwizzle::Zero: componentSwizzle_ = vk::ComponentSwizzle::eZero; break;
			case rhi::ComponentSwizzle::One: componentSwizzle_ = vk::ComponentSwizzle::eOne; break;
			case rhi::ComponentSwizzle::R: componentSwizzle_ = vk::ComponentSwizzle::eR; break;
			case rhi::ComponentSwizzle::G: componentSwizzle_ = vk::ComponentSwizzle::eG; break;
			case rhi::ComponentSwizzle::B: componentSwizzle_ = vk::ComponentSwizzle::eB; break;
			case rhi::ComponentSwizzle::A: componentSwizzle_ = vk::ComponentSwizzle::eA; break;
			}
		}

		vk::ComponentSwizzle Get() const { return componentSwizzle_; }

	private:
		vk::ComponentSwizzle componentSwizzle_{ 0 };
	};

	class VkEnumDescriptorType final {
	public:
		VkEnumDescriptorType(rhi::DescriptorType descriptorType) {
			switch (descriptorType) {
			case rhi::DescriptorType::Sampler: descriptorType_ = vk::DescriptorType::eSampler; break;
			case rhi::DescriptorType::SampledImage: descriptorType_ = vk::DescriptorType::eSampledImage; break;
			case rhi::DescriptorType::StorageImage: descriptorType_ = vk::DescriptorType::eStorageImage; break;
			case rhi::DescriptorType::UniformBuffer: descriptorType_ = vk::DescriptorType::eUniformBuffer; break;
			case rhi::DescriptorType::StorageBuffer: descriptorType_ = vk::DescriptorType::eStorageBuffer; break;
			case rhi::DescriptorType::UniformTexelBuffer: descriptorType_ = vk::DescriptorType::eUniformTexelBuffer; break;
			case rhi::DescriptorType::StorageTexelBuffer: descriptorType_ = vk::DescriptorType::eStorageTexelBuffer; break;
			}
		}

		vk::DescriptorType Get() const { return descriptorType_; }

	private:
		vk::DescriptorType descriptorType_{ 0 };
	};

	class VkEnumAttachmentLoadOp final {
	public:
		VkEnumAttachmentLoadOp(rhi::AttachmentLoadOp attachmentLoadOp) {
			switch (attachmentLoadOp) {
			case rhi::AttachmentLoadOp::Clear: attachmentLoadOp_ = vk::AttachmentLoadOp::eClear; break;
			case rhi::AttachmentLoadOp::Load: attachmentLoadOp_ = vk::AttachmentLoadOp::eLoad; break;
			}
		}

		vk::AttachmentLoadOp Get() const { return attachmentLoadOp_; }

	private:
		vk::AttachmentLoadOp attachmentLoadOp_{ 0 };
	};

	class VkEnumAttachmentStoreOp final {
	public:
		VkEnumAttachmentStoreOp(rhi::AttachmentStoreOp attachmentStoreOp) {
			switch (attachmentStoreOp) {
			case rhi::AttachmentStoreOp::Store: attachmentStoreOp_ = vk::AttachmentStoreOp::eStore; break;
			}
		}

		vk::AttachmentStoreOp Get() const { return attachmentStoreOp_; }

	private:
		vk::AttachmentStoreOp attachmentStoreOp_{ 0 };
	};

	class VkEnumShaderStage final {
	public:
		VkEnumShaderStage(rhi::ShaderStage shaderStage) {
			switch (shaderStage) {
			case rhi::ShaderStage::Vertex: shaderStage_ = vk::ShaderStageFlagBits::eVertex; break;
			case rhi::ShaderStage::TessellationControl: shaderStage_ = vk::ShaderStageFlagBits::eTessellationControl; break;
			case rhi::ShaderStage::TessellationEvaluation: shaderStage_ = vk::ShaderStageFlagBits::eTessellationEvaluation; break;
			case rhi::ShaderStage::Geometry: shaderStage_ = vk::ShaderStageFlagBits::eGeometry; break;
			case rhi::ShaderStage::Fragment: shaderStage_ = vk::ShaderStageFlagBits::eFragment; break;
			}
		}

		vk::ShaderStageFlagBits Get() const { return shaderStage_; }

	private:
		vk::ShaderStageFlagBits shaderStage_{ 0 };
	};

	class VkEnumPipelineBindPoint final {
	public:
		VkEnumPipelineBindPoint(rhi::PipelineType pipelineType) {
			switch (pipelineType) {
			case rhi::PipelineType::Graphics: pipelineBindPoint = vk::PipelineBindPoint::eGraphics; break;
			case rhi::PipelineType::Compute: pipelineBindPoint = vk::PipelineBindPoint::eCompute; break;
			case rhi::PipelineType::RayTracing: pipelineBindPoint = vk::PipelineBindPoint::eRayTracingKHR; break;
			}
		}

		vk::PipelineBindPoint Get() const { return pipelineBindPoint; }

	private:
		vk::PipelineBindPoint pipelineBindPoint{ 0 };
	};

	class VkEnumVertexInputRate final {
	public:
		VkEnumVertexInputRate(rhi::VertexInputRate vertexInputRate) {
			switch (vertexInputRate) {
			case rhi::VertexInputRate::PerVertex: vertexInputRate_ = vk::VertexInputRate::eVertex; break;
			case rhi::VertexInputRate::PerInstance: vertexInputRate_ = vk::VertexInputRate::eInstance; break;
			}
		}

		vk::VertexInputRate Get() const { return vertexInputRate_; }

	private:
		vk::VertexInputRate vertexInputRate_{ 0 };
	};

	class VkEnumPrimitiveTopology final {
	public:
		VkEnumPrimitiveTopology(rhi::PrimitiveTopology primitiveTopology) {
			switch (primitiveTopology) {
			case rhi::PrimitiveTopology::PointList: primitiveTopology_ = vk::PrimitiveTopology::ePointList; break;
			case rhi::PrimitiveTopology::LineList: primitiveTopology_ = vk::PrimitiveTopology::eLineList; break;
			case rhi::PrimitiveTopology::LineStrip: primitiveTopology_ = vk::PrimitiveTopology::eLineStrip; break;
			case rhi::PrimitiveTopology::TriangleList: primitiveTopology_ = vk::PrimitiveTopology::eTriangleList; break;
			case rhi::PrimitiveTopology::TriangleStrip: primitiveTopology_ = vk::PrimitiveTopology::eTriangleStrip; break;
			case rhi::PrimitiveTopology::TriangleFan: primitiveTopology_ = vk::PrimitiveTopology::eTriangleFan; break;
			case rhi::PrimitiveTopology::LineListAdj: primitiveTopology_ = vk::PrimitiveTopology::eLineListWithAdjacency; break;
			case rhi::PrimitiveTopology::LineStripAdj: primitiveTopology_ = vk::PrimitiveTopology::eLineStripWithAdjacency; break;
			case rhi::PrimitiveTopology::TriangleListAdj: primitiveTopology_ = vk::PrimitiveTopology::eTriangleListWithAdjacency; break;
			case rhi::PrimitiveTopology::TriangleStripAdj: primitiveTopology_ = vk::PrimitiveTopology::eTriangleStripWithAdjacency; break;
			case rhi::PrimitiveTopology::PatchList: primitiveTopology_ = vk::PrimitiveTopology::ePatchList; break;
			}
		}

		vk::PrimitiveTopology Get() const { return primitiveTopology_; }

	private:
		vk::PrimitiveTopology primitiveTopology_{ 0 };
	};

	class VkEnumCullMode final {
	public:
		VkEnumCullMode(rhi::CullMode cullMode) {
			switch (cullMode) {
			case rhi::CullMode::None: cullMode_ = vk::CullModeFlagBits::eNone; break;
			case rhi::CullMode::Front: cullMode_ = vk::CullModeFlagBits::eFront; break;
			case rhi::CullMode::Back: cullMode_ = vk::CullModeFlagBits::eBack; break;
			case rhi::CullMode::Both: cullMode_ = vk::CullModeFlagBits::eFrontAndBack; break;
			}
		}

		vk::CullModeFlagBits Get() const { return cullMode_; }

	private:
		vk::CullModeFlagBits cullMode_{ 0 };
	};

	class VkEnumPolygonMode final {
	public:
		VkEnumPolygonMode(rhi::PolygonMode polygonMode) {
			switch (polygonMode) {
			case rhi::PolygonMode::Point: polygonMode_ = vk::PolygonMode::ePoint; break;
			case rhi::PolygonMode::Line: polygonMode_ = vk::PolygonMode::eLine; break;
			case rhi::PolygonMode::Fill: polygonMode_ = vk::PolygonMode::eFill; break;
			}
		}

		vk::PolygonMode Get() const { return polygonMode_; }

	private:
		vk::PolygonMode polygonMode_{ 0 };
	};

	class VkEnumFrontFace final {
	public:
		VkEnumFrontFace(rhi::FrontFace frontFace) {
			switch (frontFace) {
			case rhi::FrontFace::ClockWise: frontFace_ = vk::FrontFace::eClockwise; break;
			case rhi::FrontFace::CounterClockWise: frontFace_ = vk::FrontFace::eCounterClockwise; break;
			}
		}

		vk::FrontFace Get() const { return frontFace_; }

	private:
		vk::FrontFace frontFace_{ 0 };
	};

	class VkEnumCompareOp final {
	public:
		VkEnumCompareOp(rhi::CompareOp compareOp) {
			switch (compareOp) {
			case rhi::CompareOp::Never: compareOp_ = vk::CompareOp::eNever; break;
			case rhi::CompareOp::Equal: compareOp_ = vk::CompareOp::eEqual; break;
			case rhi::CompareOp::NotEqual: compareOp_ = vk::CompareOp::eNotEqual; break;
			case rhi::CompareOp::Less: compareOp_ = vk::CompareOp::eLess; break;
			case rhi::CompareOp::Greater: compareOp_ = vk::CompareOp::eGreater; break;
			case rhi::CompareOp::LessEqual: compareOp_ = vk::CompareOp::eLessOrEqual; break;
			case rhi::CompareOp::GreaterEqual: compareOp_ = vk::CompareOp::eGreaterOrEqual; break;
			case rhi::CompareOp::Always: compareOp_ = vk::CompareOp::eAlways; break;
			}
		}

		vk::CompareOp Get() const { return compareOp_; }

	private:
		vk::CompareOp compareOp_{ 0 };
	};

	class VkEnumStencilOp final {
	public:
		VkEnumStencilOp(rhi::StencilOp stencilOp) {
			switch (stencilOp) {
			case rhi::StencilOp::Keep: stencilOp_ = vk::StencilOp::eKeep; break;
			case rhi::StencilOp::Zero: stencilOp_ = vk::StencilOp::eZero; break;
			case rhi::StencilOp::Replace: stencilOp_ = vk::StencilOp::eReplace; break;
			case rhi::StencilOp::Invert: stencilOp_ = vk::StencilOp::eInvert; break;
			case rhi::StencilOp::IncrementClamp: stencilOp_ = vk::StencilOp::eIncrementAndClamp; break;
			case rhi::StencilOp::DecrementClamp: stencilOp_ = vk::StencilOp::eDecrementAndClamp; break;
			case rhi::StencilOp::IncrementWrap: stencilOp_ = vk::StencilOp::eIncrementAndWrap; break;
			case rhi::StencilOp::DecrementWrap: stencilOp_ = vk::StencilOp::eDecrementAndWrap; break;
			}
		}

		vk::StencilOp Get() const { return stencilOp_; }

	private:
		vk::StencilOp stencilOp_{ 0 };
	};

	class VkEnumBlendOp final {
	public:
		VkEnumBlendOp(rhi::BlendOp blendOp) {
			switch (blendOp) {
			case rhi::BlendOp::Add: blendOp_ = vk::BlendOp::eAdd; break;
			case rhi::BlendOp::Subtract: blendOp_ = vk::BlendOp::eSubtract; break;
			case rhi::BlendOp::RevSubtract: blendOp_ = vk::BlendOp::eReverseSubtract; break;
			case rhi::BlendOp::Min: blendOp_ = vk::BlendOp::eMin; break;
			case rhi::BlendOp::Max: blendOp_ = vk::BlendOp::eMax; break;
			}
		}

		vk::BlendOp Get() const { return blendOp_; }

	private:
		vk::BlendOp blendOp_{ 0 };
	};

	class VkEnumBlendFactor final {
	public:
		VkEnumBlendFactor(rhi::BlendFactor blendFactor) {
			switch (blendFactor) {
			case rhi::BlendFactor::Zero: blendFactor_ = vk::BlendFactor::eZero; break;
			case rhi::BlendFactor::One: blendFactor_ = vk::BlendFactor::eOne; break;
			case rhi::BlendFactor::SrcColor: blendFactor_ = vk::BlendFactor::eSrcColor; break;
			case rhi::BlendFactor::DstColor: blendFactor_ = vk::BlendFactor::eDstColor; break;
			case rhi::BlendFactor::InvSrcColor: blendFactor_ = vk::BlendFactor::eOneMinusSrcColor; break;
			case rhi::BlendFactor::InvDstColor: blendFactor_ = vk::BlendFactor::eOneMinusDstColor; break;
			case rhi::BlendFactor::SrcAlpha: blendFactor_ = vk::BlendFactor::eSrcAlpha; break;
			case rhi::BlendFactor::DstAlpha: blendFactor_ = vk::BlendFactor::eDstAlpha; break;
			case rhi::BlendFactor::InvSrcAlpha: blendFactor_ = vk::BlendFactor::eOneMinusSrcAlpha; break;
			case rhi::BlendFactor::InvDstAlpha: blendFactor_ = vk::BlendFactor::eOneMinusDstAlpha; break;
			case rhi::BlendFactor::ConstantColor: blendFactor_ = vk::BlendFactor::eConstantColor; break;
			case rhi::BlendFactor::InvConstantColor: blendFactor_ = vk::BlendFactor::eOneMinusConstantColor; break;
			case rhi::BlendFactor::ContantAlpha: blendFactor_ = vk::BlendFactor::eConstantAlpha; break;
			case rhi::BlendFactor::InvContantAlpha: blendFactor_ = vk::BlendFactor::eOneMinusConstantAlpha; break;
			case rhi::BlendFactor::SrcAlphaSaturate: blendFactor_ = vk::BlendFactor::eSrcAlphaSaturate; break;
			case rhi::BlendFactor::Src1Color: blendFactor_ = vk::BlendFactor::eSrc1Color; break;
			case rhi::BlendFactor::InvSrc1Color: blendFactor_ = vk::BlendFactor::eOneMinusSrc1Color; break;
			case rhi::BlendFactor::Src1Alpha: blendFactor_ = vk::BlendFactor::eSrc1Alpha; break;
			case rhi::BlendFactor::InvSrc1Alpha: blendFactor_ = vk::BlendFactor::eOneMinusSrc1Alpha; break;
			}
		}

		vk::BlendFactor Get() const { return blendFactor_; }

	private:
		vk::BlendFactor blendFactor_{ 0 };
	};

	class VkEnumColorComponent final {
	public:
		VkEnumColorComponent(rhi::ColorComponent colorComponent) {
			switch (colorComponent) {
			case rhi::ColorComponent::R: colorComponent_ = vk::ColorComponentFlagBits::eR; break;
			case rhi::ColorComponent::G: colorComponent_ = vk::ColorComponentFlagBits::eG; break;
			case rhi::ColorComponent::B: colorComponent_ = vk::ColorComponentFlagBits::eB; break;
			case rhi::ColorComponent::A: colorComponent_ = vk::ColorComponentFlagBits::eA; break;
			case rhi::ColorComponent::All: colorComponent_ = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA; break;
			}
		}

		vk::ColorComponentFlags Get() const { return colorComponent_; }

	private:
		vk::ColorComponentFlags colorComponent_{ 0 };
	};

}

#endif // _VULKAN