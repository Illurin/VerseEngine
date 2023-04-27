#pragma once

#include "../base_pass.hpp"

namespace engine {

    class ScreenQuadPass {
    public:
        ScreenQuadPass(rhi::Image shaderReadImage);
        ~ScreenQuadPass();

        void Execute(uint32_t frame, rhi::CommandBuffer);

    private:
        void BuildSampler();
        void BuildDescriptorLayout();
        void BuildDescriptors(rhi::Image shaderReadImage);
        void BuildRenderPass();
        void BuildPipeline();
        void BuildFramebuffers();

    private:
        rhi::Sampler                    sampler{ nullptr };
        rhi::DescriptorSetLayout        descriptorSetLayout{ nullptr };
        rhi::DescriptorPool             descriptorPool{ nullptr };
        rhi::DescriptorSet              descriptorSet{ nullptr };
        rhi::PipelineLayout             pipelineLayout{ nullptr };
        rhi::RenderPass                 renderPass{ nullptr };
        rhi::Pipeline                   pipeline{ nullptr };
        std::vector<rhi::Framebuffer>   framebuffers;
    };

}