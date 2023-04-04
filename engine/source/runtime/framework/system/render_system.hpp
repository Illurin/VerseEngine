#pragma once

#include "system.hpp"

namespace engine {

	class RenderSystem : public ISystem {
	public:
		RenderSystem(rhi::Device device) : device(device) {}
		void SetCommandBuffer(rhi::CommandBuffer cmd) { this->cmd = cmd; }
		void Clear();

		void Start(ecs::registry& registry) override;
		void Update(ecs::registry& registry) override;

		void BuildVertexBuffers(ecs::registry& registry);
		void BuildUniformBuffers(ecs::registry& registry);

	private:
		rhi::Device device{ nullptr };
		rhi::CommandBuffer cmd{ nullptr };

		rhi::Buffer vertexBuffer{ nullptr };
		rhi::Buffer indexBuffer{ nullptr };

		rhi::DescriptorPool descriptorPool{ nullptr };
	};

}