#pragma once

#include "d3d12_pipeline.hpp"

#ifdef _DIRECT3D12

namespace engine {

	class D3D12EnumFormat final {
	public:
		D3D12EnumFormat(rhi::Format format) {
			switch (format) {
			case rhi::Format::Undefined: format_ = DXGI_FORMAT_UNKNOWN; break;
			case rhi::Format::R8Unorm: format_ = DXGI_FORMAT_R8_UNORM; break;
			case rhi::Format::R8Snorm: format_ = DXGI_FORMAT_R8_SNORM; break;
			case rhi::Format::R8Uint: format_ = DXGI_FORMAT_R8_UINT; break;
			case rhi::Format::R8Sint: format_ = DXGI_FORMAT_R8_SINT; break;
			case rhi::Format::R8G8Unorm: format_ = DXGI_FORMAT_R8G8_UNORM; break;
			case rhi::Format::R8G8Snorm: format_ = DXGI_FORMAT_R8G8_SNORM; break;
			case rhi::Format::R8G8Uint: format_ = DXGI_FORMAT_R8G8_UINT; break;
			case rhi::Format::R8G8Sint: format_ = DXGI_FORMAT_R8G8_SINT; break;
			case rhi::Format::R8G8B8A8Unorm: format_ = DXGI_FORMAT_R8G8B8A8_UNORM; break;
			case rhi::Format::R8G8B8A8Snorm: format_ = DXGI_FORMAT_R8G8B8A8_SNORM; break;
			case rhi::Format::R8G8B8A8Uint: format_ = DXGI_FORMAT_R8G8B8A8_UINT; break;
			case rhi::Format::R8G8B8A8Sint: format_ = DXGI_FORMAT_R8G8B8A8_SINT; break;
			case rhi::Format::R8G8B8A8Srgb: format_ = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; break;
			case rhi::Format::B8G8R8A8Unorm: format_ = DXGI_FORMAT_B8G8R8A8_UNORM; break;
			case rhi::Format::B8G8R8A8Srgb: format_ = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; break;
			case rhi::Format::R16Unorm: format_ = DXGI_FORMAT_R16_UNORM; break;
			case rhi::Format::R16Snorm: format_ = DXGI_FORMAT_R16_SNORM; break;
			case rhi::Format::R16Uint: format_ = DXGI_FORMAT_R16_UINT; break;
			case rhi::Format::R16Sint: format_ = DXGI_FORMAT_R16_SINT; break;
			case rhi::Format::R16Sfloat: format_ = DXGI_FORMAT_R16_FLOAT; break;
			case rhi::Format::R16G16Unorm: format_ = DXGI_FORMAT_R16G16_UNORM; break;
			case rhi::Format::R16G16Snorm: format_ = DXGI_FORMAT_R16G16_SNORM; break;
			case rhi::Format::R16G16Uint: format_ = DXGI_FORMAT_R16G16_UINT; break;
			case rhi::Format::R16G16Sint: format_ = DXGI_FORMAT_R16G16_SINT; break;
			case rhi::Format::R16G16Sfloat: format_ = DXGI_FORMAT_R16G16_FLOAT; break;
			case rhi::Format::R16G16B16A16Unorm: format_ = DXGI_FORMAT_R16G16B16A16_UNORM; break;
			case rhi::Format::R16G16B16A16Snorm: format_ = DXGI_FORMAT_R16G16B16A16_SNORM; break;
			case rhi::Format::R16G16B16A16Uint: format_ = DXGI_FORMAT_R16G16B16A16_UINT; break;
			case rhi::Format::R16G16B16A16Sint: format_ = DXGI_FORMAT_R16G16B16A16_SINT; break;
			case rhi::Format::R16G16B16A16Sfloat: format_ = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
			case rhi::Format::R32Uint: format_ = DXGI_FORMAT_R32_UINT; break;
			case rhi::Format::R32Sint: format_ = DXGI_FORMAT_R32_SINT; break;
			case rhi::Format::R32Sfloat: format_ = DXGI_FORMAT_R32_FLOAT; break;
			case rhi::Format::R32G32Uint: format_ = DXGI_FORMAT_R32G32_UINT; break;
			case rhi::Format::R32G32Sint: format_ = DXGI_FORMAT_R32G32_SINT; break;
			case rhi::Format::R32G32Sfloat: format_ = DXGI_FORMAT_R32G32_FLOAT; break;
			case rhi::Format::R32G32B32Uint: format_ = DXGI_FORMAT_R32G32B32_UINT; break;
			case rhi::Format::R32G32B32Sint: format_ = DXGI_FORMAT_R32G32B32_SINT; break;
			case rhi::Format::R32G32B32Sfloat: format_ = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case rhi::Format::R32G32B32A32Uint: format_ = DXGI_FORMAT_R32G32B32A32_UINT; break;
			case rhi::Format::R32G32B32A32Sint: format_ = DXGI_FORMAT_R32G32B32A32_SINT; break;
			case rhi::Format::R32G32B32A32Sfloat: format_ = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			case rhi::Format::D16Unorm: format_ = DXGI_FORMAT_D16_UNORM; break;
			case rhi::Format::D32Sfloat: format_ = DXGI_FORMAT_D32_FLOAT; break;
			case rhi::Format::D24UnormS8Uint: format_ = DXGI_FORMAT_D24_UNORM_S8_UINT; break;
			case rhi::Format::D32SfloatS8Uint: format_ = DXGI_FORMAT_D32_FLOAT_S8X24_UINT; break;
			}
		}

		DXGI_FORMAT Get() { return format_; }

	private:
		DXGI_FORMAT format_{ DXGI_FORMAT_UNKNOWN };
	};

	class D3D12EnumCommandListType final {
	public:
		D3D12EnumCommandListType(rhi::QueueType queueType) {
			switch (queueType) {
			case rhi::QueueType::Graphics: commandListType = D3D12_COMMAND_LIST_TYPE_DIRECT; break;
			case rhi::QueueType::Compute: commandListType = D3D12_COMMAND_LIST_TYPE_COMPUTE; break;
			case rhi::QueueType::Transfer: commandListType = D3D12_COMMAND_LIST_TYPE_COPY; break;
			case rhi::QueueType::VideoDecode: commandListType = D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE; break;
			case rhi::QueueType::VideoEncode: commandListType = D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE; break;
			}
		}

		D3D12_COMMAND_LIST_TYPE Get() const { return commandListType; }

	private:
		D3D12_COMMAND_LIST_TYPE commandListType{ D3D12_COMMAND_LIST_TYPE_DIRECT };
	};

	class D3D12EnumAlphaMode final {
	public:
		D3D12EnumAlphaMode(rhi::AlphaMode alphaMode) {
			switch (alphaMode) {
			case rhi::AlphaMode::Undefined: alphaMode_ = DXGI_ALPHA_MODE_UNSPECIFIED; break;
			case rhi::AlphaMode::Opaque: alphaMode_ = DXGI_ALPHA_MODE_IGNORE; break;
			case rhi::AlphaMode::PreMultiplied: alphaMode_ = DXGI_ALPHA_MODE_PREMULTIPLIED; break;
			case rhi::AlphaMode::PostMultiplied: alphaMode_ = DXGI_ALPHA_MODE_STRAIGHT; break;
			}
		}

		DXGI_ALPHA_MODE Get() const { return alphaMode_; }

	private:
		DXGI_ALPHA_MODE alphaMode_{ DXGI_ALPHA_MODE_UNSPECIFIED };
	};

	class D3D12EnumSampleCount final {
	public:
		D3D12EnumSampleCount(rhi::SampleCount sampleCount) {
			switch (sampleCount) {
			case rhi::SampleCount::Count1: sampleCount_ = 1; break;
			case rhi::SampleCount::Count2: sampleCount_ = 2; break;
			case rhi::SampleCount::Count4: sampleCount_ = 4; break;
			case rhi::SampleCount::Count8: sampleCount_ = 8; break;
			case rhi::SampleCount::Count16: sampleCount_ = 16; break;
			case rhi::SampleCount::Count32: sampleCount_ = 32; break;
			case rhi::SampleCount::Count64: sampleCount_ = 64; break;
			}
		}

		UINT Get() const { return sampleCount_; }

	private:
		UINT sampleCount_{ 1 };
	};

	class D3D12EnumResourceState final {
	public:
		D3D12EnumResourceState(rhi::BufferUsage bufferUsage) {
			switch (bufferUsage) {
			case rhi::BufferUsage::UniformBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case rhi::BufferUsage::StorageBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case rhi::BufferUsage::UniformTexelBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case rhi::BufferUsage::StorageTexelBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case rhi::BufferUsage::VertexBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case rhi::BufferUsage::IndexBuffer: state = D3D12_RESOURCE_STATE_INDEX_BUFFER; break;
			case rhi::BufferUsage::TransferSrc: state = D3D12_RESOURCE_STATE_COPY_SOURCE; break;
			case rhi::BufferUsage::TransferDst: state = D3D12_RESOURCE_STATE_COPY_DEST; break;
			}
		}

		D3D12EnumResourceState(rhi::ImageUsage bufferUsage) {
			switch (bufferUsage) {
			case rhi::ImageUsage::ColorAttachment: state = D3D12_RESOURCE_STATE_RENDER_TARGET; break;
			case rhi::ImageUsage::DepthStencilAttachment: state = D3D12_RESOURCE_STATE_DEPTH_WRITE; break;
			case rhi::ImageUsage::Sampled: state = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE; break;
			case rhi::ImageUsage::TransferSrc: state = D3D12_RESOURCE_STATE_COPY_SOURCE; break;
			case rhi::ImageUsage::TransferDst: state = D3D12_RESOURCE_STATE_COPY_DEST; break;
			}
		}

		D3D12_RESOURCE_STATES Get() const { return state; }

	private:
		D3D12_RESOURCE_STATES state{ D3D12_RESOURCE_STATE_COMMON };
	};

	class D3D12EnumUsage final {
	public:
		D3D12EnumUsage(rhi::ImageUsage imageUsage) {
			switch (imageUsage) {
			case rhi::ImageUsage::ColorAttachment: imageUsage_ = DXGI_USAGE_RENDER_TARGET_OUTPUT; break;
			case rhi::ImageUsage::DepthStencilAttachment: imageUsage_ = DXGI_USAGE_RENDER_TARGET_OUTPUT; break;
			case rhi::ImageUsage::Sampled: imageUsage_ = DXGI_USAGE_SHADER_INPUT; break;
			case rhi::ImageUsage::TransferSrc: imageUsage_ = DXGI_USAGE_READ_ONLY; break;
			case rhi::ImageUsage::TransferDst: imageUsage_ = DXGI_USAGE_UNORDERED_ACCESS; break;
			}
		}

		DXGI_USAGE Get() const { return imageUsage_; }

	private:
		DXGI_USAGE imageUsage_{ DXGI_USAGE_RENDER_TARGET_OUTPUT };
	};

	class D3D12EnumImageType final {
	public:
		D3D12EnumImageType(rhi::ImageType imageType) {
			switch (imageType) {
			case rhi::ImageType::Image1D: imageType_ = D3D12_RESOURCE_DIMENSION_TEXTURE1D; break;
			case rhi::ImageType::Image2D: imageType_ = D3D12_RESOURCE_DIMENSION_TEXTURE2D; break;
			case rhi::ImageType::Image3D: imageType_ = D3D12_RESOURCE_DIMENSION_TEXTURE3D; break;
			}
		}

		D3D12_RESOURCE_DIMENSION Get() const { return imageType_; }

	private:
		D3D12_RESOURCE_DIMENSION imageType_{ D3D12_RESOURCE_DIMENSION_TEXTURE1D };
	};

	class D3D12EnumRTVType final {
	public:
		D3D12EnumRTVType(rhi::ImageViewType imageViewType) {
			switch (imageViewType) {
			case rhi::ImageViewType::Image1D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE1D; break;
			case rhi::ImageViewType::Image2D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE2D; break;
			case rhi::ImageViewType::Image3D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE3D; break;
			case rhi::ImageViewType::ImageArray1D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE1DARRAY; break;
			case rhi::ImageViewType::ImageArray2D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE2DARRAY; break;
			}
		}

		D3D12_RTV_DIMENSION Get() const { return imageViewType_; }

	private:
		D3D12_RTV_DIMENSION imageViewType_{ D3D12_RTV_DIMENSION_UNKNOWN };
	};

	class D3D12EnumSRVType final {
	public:
		D3D12EnumSRVType(rhi::ImageViewType imageViewType) {
			switch (imageViewType) {
			case rhi::ImageViewType::Image1D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE1D; break;
			case rhi::ImageViewType::Image2D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE2D; break;
			case rhi::ImageViewType::Image3D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE3D; break;
			case rhi::ImageViewType::ImageCube: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURECUBE; break;
			case rhi::ImageViewType::ImageArray1D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE1DARRAY; break;
			case rhi::ImageViewType::ImageArray2D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE2DARRAY; break;
			case rhi::ImageViewType::ImageArrayCube: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY; break;
			}
		}

		D3D12_SRV_DIMENSION Get() const { return imageViewType_; }

	private:
		D3D12_SRV_DIMENSION imageViewType_{ D3D12_SRV_DIMENSION_UNKNOWN };
	};

	class D3D12EnumHeapType final {
	public:
		D3D12EnumHeapType(rhi::DescriptorType descriptorType) {
			switch (descriptorType) {
			case rhi::DescriptorType::Sampler: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER; break;
			case rhi::DescriptorType::SampledImage: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case rhi::DescriptorType::StorageImage: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case rhi::DescriptorType::UniformBuffer: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case rhi::DescriptorType::StorageBuffer: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case rhi::DescriptorType::UniformTexelBuffer: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case rhi::DescriptorType::StorageTexelBuffer: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			}
		}

		D3D12_DESCRIPTOR_HEAP_TYPE Get() const { return heapType_; }

	private:
		D3D12_DESCRIPTOR_HEAP_TYPE heapType_{ D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER };
	};

	class D3D12EnumPrimitiveTopologyType final {
	public:
		D3D12EnumPrimitiveTopologyType(rhi::PrimitiveTopology primitiveTopology) {
			switch (primitiveTopology) {
			case rhi::PrimitiveTopology::PointList: primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT; break;
			case rhi::PrimitiveTopology::LineList:
			case rhi::PrimitiveTopology::LineStrip:
			case rhi::PrimitiveTopology::LineListAdj:
			case rhi::PrimitiveTopology::LineStripAdj: primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE; break;
			case rhi::PrimitiveTopology::TriangleList:
			case rhi::PrimitiveTopology::TriangleStrip:
			case rhi::PrimitiveTopology::TriangleFan:
			case rhi::PrimitiveTopology::TriangleListAdj:
			case rhi::PrimitiveTopology::TriangleStripAdj: primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; break;
			case rhi::PrimitiveTopology::PatchList: primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH; break;
			}
		}

		D3D12_PRIMITIVE_TOPOLOGY_TYPE Get() const { return primitiveTopologyType; }

	private:
		D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED };
	};

	class D3D12EnumCullMode final {
	public:
		D3D12EnumCullMode(rhi::CullMode cullMode) {
			switch (cullMode) {
			case rhi::CullMode::None: cullMode_ = D3D12_CULL_MODE_NONE; break;
			case rhi::CullMode::Front: cullMode_ = D3D12_CULL_MODE_FRONT; break;
			case rhi::CullMode::Back: cullMode_ = D3D12_CULL_MODE_BACK; break;
			}
		}

		D3D12_CULL_MODE Get() const { return cullMode_; }

	private:
		D3D12_CULL_MODE cullMode_{ D3D12_CULL_MODE_NONE };
	};

	class D3D12EnumFillMode final {
	public:
		D3D12EnumFillMode(rhi::PolygonMode polygonMode) {
			switch (polygonMode) {
			case rhi::PolygonMode::Wireframe: fillMode = D3D12_FILL_MODE_WIREFRAME; break;
			case rhi::PolygonMode::Solid: fillMode = D3D12_FILL_MODE_SOLID; break;
			}
		}

		D3D12_FILL_MODE Get() const { return fillMode; }

	private:
		D3D12_FILL_MODE fillMode{ D3D12_FILL_MODE_WIREFRAME };
	};

	class D3D12EnumFrontCounterClockwise final {
	public:
		D3D12EnumFrontCounterClockwise(rhi::FrontFace frontFace) {
			switch (frontFace) {
			case rhi::FrontFace::ClockWise: frontCounterClockwise = false; break;
			case rhi::FrontFace::CounterClockWise: frontCounterClockwise = true; break;
			}
		}

		bool Get() const { return frontCounterClockwise; }

	private:
		bool frontCounterClockwise{ false };
	};

	class D3D12EnumComparisonFunc final {
	public:
		D3D12EnumComparisonFunc(rhi::CompareOp compareOp) {
			switch (compareOp) {
			case rhi::CompareOp::Never: comparisonFunc = D3D12_COMPARISON_FUNC_NEVER; break;
			case rhi::CompareOp::Equal: comparisonFunc = D3D12_COMPARISON_FUNC_EQUAL; break;
			case rhi::CompareOp::NotEqual: comparisonFunc = D3D12_COMPARISON_FUNC_NOT_EQUAL; break;
			case rhi::CompareOp::Less: comparisonFunc = D3D12_COMPARISON_FUNC_LESS; break;
			case rhi::CompareOp::Greater: comparisonFunc = D3D12_COMPARISON_FUNC_GREATER; break;
			case rhi::CompareOp::LessEqual: comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; break;
			case rhi::CompareOp::GreaterEqual: comparisonFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL; break;
			case rhi::CompareOp::Always: comparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS; break;
			}
		}

		D3D12_COMPARISON_FUNC Get() const { return comparisonFunc; }

	private:
		D3D12_COMPARISON_FUNC comparisonFunc{ D3D12_COMPARISON_FUNC_NEVER };
	};

	class D3D12EnumStencilOp final {
	public:
		D3D12EnumStencilOp(rhi::StencilOp stencilOp) {
			switch (stencilOp) {
			case rhi::StencilOp::Keep: stencilOp_ = D3D12_STENCIL_OP_KEEP; break;
			case rhi::StencilOp::Zero: stencilOp_ = D3D12_STENCIL_OP_ZERO; break;
			case rhi::StencilOp::Replace: stencilOp_ = D3D12_STENCIL_OP_REPLACE; break;
			case rhi::StencilOp::Invert: stencilOp_ = D3D12_STENCIL_OP_INVERT; break;
			case rhi::StencilOp::IncrementClamp: stencilOp_ = D3D12_STENCIL_OP_INCR_SAT; break;
			case rhi::StencilOp::DecrementClamp: stencilOp_ = D3D12_STENCIL_OP_DECR_SAT; break;
			case rhi::StencilOp::IncrementWrap: stencilOp_ = D3D12_STENCIL_OP_INCR; break;
			case rhi::StencilOp::DecrementWrap: stencilOp_ = D3D12_STENCIL_OP_DECR; break;
			}
		}

		D3D12_STENCIL_OP Get() const { return stencilOp_; }

	private:
		D3D12_STENCIL_OP stencilOp_{ D3D12_STENCIL_OP_KEEP };
	};

	class D3D12EnumBlendOp final {
	public:
		D3D12EnumBlendOp(rhi::BlendOp blendOp) {
			switch (blendOp) {
			case rhi::BlendOp::Add: blendOp_ = D3D12_BLEND_OP_ADD; break;
			case rhi::BlendOp::Subtract: blendOp_ = D3D12_BLEND_OP_SUBTRACT; break;
			case rhi::BlendOp::RevSubtract: blendOp_ = D3D12_BLEND_OP_REV_SUBTRACT; break;
			case rhi::BlendOp::Min: blendOp_ = D3D12_BLEND_OP_MIN; break;
			case rhi::BlendOp::Max: blendOp_ = D3D12_BLEND_OP_MAX; break;
			}
		}

		D3D12_BLEND_OP Get() const { return blendOp_; }

	private:
		D3D12_BLEND_OP blendOp_{ D3D12_BLEND_OP_ADD };
	};

	class D3D12EnumBlend final {
	public:
		D3D12EnumBlend(rhi::BlendFactor blendFactor) {
			switch (blendFactor) {
			case rhi::BlendFactor::Zero: blend = D3D12_BLEND_ZERO; break;
			case rhi::BlendFactor::One: blend = D3D12_BLEND_ONE; break;
			case rhi::BlendFactor::SrcColor: blend = D3D12_BLEND_SRC_COLOR; break;
			case rhi::BlendFactor::DstColor: blend = D3D12_BLEND_DEST_COLOR; break;
			case rhi::BlendFactor::InvSrcColor: blend = D3D12_BLEND_INV_SRC_COLOR; break;
			case rhi::BlendFactor::InvDstColor: blend = D3D12_BLEND_INV_DEST_COLOR; break;
			case rhi::BlendFactor::SrcAlpha: blend = D3D12_BLEND_SRC_ALPHA; break;
			case rhi::BlendFactor::DstAlpha: blend = D3D12_BLEND_DEST_ALPHA; break;
			case rhi::BlendFactor::InvSrcAlpha: blend = D3D12_BLEND_INV_SRC_ALPHA; break;
			case rhi::BlendFactor::InvDstAlpha: blend = D3D12_BLEND_INV_DEST_ALPHA; break;
			case rhi::BlendFactor::ConstantColor: blend = D3D12_BLEND_BLEND_FACTOR; break;
			case rhi::BlendFactor::InvConstantColor: blend = D3D12_BLEND_INV_BLEND_FACTOR; break;
			case rhi::BlendFactor::ContantAlpha: blend = D3D12_BLEND_BLEND_FACTOR; break;
			case rhi::BlendFactor::InvContantAlpha: blend = D3D12_BLEND_INV_BLEND_FACTOR; break;
			case rhi::BlendFactor::SrcAlphaSaturate: blend = D3D12_BLEND_SRC_ALPHA_SAT; break;
			case rhi::BlendFactor::Src1Color: blend = D3D12_BLEND_SRC1_COLOR; break;
			case rhi::BlendFactor::InvSrc1Color: blend = D3D12_BLEND_INV_SRC1_COLOR; break;
			case rhi::BlendFactor::Src1Alpha: blend = D3D12_BLEND_SRC1_ALPHA; break;
			case rhi::BlendFactor::InvSrc1Alpha: blend = D3D12_BLEND_INV_SRC1_ALPHA; break;
			}
		}

		D3D12_BLEND Get() const { return blend; }

	private:
		D3D12_BLEND blend{ D3D12_BLEND_ZERO };
	};

	class D3D12EnumColorWriteEnable final {
	public:
		D3D12EnumColorWriteEnable(rhi::ColorComponent colorComponent) {
			switch (colorComponent) {
			case rhi::ColorComponent::R: colorWriteEnable = D3D12_COLOR_WRITE_ENABLE_RED; break;
			case rhi::ColorComponent::G: colorWriteEnable = D3D12_COLOR_WRITE_ENABLE_GREEN; break;
			case rhi::ColorComponent::B: colorWriteEnable = D3D12_COLOR_WRITE_ENABLE_BLUE; break;
			case rhi::ColorComponent::A: colorWriteEnable = D3D12_COLOR_WRITE_ENABLE_ALPHA; break;
			case rhi::ColorComponent::All: colorWriteEnable = D3D12_COLOR_WRITE_ENABLE_ALL; break;
			}
		}

		D3D12_COLOR_WRITE_ENABLE Get() const { return colorWriteEnable; }

	private:
		D3D12_COLOR_WRITE_ENABLE colorWriteEnable{ D3D12_COLOR_WRITE_ENABLE_ALL };
	};

}

#endif // _DIRECT3D12