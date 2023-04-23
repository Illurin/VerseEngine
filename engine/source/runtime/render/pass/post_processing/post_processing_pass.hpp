#pragma once

#include "../base_pass.hpp"

namespace engine {

    class PostProcessingPass {
    public:
        PostProcessingPass(rhi::Image shaderReadImage);
        ~PostProcessingPass();

        rhi::Pipeline GetPipeline() const { return pipeline; }

    private:
        void BuildStorageImage();
        void BuildDescriptorLayout();
        void BuildDescriptors(rhi::Image shaderReadImage);
        void BuildPipeline();

    private:
        rhi::Image                      storageImage{ nullptr };
        rhi::DescriptorSetLayout        descriptorSetLayout{ nullptr };
        rhi::DescriptorPool             descriptorPool{ nullptr };
        std::vector<rhi::DescriptorSet> descriptorSets;
        rhi::PipelineLayout             pipelineLayout{ nullptr };
        rhi::Pipeline                   pipeline{ nullptr };
    };

}