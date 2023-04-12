#include "skybox_pass.hpp"

namespace engine {

	SkyboxPass::SkyboxPass(rhi::DescriptorSetLayout passDescriptorSetLayout, rhi::RenderPass renderPass) {
		BuildPipeline(passDescriptorSetLayout, renderPass);
	}

	SkyboxPass::~SkyboxPass() {
		if (pipelineLayout) pipelineLayout->Destroy();
		if (pipeline) pipeline->Destroy();
	}

	void SkyboxPass::BuildPipeline(rhi::DescriptorSetLayout passDescriptorSetLayout, rhi::RenderPass renderPass) {
		// Create pipeline layout

		auto pipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
			.SetDescriptorSetLayoutCount(1).SetPDescriptorSetLayouts(&passDescriptorSetLayout);

		pipelineLayout = RenderDevice::GetDevice()->CreatePipelineLayout(pipelineLayoutInfo);

		// Compile shaders

		std::vector<uint8_t> shaderSource;

		auto shaderCompiler = RenderDevice::GetInstance()->CreateShaderCompiler();

		shaderCompiler.Compile(Path::GetAbsolutePath("shader/skybox/hlsl/vertex.hlsl"), L"main", ShaderCompileArgument::vs_profile);
		shaderCompiler.GetSourceObject(shaderSource);

		auto shaderModuleInfo = rhi::ShaderModuleCreateInfo()
			.SetSourceCode(shaderSource)
			.SetPEntryPoint("main")
			.SetShaderStage(rhi::ShaderStage::Vertex);

		auto vertexShaderModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

		shaderCompiler.Compile(Path::GetAbsolutePath("shader/skybox/hlsl/fragment.hlsl"), L"main", ShaderCompileArgument::ps_profile);
		shaderCompiler.GetSourceObject(shaderSource);

		shaderModuleInfo = rhi::ShaderModuleCreateInfo()
			.SetSourceCode(shaderSource)
			.SetPEntryPoint("main")
			.SetShaderStage(rhi::ShaderStage::Fragment);

		auto fragmentShaderModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

		// Pipeline shader stages

		std::array<rhi::ShaderModule, 2> shaderModules = { vertexShaderModule, fragmentShaderModule };

		auto shaderStageInfo = rhi::PipelineShaderStageInfo()
			.SetShaderModuleCount(static_cast<uint32_t>(shaderModules.size())).SetPShaderModules(shaderModules.data());

		// Pipeline vertex input

		auto vertexBindingInfo = rhi::VertexBindingInfo()
			.SetInputRate(rhi::VertexInputRate::PerVertex)
			.SetStride(sizeof(ObjectVertexData));

		std::vector<rhi::VertexAttributeInfo> vertexAttributeInfos;

		vertexAttributeInfos.emplace_back(rhi::VertexAttributeInfo()
			.SetBinding(0)
			.SetFormat(rhi::Format::R32G32B32Sfloat)
			.SetSemantic("LocalPos")
			.SetOffset(0));

		auto vertexInputInfo = rhi::PipelineVertexInputInfo()
			.SetBindingCount(1).SetPBindings(&vertexBindingInfo)
			.SetAttributes(vertexAttributeInfos);

		// Pipeline input assembly

		auto inputAssemblyInfo = rhi::PipelineInputAssemblyInfo()
			.SetTopology(rhi::PrimitiveTopology::TriangleList)
			.SetPrimitivieRestart(false);

		// Pipeline rasterization

		auto rasterizationInfo = rhi::PipelineRasterizationInfo()
			.SetSmoothLine(true)
			.SetPolygonMode(rhi::PolygonMode::Solid)
			.SetCullMode(rhi::CullMode::None)
			.SetDepthClamp(false)
			.SetDepthBias(0.0f)
			.SetSlopeScaledDepthBias(0.0f);

		// Pipeline scissor & viewport

		auto scissor = rhi::Rect2D()
			.SetExtent(RenderDevice::GetSwapchain()->GetImageExtent())
			.SetOffset(rhi::Offset2D().SetX(0).SetY(0));

		auto viewport = rhi::Viewport()
			.SetWidth(static_cast<float>(RenderDevice::GetSwapchain()->GetImageExtent().width)).SetHeight(static_cast<float>(RenderDevice::GetSwapchain()->GetImageExtent().height))
			.SetX(0.0f).SetY(0.0f)
			.SetMinDepth(0.0f).SetMaxDepth(1.0f);

		auto viewportInfo = rhi::PipelineViewportInfo()
			.SetPScissors(&scissor).SetScissorCount(1)
			.SetPViewports(&viewport).SetViewportCount(1);

		// Pipeline depth & stencil

		auto depthStencilInfo = rhi::PipelineDepthStencilInfo()
			.SetDepthTestEnable(true)
			.SetDepthCompareOp(rhi::CompareOp::LessEqual)
			.SetDepthWriteEnable(false)
			.SetDepthBoundsTestEnable(false)
			.SetStencilTestEnable(false);

		// Pipeline color blend

		auto colorAttachmentInfo = rhi::ColorBlendAttachmentInfo()
			.SetBlendEnable(false);

		auto colorBlendInfo = rhi::PipelineColorBlendInfo()
			.SetPAttachments(&colorAttachmentInfo).SetAttachmentCount(1);

		// Pipeline multisample

		auto multisampleInfo = rhi::PipelineMultisampleInfo()
			.SetRasterizationSamples(rhi::SampleCount::Count1)
			.SetSampleShadingEnable(false)
			.SetAlphaToCoverageEnable(false);

		// Create pipeline

		auto pipelineInfo = rhi::GraphicsPipelineCreateInfo()
			.SetShaderStageInfo(shaderStageInfo)
			.SetVertexInputInfo(vertexInputInfo)
			.SetInputAssemblyInfo(inputAssemblyInfo)
			.SetRasterizationInfo(rasterizationInfo)
			.SetViewportInfo(viewportInfo)
			.SetDepthStencilInfo(depthStencilInfo)
			.SetColorBlendInfo(colorBlendInfo)
			.SetMultisampleInfo(multisampleInfo)
			.SetLayout(pipelineLayout)
			.SetRenderPass(renderPass);

		pipeline = RenderDevice::GetDevice()->CreateGraphicsPipeline(pipelineInfo);

		vertexShaderModule->Destroy();
		fragmentShaderModule->Destroy();
	}

}