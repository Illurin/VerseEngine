#pragma once

#include "../base_pass.hpp"

namespace engine {

    //
    // extract pipeline:
    //
    //   - shaderRead -> storage image 0
    //
    // blur pipeline:
    //
    //   - storage image 0 -> storage image 1
    //   - storage image 1 -> storage image 0
    // 
    // bloom pipeline:
    //
    //   - shaderRead, storage image 0 -> storage image 1
    // 

    class PostProcessingPass {
    public:
        PostProcessingPass(rhi::Image shaderReadImage);
        ~PostProcessingPass();

        rhi::Image GetRenderTarget() const { return storageImages[1]; }

        void Prepare(rhi::CommandBuffer);
        void Execute(rhi::CommandBuffer);

    private:
        void BuildStorageImage();
        void BuildDescriptorSetLayout();
        void BuildDescriptors(rhi::Image shaderReadImage);
        void BuildPipeline();

    private:
        std::array<rhi::Image, 2>       storageImages;
        rhi::DescriptorPool             descriptorPool{ nullptr };
        rhi::DescriptorSetLayout        storageImageSetLayout{ nullptr };
        rhi::DescriptorSetLayout        combineImageSetLayout{ nullptr };
        std::vector<rhi::DescriptorSet> storageImageDescriptorSets;

        // blur pipelines

        rhi::PipelineLayout             blurPipelineLayout{ nullptr };
        std::array<rhi::Pipeline, 2>    blurPipelines;

        // extract pipeline

        rhi::PipelineLayout             extractPipelineLayout{ nullptr };
        rhi::Pipeline                   extractPipeline{ nullptr };

        // bloom pipeline
        
        rhi::PipelineLayout             bloomPipelineLayout{ nullptr };
        rhi::Pipeline                   bloomPipeline{ nullptr };
    };

}