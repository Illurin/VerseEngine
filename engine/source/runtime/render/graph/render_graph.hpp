#pragma once

#include "runtime/render/pass/base_pass.hpp"

namespace engine {

    class RenderGraphExecutor;

    class RenderGraph final {
    private:
        struct ResourceEntry {

        };

    public:


    public:
        RenderGraph() {}

        uint32_t AddResourceNode();
        uint32_t AddPassNode(const std::vector<uint32_t>& inputs,
                             const std::vector<uint32_t>& outputs);

        void RegisterVirtualResource(uint32_t resourceNode, const rhi::ImageCreateInfo&);
        void RegisterImportedResource(uint32_t resourceNode, rhi::Image, rhi::ImageLayout);
        void RegisterPassResource(uint32_t passNode, uint32_t resourceNode,
                                  rhi::ImageUsage, rhi::ImageLayout, const rhi::ImageViewInfo&);

        // void RegisterPassFunction();

        // RenderGraphExecutor Compile();

    private:
        struct ResourceNode {
            bool                 imported{ false };
            rhi::Image           image{ nullptr };
            rhi::ImageLayout     imageLayout{ 0 };
            rhi::ImageCreateInfo imageCreateInfo{};
        };

        struct PassNode {
            std::vector<uint32_t>         inputs;
            std::vector<uint32_t>         outputs;
            std::vector<rhi::ImageLayout> inputDependencies;
        };

    private:
        std::vector<ResourceNode> resourceNodes;
        std::vector<PassNode>     passNodes;
    };

    class RenderGraphExecutor final {
    public:
        void Execute() {}

    private:

    };

}