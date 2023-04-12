#include "scene_pass.hpp"

namespace engine {

	ScenePass::ScenePass() {
		BuildDescriptorLayout();
		BuildRenderPass();
		BuildPipeline();
		BuildDepthImage();
		BuildFramebuffers();
	}

	ScenePass::~ScenePass() {
		for (auto layout : descriptorSetLayouts) layout->Destroy();
		if (pipelineLayout) pipelineLayout->Destroy();
		if (renderPass) renderPass->Destroy();
		if (pipeline) pipeline->Destroy();
		if (depthImage) depthImage->Destroy();
		for (auto& framebuffer : framebuffers) framebuffer->Destroy();
	}

	void ScenePass::BuildDescriptorLayout() {
		std::vector<rhi::DescriptorSetLayoutBindingInfo> objectLayoutBindings(1);
		objectLayoutBindings[0].SetBinding(0)
			                   .SetDescriptorType(rhi::DescriptorType::UniformBuffer)
			                   .SetShaderStage(rhi::ShaderStage::Vertex);

		std::vector<rhi::DescriptorSetLayoutBindingInfo> materialLayoutBindings(3);
		materialLayoutBindings[0].SetBinding(0)
			                     .SetDescriptorType(rhi::DescriptorType::UniformBuffer)
			                     .SetShaderStage(rhi::ShaderStage::Fragment);
		materialLayoutBindings[1].SetBinding(1)
			                     .SetDescriptorType(rhi::DescriptorType::SampledImage)
			                     .SetShaderStage(rhi::ShaderStage::Fragment);
		materialLayoutBindings[2].SetBinding(2)
			                     .SetDescriptorType(rhi::DescriptorType::Sampler)
			                     .SetShaderStage(rhi::ShaderStage::Fragment);

		std::vector<rhi::DescriptorSetLayoutBindingInfo> passLayoutBindings(3);
		passLayoutBindings[0].SetBinding(0)
			                 .SetDescriptorType(rhi::DescriptorType::UniformBuffer)
			                 .SetShaderStage(rhi::ShaderStage::All);
		passLayoutBindings[1].SetBinding(1)
			                 .SetDescriptorType(rhi::DescriptorType::SampledImage)
			                 .SetShaderStage(rhi::ShaderStage::Fragment);
		passLayoutBindings[2].SetBinding(2)
			                 .SetDescriptorType(rhi::DescriptorType::Sampler)
			                 .SetShaderStage(rhi::ShaderStage::Fragment);

		descriptorSetLayouts.resize(3);
		descriptorSetLayouts[0] = RenderDevice::GetDevice()->CreateDescriptorSetLayout(rhi::DescriptorSetLayoutCreateInfo().SetBindings(objectLayoutBindings));
		descriptorSetLayouts[1] = RenderDevice::GetDevice()->CreateDescriptorSetLayout(rhi::DescriptorSetLayoutCreateInfo().SetBindings(materialLayoutBindings));
		descriptorSetLayouts[2] = RenderDevice::GetDevice()->CreateDescriptorSetLayout(rhi::DescriptorSetLayoutCreateInfo().SetBindings(passLayoutBindings));
	}
	
	void ScenePass::BuildRenderPass() {
		auto colorAttachmentDescription = rhi::AttachmentDescription()
			.SetFormat(RenderDevice::GetSwapchainFormat())
			.SetSampleCount(rhi::SampleCount::Count1)
			.SetLoadOp(rhi::AttachmentLoadOp::Clear)
			.SetStoreOp(rhi::AttachmentStoreOp::Store)
			.SetInitialLayout(rhi::ImageLayout::Undefined)
			.SetPassLayout(rhi::ImageLayout::ColorAttachment)
			.SetFinalLayout(rhi::ImageLayout::Present);

		auto depthStencilDescription = rhi::AttachmentDescription()
			.SetFormat(rhi::Format::D32Sfloat)
			.SetSampleCount(rhi::SampleCount::Count1)
			.SetLoadOp(rhi::AttachmentLoadOp::Clear)
			.SetStoreOp(rhi::AttachmentStoreOp::Store)
			.SetInitialLayout(rhi::ImageLayout::Undefined)
			.SetPassLayout(rhi::ImageLayout::DepthStencilAttachment)
			.SetFinalLayout(rhi::ImageLayout::DepthStencilAttachment);

		auto renderPassInfo = rhi::RenderPassCreateInfo()
			.SetPipelineType(rhi::PipelineType::Graphics)
			.SetColorAttachmentCount(1)
			.SetPColorAttachments(&colorAttachmentDescription)
			.SetPDepthStencilAttachment(&depthStencilDescription);

		renderPass = RenderDevice::GetDevice()->CreateRenderPass(renderPassInfo);
	}

	void ScenePass::BuildPipeline() {

		// Create pipeline layout

		auto pipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
			.SetDescriptorSetLayouts(descriptorSetLayouts);

		pipelineLayout = RenderDevice::GetDevice()->CreatePipelineLayout(pipelineLayoutInfo);

		// Compile shaders

		std::vector<uint8_t> shaderSource;

		auto shaderCompiler = RenderDevice::GetInstance()->CreateShaderCompiler();

		shaderCompiler.Compile(Path::GetAbsolutePath("shader/scene/hlsl/vertex.hlsl"), L"main", ShaderCompileArgument::vs_profile);
		shaderCompiler.GetSourceObject(shaderSource);

		auto shaderModuleInfo = rhi::ShaderModuleCreateInfo()
			.SetSourceCode(shaderSource)
			.SetPEntryPoint("main")
			.SetShaderStage(rhi::ShaderStage::Vertex);

		auto vertexShaderModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

		shaderCompiler.Compile(Path::GetAbsolutePath("shader/scene/hlsl/fragment.hlsl"), L"main", ShaderCompileArgument::ps_profile);
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

		vertexAttributeInfos.emplace_back(rhi::VertexAttributeInfo()
			.SetBinding(0)
			.SetFormat(rhi::Format::R32G32B32Sfloat)
			.SetSemantic("Normal")
			.SetOffset(sizeof(math::Vector3f)));

		vertexAttributeInfos.emplace_back(rhi::VertexAttributeInfo()
			.SetBinding(0)
			.SetFormat(rhi::Format::R32G32Sfloat)
			.SetSemantic("TexCoord")
			.SetOffset(sizeof(math::Vector3f) * 2));

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
			.SetDepthWriteEnable(true)
			.SetDepthCompareOp(rhi::CompareOp::Less)
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

	void ScenePass::BuildDepthImage() {
		auto imageInfo = rhi::ImageCreateInfo()
			.SetFormat(rhi::Format::D32Sfloat)
			.SetImageType(rhi::ImageType::Image2D)
			.SetExtent(rhi::Extent3D().SetWidth(RenderDevice::GetSwapchain()->GetImageExtent().width)
				                      .SetHeight(RenderDevice::GetSwapchain()->GetImageExtent().height)
				                      .SetDepth(1))
			.SetArrayLayers(1)
			.SetMipLevels(1)
			.SetUsage(rhi::ImageUsage::DepthStencilAttachment);
		depthImage = RenderDevice::GetDevice()->CreateImage(imageInfo);
	}

	void ScenePass::BuildFramebuffers() {
		auto colorAttachments = RenderDevice::GetSwapchain()->GetImages();

		for (auto& colorAttachment : colorAttachments) {
			auto colorImageViewInfo = rhi::ImageViewInfo()
				.SetImage(colorAttachment)
				.SetFormat(RenderDevice::GetSwapchainFormat())
				.SetViewType(rhi::ImageViewType::Image2D)
				.SetSubresourceRange(rhi::ImageSubresourceRange()
					.SetImageAspect(rhi::ImageAspect::Color)
					.SetBaseMipLevel(0)
					.SetMipLevelCount(1)
					.SetBaseArrayLayer(0)
					.SetArrayLayerCount(1));

			auto depthImageViewInfo = rhi::ImageViewInfo()
				.SetImage(depthImage)
				.SetFormat(rhi::Format::D32Sfloat)
				.SetViewType(rhi::ImageViewType::Image2D)
				.SetSubresourceRange(rhi::ImageSubresourceRange()
					.SetImageAspect(rhi::ImageAspect::Depth)
					.SetBaseMipLevel(0)
					.SetMipLevelCount(1)
					.SetBaseArrayLayer(0)
					.SetArrayLayerCount(1));

			auto framebufferInfo = rhi::FramebufferCreateInfo()
				.SetWidth(RenderDevice::GetSwapchain()->GetImageExtent().width)
				.SetHeight(RenderDevice::GetSwapchain()->GetImageExtent().height)
				.SetLayers(1)
				.SetColorAttachmentCount(1)
				.SetPColorAttachments(&colorImageViewInfo)
				.SetPDepthStencilAttachments(&depthImageViewInfo)
				.SetRenderPass(renderPass);

			framebuffers.emplace_back(RenderDevice::GetDevice()->CreateFramebuffer(framebufferInfo));
		}
	}

	void ScenePass::Begin(uint32_t frame, rhi::CommandBuffer commandBuffer) {
		auto clearColorValue = rhi::ClearColorValue().SetColorValue(std::array<float, 4>({ 0.0f, 0.0f, 0.0f, 0.0f }));
		auto clearDepthValue = rhi::ClearDepthStencilValue().SetDepthValue(1.0f);

		auto renderPassBeginInfo = rhi::RenderPassBeginInfo()
			.SetRenderPass(renderPass)
			.SetFramebuffer(framebuffers[frame])
			.SetPClearColorValues(&clearColorValue)
			.SetClearDepthStencilValue(clearDepthValue);
		commandBuffer->BeginRenderPass(renderPassBeginInfo);

		commandBuffer->BindPipeline(pipeline);
	}

}