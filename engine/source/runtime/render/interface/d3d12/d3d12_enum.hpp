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

	class D3D12EnumUsage final {
	public:
		D3D12EnumUsage(rhi::ImageUsage imageUsage) {
			switch (imageUsage) {
			case rhi::ImageUsage::ColorAttachment: usage = DXGI_USAGE_RENDER_TARGET_OUTPUT; break;
			case rhi::ImageUsage::DepthStencilAttachment: usage = DXGI_USAGE_RENDER_TARGET_OUTPUT; break;
			case rhi::ImageUsage::Sampled: usage = DXGI_USAGE_SHADER_INPUT; break;
			case rhi::ImageUsage::TransferSrc: usage = DXGI_USAGE_READ_ONLY; break;
			case rhi::ImageUsage::TransferDst: usage = DXGI_USAGE_UNORDERED_ACCESS; break;
			}
		}

		DXGI_USAGE Get() const { return usage; }

	private:
		DXGI_USAGE usage{ DXGI_USAGE_RENDER_TARGET_OUTPUT };
	};

	class D3D12EnumResourceStates final {
	public:
		D3D12EnumResourceStates(rhi::ImageLayout imageLayout) {
			switch (imageLayout) {
			case rhi::ImageLayout::Undefined: resourceStates = D3D12_RESOURCE_STATE_COMMON; break;
			case rhi::ImageLayout::ColorAttachment: resourceStates = D3D12_RESOURCE_STATE_RENDER_TARGET; break;
			case rhi::ImageLayout::DepthStencilAttachment: resourceStates = D3D12_RESOURCE_STATE_DEPTH_WRITE | D3D12_RESOURCE_STATE_DEPTH_READ; break;
			case rhi::ImageLayout::DepthStencilReadOnly: resourceStates = D3D12_RESOURCE_STATE_DEPTH_READ; break;
			case rhi::ImageLayout::ShaderReadOnly: resourceStates = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE; break;
			case rhi::ImageLayout::TransferSrc: resourceStates = D3D12_RESOURCE_STATE_COPY_SOURCE; break;
			case rhi::ImageLayout::TransferDst: resourceStates = D3D12_RESOURCE_STATE_COPY_DEST; break;
			case rhi::ImageLayout::Present: resourceStates = D3D12_RESOURCE_STATE_PRESENT; break;
			}
		}

		D3D12EnumResourceStates(rhi::BufferUsage bufferUsage) {
			switch (bufferUsage) {
			case rhi::BufferUsage::UniformBuffer:
			case rhi::BufferUsage::VertexBuffer:
			case rhi::BufferUsage::IndexBuffer: resourceStates = D3D12_RESOURCE_STATE_GENERIC_READ; break;
			case rhi::BufferUsage::StorageBuffer: resourceStates = D3D12_RESOURCE_STATE_UNORDERED_ACCESS; break;
			case rhi::BufferUsage::TransferSrc: resourceStates = D3D12_RESOURCE_STATE_COPY_SOURCE; break;
			case rhi::BufferUsage::TransferDst: resourceStates = D3D12_RESOURCE_STATE_COPY_DEST; break;
			}
		}

		D3D12_RESOURCE_STATES Get() const { return resourceStates; }

	private:
		D3D12_RESOURCE_STATES resourceStates{ D3D12_RESOURCE_STATE_COMMON };
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

	class D3D12EnumRTVDimension final {
	public:
		D3D12EnumRTVDimension(rhi::ImageViewType imageViewType) {
			switch (imageViewType) {
			case rhi::ImageViewType::Image1D: rtvDimension = D3D12_RTV_DIMENSION_TEXTURE1D; break;
			case rhi::ImageViewType::Image2D: rtvDimension = D3D12_RTV_DIMENSION_TEXTURE2D; break;
			case rhi::ImageViewType::Image3D: rtvDimension = D3D12_RTV_DIMENSION_TEXTURE3D; break;
			case rhi::ImageViewType::ImageArray1D: rtvDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY; break;
			case rhi::ImageViewType::ImageArray2D: rtvDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY; break;
			}
		}

		D3D12_RTV_DIMENSION Get() const { return rtvDimension; }

	private:
		D3D12_RTV_DIMENSION rtvDimension{ D3D12_RTV_DIMENSION_UNKNOWN };
	};

	class D3D12EnumDSVDimension final {
	public:
		D3D12EnumDSVDimension(rhi::ImageViewType imageViewType) {
			switch (imageViewType) {
			case rhi::ImageViewType::Image1D: dsvDimension = D3D12_DSV_DIMENSION_TEXTURE1D; break;
			case rhi::ImageViewType::Image2D: dsvDimension = D3D12_DSV_DIMENSION_TEXTURE2D; break;
			case rhi::ImageViewType::ImageArray1D: dsvDimension = D3D12_DSV_DIMENSION_TEXTURE1DARRAY; break;
			case rhi::ImageViewType::ImageArray2D: dsvDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY; break;
			}
		}

		D3D12_DSV_DIMENSION Get() const { return dsvDimension; }

	private:
		D3D12_DSV_DIMENSION dsvDimension{ D3D12_DSV_DIMENSION_UNKNOWN };
	};

	class D3D12EnumSRVDimension final {
	public:
		D3D12EnumSRVDimension(rhi::ImageViewType imageViewType) {
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

	class D3D12EnumDescriptorRangeType final {
	public:
		D3D12EnumDescriptorRangeType(rhi::DescriptorType descriptorType) {
			switch (descriptorType) {
			case rhi::DescriptorType::UniformBuffer: descriptorRangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; break;
			case rhi::DescriptorType::SampledImage: descriptorRangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; break;
			case rhi::DescriptorType::StorageImage:
			case rhi::DescriptorType::StorageBuffer: descriptorRangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV; break;
			case rhi::DescriptorType::Sampler: descriptorRangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER; break;
			}
		}

		D3D12_DESCRIPTOR_RANGE_TYPE Get() const { return descriptorRangeType; }

	private:
		D3D12_DESCRIPTOR_RANGE_TYPE descriptorRangeType{ D3D12_DESCRIPTOR_RANGE_TYPE_SRV };
	};

	class D3D12EnumFilter final {
	public:
		D3D12EnumFilter(rhi::Filter min, rhi::Filter mag, rhi::Filter mip, bool compare, bool anisotropy) {
			if (anisotropy) {
				if (compare) filter = D3D12_FILTER_COMPARISON_ANISOTROPIC; return;
				filter = D3D12_FILTER_ANISOTROPIC; return;
			}
			if (min == rhi::Filter::Nearest && mag == rhi::Filter::Nearest && mip == rhi::Filter::Nearest) {
				if (compare) filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT; return;
				filter = D3D12_FILTER_MIN_MAG_MIP_POINT; return;
			}
			if (min == rhi::Filter::Nearest && mag == rhi::Filter::Nearest && mip == rhi::Filter::Linear) {
				if (compare) filter = D3D12_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR; return;
				filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR; return;
			}
			if (min == rhi::Filter::Nearest && mag == rhi::Filter::Linear && mip == rhi::Filter::Nearest) {
				if (compare) filter = D3D12_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT; return;
				filter = D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT; return;
			}
			if (min == rhi::Filter::Nearest && mag == rhi::Filter::Linear && mip == rhi::Filter::Linear) {
				if (compare) filter = D3D12_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR; return;
				filter = D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR; return;
			}
			if (min == rhi::Filter::Linear && mag == rhi::Filter::Nearest && mip == rhi::Filter::Nearest) {
				if (compare) filter = D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT; return;
				filter = D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT; return;
			}
			if (min == rhi::Filter::Linear && mag == rhi::Filter::Nearest && mip == rhi::Filter::Linear) {
				if (compare) filter = D3D12_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR; return;
				filter = D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR; return;
			}
			if (min == rhi::Filter::Linear && mag == rhi::Filter::Linear && mip == rhi::Filter::Nearest) {
				if (compare) filter = D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT; return;
				filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT; return;
			}
			if (min == rhi::Filter::Linear && mag == rhi::Filter::Linear && mip == rhi::Filter::Linear) {
				if (compare) filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR; return;
				filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; return;
			}
		}

		D3D12_FILTER Get() const { return filter; }

	private:
		D3D12_FILTER filter{ D3D12_FILTER_MIN_MAG_MIP_POINT };
	};

	class D3D12EnumTextureAddressMode final {
	public:
		D3D12EnumTextureAddressMode(rhi::SamplerAddressMode addressMode) {
			switch (addressMode) {
			case rhi::SamplerAddressMode::Repeat: addressMode_ = D3D12_TEXTURE_ADDRESS_MODE_WRAP; break;
			case rhi::SamplerAddressMode::Mirror: addressMode_ = D3D12_TEXTURE_ADDRESS_MODE_MIRROR; break;
			case rhi::SamplerAddressMode::Clamp: addressMode_ = D3D12_TEXTURE_ADDRESS_MODE_CLAMP; break;
			case rhi::SamplerAddressMode::Border: addressMode_ = D3D12_TEXTURE_ADDRESS_MODE_BORDER; break;
			}
		}

		D3D12_TEXTURE_ADDRESS_MODE Get() const { return addressMode_; }

	private:
		D3D12_TEXTURE_ADDRESS_MODE addressMode_{ D3D12_TEXTURE_ADDRESS_MODE_WRAP };
	};

	class D3D12EnumStaticBorderColor final {
	public:
		D3D12EnumStaticBorderColor(rhi::BorderColor borderColor) {
			switch (borderColor) {
			case rhi::BorderColor::TransparentBlack: borderColor_ = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK; break;
			case rhi::BorderColor::OpaqueBlack: borderColor_ = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK; break;
			case rhi::BorderColor::OpaqueWhite: borderColor_ = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE; break;
			}
		}

		D3D12_STATIC_BORDER_COLOR Get() const { return borderColor_; }

	private:
		D3D12_STATIC_BORDER_COLOR borderColor_{ D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK };
	};

	class D3D12EnumRenderPassBeginningAccessType final {
	public:
		D3D12EnumRenderPassBeginningAccessType(rhi::AttachmentLoadOp attachmentLoadOp) {
			switch (attachmentLoadOp) {
			case rhi::AttachmentLoadOp::Load: acessType = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE; break;
			case rhi::AttachmentLoadOp::Clear: acessType = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR; break;
			case rhi::AttachmentLoadOp::DontCare: acessType = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS; break;
			}
		}

		D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE Get() const { return acessType; }

	private:
		D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE acessType{ D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD };
	};

	class D3D12EnumRenderPassEndingAccessType final {
	public:
		D3D12EnumRenderPassEndingAccessType(rhi::AttachmentStoreOp attachmentStoreOp) {
			switch (attachmentStoreOp) {
			case rhi::AttachmentStoreOp::Store: acessType = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE; break;
			case rhi::AttachmentStoreOp::DontCare: acessType = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS; break;
			}
		}

		D3D12_RENDER_PASS_ENDING_ACCESS_TYPE Get() const { return acessType; }

	private:
		D3D12_RENDER_PASS_ENDING_ACCESS_TYPE acessType{ D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD };
	};

	class D3D12EnumShaderVisibility final {
	public:
		D3D12EnumShaderVisibility(rhi::ShaderStage shaderStage) {
			switch (shaderStage) {
			case rhi::ShaderStage::All: shaderVisibility = D3D12_SHADER_VISIBILITY_ALL; break;
			case rhi::ShaderStage::Vertex: shaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; break;
			case rhi::ShaderStage::TessellationControl: shaderVisibility = D3D12_SHADER_VISIBILITY_HULL; break;
			case rhi::ShaderStage::TessellationEvaluation: shaderVisibility = D3D12_SHADER_VISIBILITY_DOMAIN; break;
			case rhi::ShaderStage::Geometry: shaderVisibility = D3D12_SHADER_VISIBILITY_GEOMETRY; break;
			case rhi::ShaderStage::Fragment: shaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; break;
			case rhi::ShaderStage::Compute: shaderVisibility = D3D12_SHADER_VISIBILITY_ALL; break;
			}
		}

		D3D12_SHADER_VISIBILITY Get() const { return shaderVisibility; }

	private:
		D3D12_SHADER_VISIBILITY shaderVisibility{ D3D12_SHADER_VISIBILITY_ALL };
	};

	class D3D12EnumInputClassification final {
	public:
		D3D12EnumInputClassification(rhi::VertexInputRate vertexInputRate) {
			switch (vertexInputRate) {
			case rhi::VertexInputRate::PerVertex: inputClassification = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA; break;
			case rhi::VertexInputRate::PerInstance: inputClassification = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA; break;
			}
		}

		D3D12_INPUT_CLASSIFICATION Get() const { return inputClassification; }

	private:
		D3D12_INPUT_CLASSIFICATION inputClassification{ D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA };
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
			case rhi::PrimitiveTopology::TriangleListAdj:
			case rhi::PrimitiveTopology::TriangleStripAdj: primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; break;
			case rhi::PrimitiveTopology::PatchList: primitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH; break;
			}
		}

		D3D12_PRIMITIVE_TOPOLOGY_TYPE Get() const { return primitiveTopologyType; }

	private:
		D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopologyType{ D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED };
	};

	class D3DEnumPrimitiveTopology final {
	public:
		D3DEnumPrimitiveTopology(rhi::PrimitiveTopology primitiveTopology) {
			switch (primitiveTopology) {
			case rhi::PrimitiveTopology::PointList: primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_POINTLIST; break;
			case rhi::PrimitiveTopology::LineList: primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_LINELIST; break;
			case rhi::PrimitiveTopology::LineStrip: primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
			case rhi::PrimitiveTopology::LineListAdj: primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ; break;
			case rhi::PrimitiveTopology::LineStripAdj: primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ; break;
			case rhi::PrimitiveTopology::TriangleList: primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
			case rhi::PrimitiveTopology::TriangleStrip: primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
			case rhi::PrimitiveTopology::TriangleListAdj: primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ; break;
			case rhi::PrimitiveTopology::TriangleStripAdj: primitiveTopology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ; break;
			}
		}

		D3D_PRIMITIVE_TOPOLOGY Get() const { return primitiveTopology_; }

	private:
		D3D_PRIMITIVE_TOPOLOGY primitiveTopology_{ D3D_PRIMITIVE_TOPOLOGY_UNDEFINED };
	};

	class D3DEnumPatchControlPoints final {
	public:
		D3DEnumPatchControlPoints(uint32_t patchControlPoints) {
			switch (patchControlPoints) {
			case 1: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST; break;
			case 2: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST; break;
			case 3: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST; break;
			case 4: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST; break;
			case 5: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST; break;
			case 6: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST; break;
			case 7: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST; break;
			case 8: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST; break;
			case 9: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST; break;
			case 10: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST; break;
			case 11: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST; break;
			case 12: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST; break;
			case 13: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST; break;
			case 14: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST; break;
			case 15: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST; break;
			case 16: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST; break;
			case 17: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST; break;
			case 18: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST; break;
			case 19: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST; break;
			case 20: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST; break;
			case 21: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST; break;
			case 22: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST; break;
			case 23: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST; break;
			case 24: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST; break;
			case 25: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST; break;
			case 26: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST; break;
			case 27: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST; break;
			case 28: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST; break;
			case 29: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST; break;
			case 30: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST; break;
			case 31: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST; break;
			case 32: primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST; break;
			}
		}

		D3D_PRIMITIVE_TOPOLOGY Get() const { return primitiveTopology; }

	private:
		D3D_PRIMITIVE_TOPOLOGY primitiveTopology{ D3D_PRIMITIVE_TOPOLOGY_UNDEFINED };
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

	class D3D12EnumIndexType final {
	public:
		D3D12EnumIndexType(rhi::IndexType indexType) {
			switch (indexType) {
			case rhi::IndexType::Uint16: format = DXGI_FORMAT_R16_UINT; break;
			case rhi::IndexType::Uint32: format = DXGI_FORMAT_R32_UINT; break;
			}
		}

		DXGI_FORMAT Get() const { return format; }

	private:
		DXGI_FORMAT format{ DXGI_FORMAT_UNKNOWN };
	};

}

#endif // _DIRECT3D12