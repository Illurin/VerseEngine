#pragma once

#include "../base_pass.hpp"

namespace engine {

    class ScenePass {
    public:
        ScenePass();
        ~ScenePass();

        void Prepare(rhi::CommandBuffer);
        void Begin(rhi::CommandBuffer);

        rhi::Image                            GetRenderTarget()         const { return renderTarget; }
        std::vector<rhi::DescriptorSetLayout> GetDescriptorSetLayouts() const { return descriptorSetLayouts; }
        rhi::PipelineLayout                   GetPipelineLayout()       const { return pipelineLayout; }
        rhi::RenderPass                       GetRenderPass()           const { return renderPass; }

    private:
        void BuildDescriptorLayout();
        void BuildRenderPass();
        void BuildPipeline();
        void BuildRenderTarget();
        void BuildDepthImage();
        void BuildFramebuffers();

    private:
        rhi::Format                           renderTargetFormat{ rhi::Format::R16G16B16A16Sfloat };
        std::vector<rhi::DescriptorSetLayout> descriptorSetLayouts;
        rhi::PipelineLayout                   pipelineLayout{ nullptr };
        rhi::RenderPass                       renderPass{ nullptr };
        rhi::Pipeline                         pipeline{ nullptr };
        rhi::Image                            renderTarget{ nullptr };
        rhi::Image                            depthImage{ nullptr };
        rhi::Framebuffer                      framebuffer{ nullptr };
    };

}