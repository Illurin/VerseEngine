#pragma once

#include "../base_pass.hpp"

namespace engine {

    class RayTracingPass {
    public:
        RayTracingPass();
        ~RayTracingPass();

        rhi::DescriptorSetLayout GetDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        void BuildDescriptorLayout();
        void BuildDescriptorSets();
        void BuildPipeline();
        void BuildShaderBindingTable();

    private:
        rhi::Buffer                                     sbtBuffer{ nullptr };

        rhi::DescriptorSetLayout                        descriptorSetLayout{ nullptr };
        rhi::PipelineLayout                             pipelineLayout{ nullptr };

        rhi::Pipeline                                   pipeline{ nullptr };
        rhi::BufferStridedRegion                        raygenRegion;
        rhi::BufferStridedRegion                        missRegion;
        rhi::BufferStridedRegion                        hitRegion;
    };

}