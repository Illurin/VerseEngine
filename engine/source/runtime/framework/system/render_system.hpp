#pragma once

#include "system.hpp"
#include "runtime/render/render_pass.hpp"
#include "runtime/render/graph/render_graph.hpp"

namespace engine {

    //
    // Pass 0: Scene pass
    // 
    //   - Input:
    //   - Output: color attachment, depth attachment
    // 
    //   - Pipeline 0: Scene pipeline
    //   - Pipeline 1: Skybox pipeline
    // 
    // Pass 1: Post processing pass
    // 
    //   - Input: color attachment
    //   - Output: storage image
    // 
    //   - Pipeline 0: Blur H pipeline
    //   - Pipeline 1: Blur V pipeline
    //
    // Pass 2: Screen quad pass
    //
    //   - Input: render target
    //   - output: swapchain image
    //
    //   - Pipeline 0: Screen quad pipeline
    //

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
        void UpdateCommandBuffers(ecs::registry& registry);
        void PrepareRendering();
        void UpdateRendering();

    private:
        rhi::CommandPool                commandPool{ nullptr };
        std::vector<rhi::CommandBuffer> commandBuffers;
        rhi::Fence                      fence{ nullptr };
        bool                            prepared{ false };
                                        
        // Resource                        
                                        
        rhi::Buffer                     vertexBuffer{ nullptr };
        rhi::Buffer                     indexBuffer{ nullptr };
        rhi::Buffer                     passUniformBuffer{ nullptr };
                                        
        rhi::DescriptorPool             descriptorPool{ nullptr };
        rhi::DescriptorSet              passDescriptorSet{ nullptr };
                                        
        // Render pass                    
                                        
        std::unique_ptr<ScenePass>          pScenePass;
        std::unique_ptr<SkyboxPass>         pSkyboxPass;
        std::unique_ptr<PostProcessingPass> pPostProcessingPass;
        std::unique_ptr<ScreenQuadPass>     pScreenQuadPass;
    };

}