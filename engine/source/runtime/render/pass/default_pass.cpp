#include "default_pass.hpp"

namespace engine {

	DefaultPass::DefaultPass(const DefaultPassCreateInfo& info)
		: device(info.device),
		  colorFormat(info.colorFormat),
		  renderExtent(info.renderExtent),
		  colorAttachments(info.colorAttachments),
		  vertexCount(info.vertexCount),
		  pVertexData(info.pVertexData),
		  shaderCompiler(info.shaderCompiler) {
		BuildBuffers();
		BuildDescriptorLayout();
		BuildDescriptorSets();
		BuildRenderPass();
		BuildPipeline();
		BuildFramebuffers();
	}

	DefaultPass::~DefaultPass() {
		vertexBuffer->Destroy();
		descriptorPool->Destroy();
		descriptorSetLayout->Destroy();
		pipelineLayout->Destroy();
		renderPass->Destroy();
		pipeline->Destroy();
		for (auto& framebuffer : framebuffers) framebuffer->Destroy();
	}

	void DefaultPass::BuildBuffers() {
		auto vertexBufferInfo = rhi::BufferCreateInfo()
			.SetSize(sizeof(DefaultVertexData) * vertexCount)
			.SetUsage(rhi::BufferUsage::VertexBuffer);

		vertexBuffer = device->CreateBuffer(vertexBufferInfo);

		auto ptr = reinterpret_cast<DefaultVertexData*>(vertexBuffer->Map());
		std::copy(pVertexData, pVertexData + vertexCount, ptr);
		vertexBuffer->Unmap();
	}

	void DefaultPass::BuildDescriptorLayout() {
		std::vector<rhi::DescriptorSetLayoutBinding> layoutBindings(1);

		layoutBindings[0] = rhi::DescriptorSetLayoutBinding()
			.SetBinding(0)
			.SetDescriptorCount(1)
			.SetDescriptorType(rhi::DescriptorType::UniformBuffer)
			.SetShaderStage(rhi::ShaderStage::Vertex);

		auto descriptorSetLayoutInfo = rhi::DescriptorSetLayoutCreateInfo()
			.SetBindings(layoutBindings);

		descriptorSetLayout = device->CreateDescriptorSetLayout(descriptorSetLayoutInfo);
	}

	void DefaultPass::BuildDescriptorSets() {
		std::vector<rhi::DescriptorPoolSize> poolSizes(1);

		poolSizes[0] = rhi::DescriptorPoolSize()
			.SetType(rhi::DescriptorType::UniformBuffer)
			.SetSize(1);

		auto descriptorPoolInfo = rhi::DescriptorPoolCreateInfo()
			.SetMaxSetCount(1)
			.SetPoolSizes(poolSizes);
		
		descriptorPool = device->CreateDescriptorPool(descriptorPoolInfo);

		auto descriptorSetAllocInfo = rhi::DescriptorSetAllocateInfo()
			.SetDescriptorSetCount(1)
			.SetPDescriptorSetLayouts(&descriptorSetLayout);

		descriptorSets = descriptorPool->AllocateDescriptorSets(descriptorSetAllocInfo);


	}

	void DefaultPass::BuildRenderPass() {
		auto colorAttachmentDescription = rhi::AttachmentDescription()
			.SetFormat(colorFormat)
			.SetSampleCount(rhi::SampleCount::Count1)
			.SetLoadOp(rhi::AttachmentLoadOp::Clear)
			.SetStoreOp(rhi::AttachmentStoreOp::Store)
			.SetInitialLayout(rhi::ImageLayout::Undefined)
			.SetPassLayout(rhi::ImageLayout::ColorAttachment)
			.SetFinalLayout(rhi::ImageLayout::Present);

		auto renderPassInfo = rhi::RenderPassCreateInfo()
			.SetPipelineType(rhi::PipelineType::Graphics)
			.SetColorAttachmentCount(1)
			.SetPColorAttachments(&colorAttachmentDescription);

		renderPass = device->CreateRenderPass(renderPassInfo);
	}

	void DefaultPass::BuildPipeline() {
		auto pipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
			.SetDescriptorSetLayoutCount(1)
			.SetPDescriptorSetLayouts(&descriptorSetLayout);

		pipelineLayout = device->CreatePipelineLayout(pipelineLayoutInfo);

		std::vector<uint8_t> shaderSource;

		shaderCompiler->Compile(StringToWString(Path("shader/hlsl/vertex_shader.hlsl").GetAbsolutePath()).c_str(), L"main", ShaderCompileArgument::vs_profile);
		shaderCompiler->GetSourceObject(shaderSource);

		auto shaderModuleInfo = rhi::ShaderModuleCreateInfo()
			.SetSourceCode(shaderSource)
			.SetPEntryPoint("main")
			.SetShaderStage(rhi::ShaderStage::Vertex);

		auto vertexShaderModule = device->CreateShaderModule(shaderModuleInfo);

		shaderCompiler->Compile(StringToWString(Path("shader/hlsl/fragment_shader.hlsl").GetAbsolutePath()).c_str(), L"main", ShaderCompileArgument::ps_profile);
		shaderCompiler->GetSourceObject(shaderSource);

		shaderModuleInfo = rhi::ShaderModuleCreateInfo()
			.SetSourceCode(shaderSource)
			.SetPEntryPoint("main")
			.SetShaderStage(rhi::ShaderStage::Fragment);

		auto fragmentShaderModule = device->CreateShaderModule(shaderModuleInfo);

		std::array<rhi::ShaderModule, 2> shaderModules = { vertexShaderModule, fragmentShaderModule };

		auto shaderStageInfo = rhi::PipelineShaderStageInfo()
			.SetShaderModuleCount(static_cast<uint32_t>(shaderModules.size())).SetPShaderModules(shaderModules.data());

		auto vertexBindingInfo = rhi::VertexBindingInfo()
			.SetInputRate(rhi::VertexInputRate::PerVertex)
			.SetStride(sizeof(DefaultVertexData));

		std::vector<rhi::VertexAttributeInfo> vertexAttributeInfos;
		vertexAttributeInfos.emplace_back(rhi::VertexAttributeInfo()
			.SetBinding(0)
			.SetFormat(rhi::Format::R32G32B32Sfloat)
			.SetSemantic("WorldPos")
			.SetOffset(0));
		vertexAttributeInfos.emplace_back(rhi::VertexAttributeInfo()
			.SetBinding(0)
			.SetFormat(rhi::Format::R32G32B32Sfloat)
			.SetSemantic("Color")
			.SetOffset(sizeof(math::Vector3f)));

		auto vertexInputInfo = rhi::PipelineVertexInputInfo()
			.SetBindingCount(1).SetPBindings(&vertexBindingInfo)
			.SetAttributes(vertexAttributeInfos);

		auto inputAssemblyInfo = rhi::PipelineInputAssemblyInfo()
			.SetTopology(rhi::PrimitiveTopology::TriangleList)
			.SetPrimitivieRestart(false);

		auto rasterizationInfo = rhi::PipelineRasterizationInfo()
			.SetSmoothLine(true)
			.SetPolygonMode(rhi::PolygonMode::Solid)
			.SetCullMode(rhi::CullMode::None)
			.SetDepthClamp(false)
			.SetDepthBias(0.0f)
			.SetSlopeScaledDepthBias(0.0f);

		auto scissor = rhi::Rect2D()
			.SetExtent(renderExtent)
			.SetOffset(rhi::Offset2D().SetX(0).SetY(0));

		auto viewport = rhi::Viewport()
			.SetWidth(static_cast<float>(renderExtent.width)).SetHeight(static_cast<float>(renderExtent.height))
			.SetX(0.0f).SetY(0.0f)
			.SetMinDepth(0.0f).SetMaxDepth(0.0f);

		auto viewportInfo = rhi::PipelineViewportInfo()
			.SetPScissors(&scissor).SetScissorCount(1)
			.SetPViewports(&viewport).SetViewportCount(1);

		auto depthStencilInfo = rhi::PipelineDepthStencilInfo()
			.SetDepthTestEnable(false)
			.SetDepthWriteEnable(false)
			.SetStencilTestEnable(false);

		auto colorAttachmentInfo = rhi::ColorBlendAttachmentInfo()
			.SetBlendEnable(false);

		auto colorBlendInfo = rhi::PipelineColorBlendInfo()
			.SetPAttachments(&colorAttachmentInfo).SetAttachmentCount(1);

		auto multisampleInfo = rhi::PipelineMultisampleInfo()
			.SetRasterizationSamples(rhi::SampleCount::Count1)
			.SetSampleShadingEnable(false)
			.SetAlphaToCoverageEnable(false);

		auto pipelineInfo = rhi::GraphicsPipelineCreateInfo()
			.SetShaderStageInfo(shaderStageInfo)
			.SetVertexInputInfo(vertexInputInfo)
			.SetInputAssemblyInfo(inputAssemblyInfo)
			.SetRasterizationInfo(rasterizationInfo)
			.SetViewportInfo(viewportInfo)
			.SetDepthStencilInfo(depthStencilInfo)
			.SetColorBlendInfo(colorBlendInfo)
			.SetMultisampleInfo(multisampleInfo)
			.SetPipelineLayout(pipelineLayout)
			.SetRenderPass(renderPass);

		pipeline = device->CreateGraphicsPipeline(pipelineInfo);

		vertexShaderModule->Destroy();
		fragmentShaderModule->Destroy();
	}

	void DefaultPass::BuildFramebuffers() {
		for (auto& colorAttachment : colorAttachments) {
			auto imageViewInfo = rhi::ImageViewInfo()
				.SetImage(colorAttachment)
				.SetFormat(colorFormat)
				.SetViewType(rhi::ImageViewType::Image2D)
				.SetBaseMipLevel(0)
				.SetMipLevelCount(1)
				.SetBaseArrayLayer(0)
				.SetArrayLayerCount(1);

			auto framebufferInfo = rhi::FramebufferCreateInfo()
				.SetWidth(renderExtent.width)
				.SetHeight(renderExtent.height)
				.SetLayers(1)
				.SetColorAttachmentCount(1)
				.SetPColorAttachments(&imageViewInfo)
				.SetRenderPass(renderPass);

			framebuffers.emplace_back(device->CreateFramebuffer(framebufferInfo));
		}
	}

	void DefaultPass::Draw(rhi::CommandBuffer commandBuffer, uint32_t imageIndex) const {
		auto clearColorValue = rhi::ClearColorValue().SetColorValue(std::array<float, 4>({ 0.0f, 0.0f, 0.0f, 0.0f }));

		auto renderPassBeginInfo = rhi::RenderPassBeginInfo()
			.SetRenderPass(renderPass)
			.SetFramebuffer(framebuffers[imageIndex])
			.SetPClearColorValues(&clearColorValue);
		commandBuffer->BeginRenderPass(renderPassBeginInfo);

		// Draw Call
		commandBuffer->BindPipeline(pipeline);
		commandBuffer->BindVertexBuffer(0, 1, &vertexBuffer);

		commandBuffer->Draw(3, 1, 0, 0);

		commandBuffer->EndRenderPass();
	}

}