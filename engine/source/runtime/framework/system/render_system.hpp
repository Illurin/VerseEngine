#pragma once

#include "system.hpp"
#include "runtime/render/render_pass.hpp"

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

        void Start() override;
        void Update() override;

    private:
        void BuildMeshes();
        void BuildAccelerationStructures();
        void BuildDescriptors();
        
        void UpdateFrameResource();
        void UpdateCommandBuffers();
        void PrepareRendering();
        void UpdateRendering();

    private:
        rhi::CommandPool                     commandPool{ nullptr };
        std::vector<rhi::CommandBuffer>      commandBuffers;
        rhi::Fence                           fence{ nullptr };
        bool                                 prepared{ false };

        // Scene

        std::array<uint32_t, lightTypeCount> lightCounts;
                                        
        // Resource                        
                                        
        rhi::Buffer                          vertexBuffer{ nullptr };
        uint32_t                             maxVertexCount{ 0 };
        rhi::Buffer                          indexBuffer{ nullptr };
        rhi::Buffer                          passUniformBuffer{ nullptr };
        std::vector<rhi::Buffer>             transformUniformBuffers;
                                             
        rhi::DescriptorPool                  descriptorPool{ nullptr };
        rhi::DescriptorSet                   passDescriptorSet{ nullptr };
        std::vector<rhi::DescriptorSet>      transformDescriptorSets;

        std::vector<rhi::AccelerationStructureGeometry> blasGeometries;
        std::vector<rhi::AccelerationStructureGeometry> tlasGeometries;
        rhi::AccelerationStructureBuildGeometryInfo     blasBuildGeoInfo{};
        rhi::AccelerationStructureBuildGeometryInfo     tlasBuildGeoInfo{};
        rhi::Buffer                                     instanceBuffer{ nullptr };
        rhi::Buffer                                     scratchBuffer{ nullptr };
        rhi::Buffer                                     blasBuffer{ nullptr };
        rhi::Buffer                                     tlasBuffer{ nullptr };
        rhi::AccelerationStructure                      blas{ nullptr };
        rhi::AccelerationStructure                      tlas{ nullptr };
                                        
        // Render pass                    
                                        
        std::unique_ptr<ScenePass>           pScenePass;
        std::unique_ptr<SkyboxPass>          pSkyboxPass;
        std::unique_ptr<PostProcessingPass>  pPostProcessingPass;
        std::unique_ptr<ScreenQuadPass>      pScreenQuadPass;
    };

}