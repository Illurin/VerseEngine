#include "scene_pass.hpp"

namespace engine {

	void ScenePass::Init(const ScenePassInfo& info) {
		this->info = info;
		BuildBuffers();
		BuildSamplers();
		BuildImages();
		BuildDescriptorLayout();
		BuildDescriptorSets();
		BuildRenderPass();
		BuildPipeline();
		BuildFramebuffers();
	}

	void ScenePass::Destroy() {
		vertexBuffer->Destroy();
		passUniformBuffer->Destroy();
		if (uploaderBuffer) uploaderBuffer->Destroy();
		textureImage->Destroy();
		sampler->Destroy();
		descriptorPool->Destroy();
		descriptorSetLayout->Destroy();
		pipelineLayout->Destroy();
		renderPass->Destroy();
		pipeline->Destroy();
		for (auto& framebuffer : framebuffers) framebuffer->Destroy();
	}

	void ScenePass::BuildBuffers() {

		// Build vertex buffer

		std::array<TexQuadVertexData, 3> vertices;
		vertices[0].position = math::Vector2f(0.0f, 0.5f);
		vertices[0].texCoord = math::Vector2f(0.5f, 1.0f);
		vertices[1].position = math::Vector2f(-0.5f, -0.5f);
		vertices[1].texCoord = math::Vector2f(0.0f, 0.0f);
		vertices[2].position = math::Vector2f(0.5f, -0.5f);
		vertices[2].texCoord = math::Vector2f(1.0f, 0.0f);

		auto vertexBufferInfo = rhi::BufferCreateInfo()
			.SetSize(sizeof(TexQuadVertexData) * 3)
			.SetUsage(rhi::BufferUsage::VertexBuffer);

		vertexBuffer = info.device->CreateBuffer(vertexBufferInfo);

		auto vertexBufferPtr = reinterpret_cast<TexQuadVertexData*>(vertexBuffer->Map());
		std::copy(vertices.begin(), vertices.end(), vertexBufferPtr);
		vertexBuffer->Unmap();

		// Build uniform buffer

		math::Affine3f transformation(math::AngleAxisf(math::Pi<float>() * 0.2f, math::Vector3f::UnitZ()));

		auto passUniformBufferInfo = rhi::BufferCreateInfo()
			.SetSize(sizeof(math::Matrix4f))
			.SetUsage(rhi::BufferUsage::UniformBuffer);

		passUniformBuffer = info.device->CreateBuffer(passUniformBufferInfo);

		auto passUniformBufferPtr = reinterpret_cast<math::Matrix4f*>(passUniformBuffer->Map());
		*passUniformBufferPtr = transformation.matrix();
		passUniformBuffer->Unmap();
	}

	void ScenePass::BuildSamplers() {
		auto samplerInfo = rhi::SamplerCreateInfo()
			.SetMinFilter(rhi::Filter::Linear)
			.SetMagFilter(rhi::Filter::Linear)
			.SetMipFilter(rhi::Filter::Linear)
			.SetAddressModeU(rhi::SamplerAddressMode::Repeat)
			.SetAddressModeV(rhi::SamplerAddressMode::Repeat)
			.SetAddressModeW(rhi::SamplerAddressMode::Repeat)
			.SetMinLod(0.0f)
			.SetMaxLod(1.0f);

		sampler = info.device->CreateSampler(samplerInfo);
	}

	void ScenePass::BuildImages() {
		auto rawImage = asset::ImageLoader::Load(Path("asset/brickTexture.jpg").GetAbsolutePath().c_str(), 32);

		auto imageInfo = rhi::ImageCreateInfo()
			.SetFormat(rhi::Format::R8G8B8A8Unorm)
			.SetImageType(rhi::ImageType::Image2D)
			.SetExtent(rhi::Extent3D().SetWidth(rawImage.width).SetHeight(rawImage.height).SetDepth(1))
			.SetArrayLayers(1)
			.SetMipLevels(1)
			.SetUsage(rhi::ImageUsage::SampledAndTransferDst);

		textureImage = info.device->CreateImage(imageInfo);

		auto footprint = textureImage->GetCopyableFootprint();

		auto bufferInfo = rhi::BufferCreateInfo()
			.SetSize(footprint.size)
			.SetUsage(rhi::BufferUsage::TransferSrc);

		uploaderBuffer = info.device->CreateBuffer(bufferInfo);

		auto uploaderBufferPtr = reinterpret_cast<uint8_t*>(uploaderBuffer->Map());
		if (footprint.rowPitch == 0) {
			std::copy(rawImage.source.begin(), rawImage.source.end(), uploaderBufferPtr);
		}
		else {
			for (uint32_t i = 0; i < rawImage.height; i++) {
				auto rowBegin = rawImage.source.begin() + i * rawImage.rowPitch;
				std::copy(rowBegin, rowBegin + rawImage.rowPitch, uploaderBufferPtr + i * footprint.rowPitch);
			}
		}
		uploaderBuffer->Unmap();
	}

	void ScenePass::BuildDescriptorLayout() {
		std::vector<rhi::DescriptorSetLayoutBindingInfo> layoutBindings(3);

		layoutBindings[0] = rhi::DescriptorSetLayoutBindingInfo()
			.SetBinding(0)
			.SetDescriptorType(rhi::DescriptorType::UniformBuffer)
			.SetShaderStage(rhi::ShaderStage::Vertex);

		layoutBindings[1] = rhi::DescriptorSetLayoutBindingInfo()
			.SetBinding(1)
			.SetDescriptorType(rhi::DescriptorType::SampledImage)
			.SetShaderStage(rhi::ShaderStage::Fragment);

		layoutBindings[2] = rhi::DescriptorSetLayoutBindingInfo()
			.SetBinding(2)
			.SetDescriptorType(rhi::DescriptorType::Sampler)
			.SetShaderStage(rhi::ShaderStage::Fragment)
			.SetPStaticSampler(&sampler);

		auto descriptorSetLayoutInfo = rhi::DescriptorSetLayoutCreateInfo()
			.SetBindings(layoutBindings);

		descriptorSetLayout = info.device->CreateDescriptorSetLayout(descriptorSetLayoutInfo);
	}

	void ScenePass::BuildDescriptorSets() {
		std::vector<rhi::DescriptorPoolSize> poolSizes(2);

		poolSizes[0] = rhi::DescriptorPoolSize()
			.SetType(rhi::DescriptorType::UniformBuffer)
			.SetSize(1);

		poolSizes[1] = rhi::DescriptorPoolSize()
			.SetType(rhi::DescriptorType::SampledImage)
			.SetSize(1);

		auto descriptorPoolInfo = rhi::DescriptorPoolCreateInfo()
			.SetMaxSetCount(1)
			.SetPoolSizes(poolSizes);
		
		descriptorPool = info.device->CreateDescriptorPool(descriptorPoolInfo);

		auto descriptorSetAllocInfo = rhi::DescriptorSetAllocateInfo()
			.SetDescriptorSetCount(1)
			.SetPDescriptorSetLayouts(&descriptorSetLayout);

		descriptorSets = descriptorPool->AllocateDescriptorSets(descriptorSetAllocInfo);

		// math::Matrix4f

		descriptorSets[0]->Write(0, rhi::DescriptorType::UniformBuffer, passUniformBuffer);

		// texture image

		auto imageViewInfo = rhi::ImageViewInfo()
			.SetImage(textureImage)
			.SetFormat(rhi::Format::R8G8B8A8Unorm)
			.SetViewType(rhi::ImageViewType::Image2D)
			.SetSubresourceRange(rhi::ImageSubresourceRange()
				.SetImageAspect(rhi::ImageAspect::Color)
				.SetBaseMipLevel(0)
				.SetMipLevelCount(1)
				.SetBaseArrayLayer(0)
				.SetArrayLayerCount(1));

		descriptorSets[0]->Write(1, rhi::DescriptorType::SampledImage, imageViewInfo);
	}

	void ScenePass::BuildRenderPass() {
		auto colorAttachmentDescription = rhi::AttachmentDescription()
			.SetFormat(info.colorFormat)
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

		renderPass = info.device->CreateRenderPass(renderPassInfo);
	}

	void ScenePass::BuildPipeline() {

		// Create pipeline layout

		auto pipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
			.SetDescriptorSetLayoutCount(1)
			.SetPDescriptorSetLayouts(&descriptorSetLayout);

		pipelineLayout = info.device->CreatePipelineLayout(pipelineLayoutInfo);

		// Compile shaders

		std::vector<uint8_t> shaderSource;

		info.shaderCompiler->Compile(StringToWString(Path("shader/debug/hlsl/vertex.hlsl").GetAbsolutePath()).c_str(), L"main", ShaderCompileArgument::vs_profile);
		info.shaderCompiler->GetSourceObject(shaderSource);

		auto shaderModuleInfo = rhi::ShaderModuleCreateInfo()
			.SetSourceCode(shaderSource)
			.SetPEntryPoint("main")
			.SetShaderStage(rhi::ShaderStage::Vertex);

		auto vertexShaderModule = info.device->CreateShaderModule(shaderModuleInfo);

		info.shaderCompiler->Compile(StringToWString(Path("shader/debug/hlsl/fragment.hlsl").GetAbsolutePath()).c_str(), L"main", ShaderCompileArgument::ps_profile);
		info.shaderCompiler->GetSourceObject(shaderSource);

		shaderModuleInfo = rhi::ShaderModuleCreateInfo()
			.SetSourceCode(shaderSource)
			.SetPEntryPoint("main")
			.SetShaderStage(rhi::ShaderStage::Fragment);

		auto fragmentShaderModule = info.device->CreateShaderModule(shaderModuleInfo);

		// Pipeline shader stages

		std::array<rhi::ShaderModule, 2> shaderModules = { vertexShaderModule, fragmentShaderModule };

		auto shaderStageInfo = rhi::PipelineShaderStageInfo()
			.SetShaderModuleCount(static_cast<uint32_t>(shaderModules.size())).SetPShaderModules(shaderModules.data());

		// Pipeline vertex input

		auto vertexBindingInfo = rhi::VertexBindingInfo()
			.SetInputRate(rhi::VertexInputRate::PerVertex)
			.SetStride(sizeof(TexQuadVertexData));

		std::vector<rhi::VertexAttributeInfo> vertexAttributeInfos;

		vertexAttributeInfos.emplace_back(rhi::VertexAttributeInfo()
			.SetBinding(0)
			.SetFormat(rhi::Format::R32G32Sfloat)
			.SetSemantic("Position")
			.SetOffset(0));

		vertexAttributeInfos.emplace_back(rhi::VertexAttributeInfo()
			.SetBinding(0)
			.SetFormat(rhi::Format::R32G32Sfloat)
			.SetSemantic("TexCoord")
			.SetOffset(sizeof(math::Vector2f)));

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
			.SetExtent(info.renderExtent)
			.SetOffset(rhi::Offset2D().SetX(0).SetY(0));

		auto viewport = rhi::Viewport()
			.SetWidth(static_cast<float>(info.renderExtent.width)).SetHeight(static_cast<float>(info.renderExtent.height))
			.SetX(0.0f).SetY(0.0f)
			.SetMinDepth(0.0f).SetMaxDepth(0.0f);

		auto viewportInfo = rhi::PipelineViewportInfo()
			.SetPScissors(&scissor).SetScissorCount(1)
			.SetPViewports(&viewport).SetViewportCount(1);

		// Pipeline depth & stencil

		auto depthStencilInfo = rhi::PipelineDepthStencilInfo()
			.SetDepthTestEnable(false)
			.SetDepthWriteEnable(false)
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
			.SetPipelineLayout(pipelineLayout)
			.SetRenderPass(renderPass);

		pipeline = info.device->CreateGraphicsPipeline(pipelineInfo);

		vertexShaderModule->Destroy();
		fragmentShaderModule->Destroy();
	}

	void ScenePass::BuildFramebuffers() {
		for (auto& colorAttachment : info.colorAttachments) {
			auto imageViewInfo = rhi::ImageViewInfo()
				.SetImage(colorAttachment)
				.SetFormat(info.colorFormat)
				.SetViewType(rhi::ImageViewType::Image2D)
				.SetSubresourceRange(rhi::ImageSubresourceRange()
					.SetImageAspect(rhi::ImageAspect::Color)
					.SetBaseMipLevel(0)
					.SetMipLevelCount(1)
					.SetBaseArrayLayer(0)
					.SetArrayLayerCount(1));

			auto framebufferInfo = rhi::FramebufferCreateInfo()
				.SetWidth(info.renderExtent.width)
				.SetHeight(info.renderExtent.height)
				.SetLayers(1)
				.SetColorAttachmentCount(1)
				.SetPColorAttachments(&imageViewInfo)
				.SetRenderPass(renderPass);

			framebuffers.emplace_back(info.device->CreateFramebuffer(framebufferInfo));
		}
	}

	void ScenePass::PreparePassData(rhi::CommandBuffer commandBuffer) {
		if (passDataPrepared) {
			if (uploaderBuffer) {
				uploaderBuffer->Destroy();
				uploaderBuffer = nullptr;
			}
			return;
		}

		auto imageMemoryBarrier = rhi::ImageMemoryBarrierInfo()
			.SetImage(textureImage)
			.SetOldLayout(rhi::ImageLayout::Undefined)
			.SetNewLayout(rhi::ImageLayout::TransferDst)
			.SetSubresourceRange(rhi::ImageSubresourceRange()
				.SetImageAspect(rhi::ImageAspect::Color)
				.SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(1).SetMipLevelCount(1));

		commandBuffer->ResourceBarrier(imageMemoryBarrier);

		auto bufferRegion = rhi::BufferCopyRegion()
			.SetBuffer(uploaderBuffer);

		auto imageRegion = rhi::ImageCopyRegion()
			.SetImage(textureImage)
			.SetExtent(textureImage->GetExtent())
			.SetSubresourceIndex(rhi::ImageSubresource()
				.SetImageAspect(rhi::ImageAspect::Color)
				.SetMipLevel(0)
				.SetArrayLayer(0));

		commandBuffer->CopyBufferToImage(bufferRegion, imageRegion);

		imageMemoryBarrier.SetOldLayout(rhi::ImageLayout::TransferDst);
		imageMemoryBarrier.SetNewLayout(rhi::ImageLayout::ShaderReadOnly);

		commandBuffer->ResourceBarrier(imageMemoryBarrier);

		passDataPrepared = true;
	}

	void ScenePass::Draw(uint32_t frame, rhi::CommandBuffer commandBuffer) {
		auto clearColorValue = rhi::ClearColorValue().SetColorValue(std::array<float, 4>({ 0.0f, 0.0f, 0.0f, 0.0f }));

		auto renderPassBeginInfo = rhi::RenderPassBeginInfo()
			.SetRenderPass(renderPass)
			.SetFramebuffer(framebuffers[frame])
			.SetPClearColorValues(&clearColorValue);
		commandBuffer->BeginRenderPass(renderPassBeginInfo);

		commandBuffer->BindPipeline(pipeline);

		commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pipelineLayout, 0, 1, descriptorSets.data());

		commandBuffer->BindVertexBuffer(0, 1, &vertexBuffer);

		// Draw Call
		commandBuffer->Draw(3, 1, 0, 0);

		commandBuffer->EndRenderPass();
	}

}