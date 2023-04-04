#include "render_system.hpp"

namespace engine {

	void RenderSystem::Start(ecs::registry& registry) {
		BuildVertexBuffers(registry);
		BuildUniformBuffers(registry);
	}

	void RenderSystem::Update(ecs::registry& registry) {
		auto view = registry.view<MeshRendererComponent>();

		
	}

	void RenderSystem::Clear() {
		if (vertexBuffer) vertexBuffer->Destroy();
		if (indexBuffer) indexBuffer->Destroy();
		if (descriptorPool) descriptorPool->Destroy();
	}

	void RenderSystem::BuildVertexBuffers(ecs::registry& registry) {
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
		vertexBuffer = device->CreateBuffer(vertexBufferInfo);

		auto indexBufferInfo = rhi::BufferCreateInfo()
			.SetSize(sizeof(uint32_t) * indices.size())
			.SetUsage(rhi::BufferUsage::IndexBuffer);
		indexBuffer = device->CreateBuffer(indexBufferInfo);

		auto pVertexBuffer = reinterpret_cast<ObjectVertexData*>(vertexBuffer->Map());
		std::copy(vertices.begin(), vertices.end(), pVertexBuffer);
		vertexBuffer->Unmap();

		auto pIndexBuffer = reinterpret_cast<uint32_t*>(indexBuffer->Map());
		std::copy(indices.begin(), indices.end(), pIndexBuffer);
		indexBuffer->Unmap();
	}

	void RenderSystem::BuildUniformBuffers(ecs::registry& registry) {
		auto view = registry.view<TransformComponent>();
		
		view.each([&](TransformComponent& transform) {
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

			auto uniformBufferInfo = rhi::BufferCreateInfo()
				.SetSize(sizeof(float[3][4]))
				.SetUsage(rhi::BufferUsage::UniformBuffer);
			transform.uniformBuffer = device->CreateBuffer(uniformBufferInfo);

			auto p = reinterpret_cast<float**>(transform.uniformBuffer->Map());
			p[0][0] = m(0, 0); p[0][1] = m(0, 1); p[0][2] = m(0, 2); p[0][3] = m(0, 3);
			p[1][0] = m(1, 0); p[1][1] = m(1, 1); p[1][2] = m(1, 2); p[1][3] = m(1, 3);
			p[2][0] = m(2, 0); p[2][1] = m(2, 1); p[2][2] = m(2, 2); p[2][3] = m(2, 3);
			transform.uniformBuffer->Unmap();
		});
	}

}
