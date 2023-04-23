#include "render_graph.hpp"

namespace engine {

    uint32_t RenderGraph::AddResourceNode() {
        auto index = static_cast<uint32_t>(resourceNodes.size());
        resourceNodes.emplace_back(ResourceNode());
        return index;
    }

    uint32_t RenderGraph::AddPassNode(const std::vector<uint32_t>& inputs,
                                      const std::vector<uint32_t>& outputs){
        auto index = static_cast<uint32_t>(passNodes.size());
        passNodes.emplace_back(PassNode({ inputs, outputs, std::vector<rhi::ImageLayout>(inputs.size()) }));
        return index;
    }

    void RenderGraph::RegisterVirtualResource(uint32_t resourceNode, const rhi::ImageCreateInfo& info) {
        resourceNodes[resourceNode].imageCreateInfo = info;
    }

    void RenderGraph::RegisterImportedResource(uint32_t resourceNode, rhi::Image image, rhi::ImageLayout layout) {
        resourceNodes[resourceNode].imported = true;
        resourceNodes[resourceNode].image = image;
        resourceNodes[resourceNode].imageLayout = layout;
    }

    void RenderGraph::RegisterPassResource(uint32_t passNode, uint32_t resourceNode,
                                           rhi::ImageUsage, rhi::ImageLayout, const rhi::ImageViewInfo&) {

    }

}