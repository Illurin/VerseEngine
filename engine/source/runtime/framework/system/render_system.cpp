#include "render_system.hpp"

namespace engine {

	RenderSystem::RenderSystem() {
		auto commandPoolInfo = rhi::CommandPoolCreateInfo()
			.SetQueue(RenderDevice::GetQueue());
		commandPool = RenderDevice::GetDevice()->CreateCommandPool(commandPoolInfo);

		commandBuffers = commandPool->AllocateCommandBuffers(static_cast<uint32_t>(RenderDevice::GetSwapchain()->GetImages().size()));

		fence = RenderDevice::GetDevice()->CreateFence(rhi::FenceCreateInfo());

		pScenePass = std::make_unique<ScenePass>();
	}

	RenderSystem::~RenderSystem() {
		commandPool->Destroy();
		fence->Destroy();
		if (vertexBuffer) vertexBuffer->Destroy();
		if (indexBuffer) indexBuffer->Destroy();
		if (passUniformBuffer) passUniformBuffer->Destroy();
		if (descriptorPool) descriptorPool->Destroy();
	}

	void RenderSystem::Start(ecs::registry& registry) {
		BuildMeshes(registry);
		BuildDescriptors(registry);
	}

	void RenderSystem::Update(ecs::registry& registry) {
		uint32_t swapchainImageIndex = RenderDevice::GetQueue()->AcquireNextImage(RenderDevice::GetSwapchain());
		auto& commandBuffer = commandBuffers[swapchainImageIndex];

		commandBuffer->Reset();
		commandBuffer->Begin(rhi::CommandBufferBeginInfo());

		pScenePass->Begin(swapchainImageIndex, commandBuffer);

		commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pScenePass->GetPipelineLayout(), 2, 1, &passDescriptorSet);
		commandBuffer->BindVertexBuffer(0, 1, &vertexBuffer);
		commandBuffer->BindIndexBuffer(indexBuffer, 0, rhi::IndexType::Uint32);

		auto view = registry.view<const TransformComponent, const MeshRendererComponent, const MaterialComponent>();

		view.each([&](const TransformComponent& transform, const MeshRendererComponent& meshRenderer, const MaterialComponent& material) {
			commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pScenePass->GetPipelineLayout(), 0, 1, &transform.descriptorSet);
			commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pScenePass->GetPipelineLayout(), 1, 1, &material.descriptorSet);

			commandBuffer->DrawIndexed(static_cast<uint32_t>(meshRenderer.indices.size()), 1, meshRenderer.firstIndex, meshRenderer.vertexOffset, 0);
		});

		commandBuffer->EndRenderPass();
		commandBuffer->End();

		fence->Reset();
		RenderDevice::GetQueue()->Submit(1, &commandBuffer, fence);
		fence->Wait();
		RenderDevice::GetQueue()->Present(RenderDevice::GetSwapchain(), swapchainImageIndex);
	}

	void RenderSystem::BuildMeshes(ecs::registry& registry) {
		auto view = registry.view<MeshRendererComponent>();

		std::vector<ObjectVertexData> vertices;
		std::vector<uint32_t> indices;

		view.each([&](MeshRendererComponent& meshRenderer) {
			meshRenderer.vertexOffset = static_cast<uint32_t>(vertices.size());
			meshRenderer.firstIndex = static_cast<uint32_t>(indices.size());
			vertices.insert(vertices.end(), meshRenderer.vertices.begin(), meshRenderer.vertices.end());
			indices.insert(indices.end(), meshRenderer.indices.begin(), meshRenderer.indices.end());
		});

		auto vertexBufferInfo = rhi::BufferCreateInfo()
			.SetSize(sizeof(ObjectVertexData) * vertices.size())
			.SetUsage(rhi::BufferUsage::VertexBuffer);
		vertexBuffer = RenderDevice::GetDevice()->CreateBuffer(vertexBufferInfo);

		auto indexBufferInfo = rhi::BufferCreateInfo()
			.SetSize(sizeof(uint32_t) * indices.size())
			.SetUsage(rhi::BufferUsage::IndexBuffer);
		indexBuffer = RenderDevice::GetDevice()->CreateBuffer(indexBufferInfo);

		auto pVertexBuffer = reinterpret_cast<ObjectVertexData*>(vertexBuffer->Map());
		std::copy(vertices.begin(), vertices.end(), pVertexBuffer);
		vertexBuffer->Unmap();

		auto pIndexBuffer = reinterpret_cast<uint32_t*>(indexBuffer->Map());
		std::copy(indices.begin(), indices.end(), pIndexBuffer);
		indexBuffer->Unmap();
	}

	void RenderSystem::BuildDescriptors(ecs::registry& registry) {
		auto transformView = registry.view<TransformComponent>();
		auto materialView = registry.view<MaterialComponent>();
		auto cameraView = registry.view<CameraComponent>();
		
		std::vector<rhi::DescriptorPoolSize> descriptorPoolSizes = {
			{ rhi::DescriptorType::UniformBuffer,
			  static_cast<uint32_t>(transformView.size()) + static_cast<uint32_t>(materialView.size()) + 1 },
			{ rhi::DescriptorType::SampledImage, static_cast<uint32_t>(materialView.size()) },
			{ rhi::DescriptorType::Sampler, static_cast<uint32_t>(materialView.size()) },
		};

		auto maxSetCount = static_cast<uint32_t>(transformView.size()) + static_cast<uint32_t>(materialView.size()) + 1;

		auto descriptorPoolInfo = rhi::DescriptorPoolCreateInfo()
			.SetPoolSizes(descriptorPoolSizes)
			.SetMaxSetCount(maxSetCount);
		descriptorPool = RenderDevice::GetDevice()->CreateDescriptorPool(descriptorPoolInfo);

		transformView.each([&](TransformComponent& transform) {
			auto s = math::AlignedScaling3f(transform.scale);
			auto lr = math::AngleAxisf(transform.localEulerAngle.x(), math::Vector3f::UnitX())
				    * math::AngleAxisf(transform.localEulerAngle.y(), math::Vector3f::UnitY())
				    * math::AngleAxisf(transform.localEulerAngle.z(), math::Vector3f::UnitZ());
			auto t = math::Translation3f(transform.position);
			auto gr = math::AngleAxisf(transform.globalEulerAngle.x(), math::Vector3f::UnitX())
				    * math::AngleAxisf(transform.globalEulerAngle.y(), math::Vector3f::UnitY())
				    * math::AngleAxisf(transform.globalEulerAngle.z(), math::Vector3f::UnitZ());
			math::Affine3f affine(gr * t * lr * s);
			auto m = affine.matrix();

			std::array<std::array<float, 4>, 3> worldMatrix;
			worldMatrix[0][0] = m(0, 0); worldMatrix[0][1] = m(0, 1); worldMatrix[0][2] = m(0, 2); worldMatrix[0][3] = m(0, 3);
			worldMatrix[1][0] = m(1, 0); worldMatrix[1][1] = m(1, 1); worldMatrix[1][2] = m(1, 2); worldMatrix[1][3] = m(1, 3);
			worldMatrix[2][0] = m(2, 0); worldMatrix[2][1] = m(2, 1); worldMatrix[2][2] = m(2, 2); worldMatrix[2][3] = m(2, 3);

			auto uniformBufferInfo = rhi::BufferCreateInfo()
				.SetSize(sizeof(ObjectConstants))
				.SetUsage(rhi::BufferUsage::UniformBuffer);
			transform.uniformBuffer = RenderDevice::GetDevice()->CreateBuffer(uniformBufferInfo);

			auto p = reinterpret_cast<ObjectConstants*>(transform.uniformBuffer->Map());
			p->worldMatrix = worldMatrix;
			transform.uniformBuffer->Unmap();

			std::vector allocateLayouts = { pScenePass->GetDescriptorSetLayouts()[0] };

			auto allocateInfo = rhi::DescriptorSetAllocateInfo()
				.SetDescriptorSetLayouts(allocateLayouts);
			auto desriptorSets = descriptorPool->AllocateDescriptorSets(allocateInfo);

			desriptorSets[0]->Write(0, rhi::DescriptorType::UniformBuffer, transform.uniformBuffer);
			transform.descriptorSet = desriptorSets[0];
		});

		materialView.each([&](MaterialComponent& material) {
			auto uniformBufferInfo = rhi::BufferCreateInfo()
				.SetSize(sizeof(MaterialConstants))
				.SetUsage(rhi::BufferUsage::UniformBuffer);
			material.uniformBuffer = RenderDevice::GetDevice()->CreateBuffer(uniformBufferInfo);

			auto p = reinterpret_cast<MaterialConstants*>(material.uniformBuffer->Map());
			*p = material.constants;
			material.uniformBuffer->Unmap();

			std::vector allocateLayouts = { pScenePass->GetDescriptorSetLayouts()[1] };

			auto allocateInfo = rhi::DescriptorSetAllocateInfo()
				.SetDescriptorSetLayouts(allocateLayouts);
			auto desriptorSets = descriptorPool->AllocateDescriptorSets(allocateInfo);

			auto imageViewInfo = rhi::ImageViewInfo()
				.SetImage(material.texture)
				.SetFormat(rhi::Format::R8G8B8A8Unorm)
				.SetViewType(rhi::ImageViewType::Image2D)
				.SetSubresourceRange(rhi::ImageSubresourceRange()
					.SetImageAspect(rhi::ImageAspect::Color)
					.SetBaseArrayLayer(0).SetBaseMipLevel(0).SetArrayLayerCount(1).SetMipLevelCount(1));

			desriptorSets[0]->Write(0, rhi::DescriptorType::UniformBuffer, material.uniformBuffer);
			desriptorSets[0]->Write(1, rhi::DescriptorType::SampledImage, rhi::ImageLayout::ShaderReadOnly, imageViewInfo);
			desriptorSets[0]->Write(2, rhi::DescriptorType::Sampler, material.sampler);
			material.descriptorSet = desriptorSets[0];
		});

		auto uniformBufferInfo = rhi::BufferCreateInfo()
			.SetSize(sizeof(PassConstants))
			.SetUsage(rhi::BufferUsage::UniformBuffer);
		passUniformBuffer = RenderDevice::GetDevice()->CreateBuffer(uniformBufferInfo);

		auto p = reinterpret_cast<PassConstants*>(passUniformBuffer->Map());

		cameraView.each([&](CameraComponent& camera) {
			if (!camera.active) return;

			p->viewMatrix = camera.camera.GetViewMatrix();
			p->projMatrix = camera.camera.GetProjMatrix();
		});

		passUniformBuffer->Unmap();

		std::vector allocateLayouts = { pScenePass->GetDescriptorSetLayouts()[2] };

		auto allocateInfo = rhi::DescriptorSetAllocateInfo()
			.SetDescriptorSetLayouts(allocateLayouts);
		auto desriptorSets = descriptorPool->AllocateDescriptorSets(allocateInfo);

		desriptorSets[0]->Write(0, rhi::DescriptorType::UniformBuffer, passUniformBuffer);
		passDescriptorSet = desriptorSets[0];
	}

}
