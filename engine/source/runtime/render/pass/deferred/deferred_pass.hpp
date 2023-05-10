#pragma once

#include "../base_pass.hpp"

namespace engine {

    class DeferredPass {
    public:
        DeferredPass();
        ~DeferredPass();

    private:
        void BuildImages();
        void BuildDescriptorSetLayout();
        void BuildDescriptors();
        void BuildRenderPasses();
        void BuildPipelines();
        void BuildFramebuffers();

    private:
        rhi::Image renderTarget{ nullptr };

        struct {
            rhi::Image depth{ nullptr };
            rhi::Image normal{ nullptr };
            rhi::Image worldPos{ nullptr };
            rhi::Image diffuseAlbedo{ nullptr };
            rhi::Image material{ nullptr };
        } gBuffer;

        rhi::DescriptorPool      descriptorPool{ nullptr };

        rhi::RenderPass          outputRenderPass{ nullptr };
        rhi::PipelineLayout      outputPipelineLayout{ nullptr };
        rhi::PipelineLayout      outputPipeline{ nullptr };

        rhi::RenderPass          shadingRenderPass{ nullptr };
        rhi::DescriptorSetLayout shadingDescriptorSetLayout{ nullptr };
        rhi::DescriptorSet       shadingDescriptorSet{ nullptr };
        rhi::PipelineLayout      shadingPipelineLayout{ nullptr };
        rhi::Pipeline            shadingPipeline{ nullptr };
    };

}