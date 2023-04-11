#pragma once

#include "system.hpp"
#include "runtime/render/render_pass.hpp"

namespace engine {

	class RenderSystem final : public ISystem {
	public:
		RenderSystem();
		~RenderSystem();

		void Start(ecs::registry& registry) override;
		void Update(ecs::registry& registry) override;

	private:
		void BuildMeshes(ecs::registry& registry);
		void BuildDescriptors(ecs::registry& registry);

		void UpdateFrameResource(ecs::registry& registry);
		void UpdateRendering(ecs::registry& registry);

	private:
		rhi::CommandPool                commandPool{ nullptr };
		std::vector<rhi::CommandBuffer> commandBuffers;
		rhi::Fence                      fence{ nullptr };
									    
		// Resource					    
									    
		rhi::Buffer                     vertexBuffer{ nullptr };
		rhi::Buffer                     indexBuffer{ nullptr };
		rhi::Buffer                     passUniformBuffer{ nullptr };
									    
		rhi::DescriptorPool             descriptorPool{ nullptr };
		rhi::DescriptorSet              passDescriptorSet{ nullptr };
									    
		// Render pass				    
									    
		std::unique_ptr<ScenePass>      pScenePass;
		std::unique_ptr<SkyboxPass>     pSkyboxPass;
	};

}