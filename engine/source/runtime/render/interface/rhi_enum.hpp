#pragma once

namespace engine {
	namespace rhi {

		enum class Format {
			Undefined,
			R8Unorm,
			R8Snorm,
			R8Uint,
			R8Sint,
			R8G8Unorm,
			R8G8Snorm,
			R8G8Uint,
			R8G8Sint,
			R8G8B8A8Unorm,
			R8G8B8A8Snorm,
			R8G8B8A8Uint,
			R8G8B8A8Sint,
			R8G8B8A8Srgb,
			B8G8R8A8Unorm,
			B8G8R8A8Srgb,
			R16Unorm,
			R16Snorm,
			R16Uint,
			R16Sint,
			R16Sfloat,
			R16G16Unorm,
			R16G16Snorm,
			R16G16Uint,
			R16G16Sint,
			R16G16Sfloat,
			R16G16B16A16Unorm,
			R16G16B16A16Snorm,
			R16G16B16A16Uint,
			R16G16B16A16Sint,
			R16G16B16A16Sfloat,
			R32Uint,
			R32Sint,
			R32Sfloat,
			R32G32Uint,
			R32G32Sint,
			R32G32Sfloat,
			R32G32B32Uint,
			R32G32B32Sint,
			R32G32B32Sfloat,
			R32G32B32A32Uint,
			R32G32B32A32Sint,
			R32G32B32A32Sfloat,
			D16Unorm,
			D32Sfloat,
			D24UnormS8Uint,
			D32SfloatS8Uint
		};

		enum class QueueType {
			Graphics,
			Compute,
			Transfer,
			VideoDecode,
			VideoEncode
		};

		enum class AlphaMode {
			Undefined,
			Opaque,
			PreMultiplied,
			PostMultiplied
		};

		enum class SampleCount {
			Count1,
			Count2,
			Count4,
			Count8,
			Count16,
			Count32,
			Count64
		};

		enum class BufferUsage {
			ConstantBuffer,
			StorageBuffer,
			VertexBuffer,
			IndexBuffer,
			TransferSrc,
			TransferDst
		};

		enum class ImageUsage {
			ColorAttachment,
			DepthStencilAttachment,
			Sampled,
			TransferSrc,
			TransferDst
		};

		enum class ImageLayout {
			Undefined,
			ColorAttachment,
			DepthStencilAttachment,
			DepthStencilReadOnly,
			ShaderReadOnly,
			TransferSrc,
			TransferDst,
			Present
		};

		enum class ImageType {
			Image1D,
			Image2D,
			Image3D
		};

		enum class ImageViewType {
			Image1D,
			Image2D,
			Image3D,
			ImageCube,
			ImageArray1D,
			ImageArray2D,
			ImageArrayCube
		};

		enum class ComponentSwizzle {
			Identity,
			Zero,
			One,
			R,
			G,
			B,
			A
		};

		enum class DescriptorType {
			Sampler,
			SampledImage,
			StorageImage,
			ConstantBuffer,
			StorageBuffer
		};

		enum class AttachmentLoadOp {
			Load,
			Clear,
			DontCare
		};

		enum class AttachmentStoreOp {
			Store,
			DontCare
		};

		enum class ShaderStage {
			Vertex,
			TessellationControl,
			TessellationEvaluation,
			Geometry,
			Fragment
		};

		enum class PipelineType {
			Graphics,
			Compute,
			RayTracing
		};

		enum class VertexInputRate {
			PerVertex,
			PerInstance
		};

		enum class PrimitiveTopology {
			PointList,
			LineList,
			LineStrip,
			TriangleList,
			TriangleStrip,
			LineListAdj,
			LineStripAdj,
			TriangleListAdj,
			TriangleStripAdj,
			PatchList
		};

		enum class CullMode {
			None,
			Front,
			Back
		};

		enum class PolygonMode {
			Wireframe,
			Solid
		};

		enum class FrontFace {
			ClockWise,
			CounterClockWise
		};

		enum class CompareOp {
			Never,
			Equal,
			NotEqual,
			Less,
			Greater,
			LessEqual,
			GreaterEqual,
			Always
		};

		enum class StencilOp {
			Keep,
			Zero,
			Replace,
			Invert,
			IncrementClamp,
			DecrementClamp,
			IncrementWrap,
			DecrementWrap,
		};

		enum class BlendOp {
			Add,
			Subtract,
			RevSubtract,
			Min,
			Max
		};

		enum class BlendFactor {
			Zero,
			One,
			SrcColor,
			DstColor,
			InvSrcColor,
			InvDstColor,
			SrcAlpha,
			DstAlpha,
			InvSrcAlpha,
			InvDstAlpha,
			ConstantColor,
			InvConstantColor,
			ContantAlpha,
			InvContantAlpha,
			SrcAlphaSaturate,
			Src1Color,
			InvSrc1Color,
			Src1Alpha,
			InvSrc1Alpha
		};

		enum class ColorComponent {
			All,
			R,
			G,
			B,
			A
		};

		enum class IndexType {
			Uint16,
			Uint32
		};
	
	}
}