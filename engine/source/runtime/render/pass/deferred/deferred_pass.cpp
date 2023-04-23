#include "deferred_pass.hpp"

//#include "runtime/render/graph/render_graph.hpp"

namespace engine {

    DeferredPass::DeferredPass() {
        /*RenderGraph renderGraph;

        auto diffuseAlbedo     = renderGraph.AddResourceNode();
        auto normal            = renderGraph.AddResourceNode();
        auto materialConstants = renderGraph.AddResourceNode();
        auto worldPos          = renderGraph.AddResourceNode();
        auto depth             = renderGraph.AddResourceNode();
        auto renderTarget      = renderGraph.AddResourceNode();

        auto outputPass = renderGraph.AddPassNode(
            { },
            { diffuseAlbedo, normal, materialConstants, worldPos, depth }
        );
        auto shadingPass = renderGraph.AddPassNode(
            { diffuseAlbedo, normal, materialConstants, worldPos },
            { renderTarget }
        );

        auto imageCreateInfo = rhi::ImageCreateInfo()
            .SetUsage(rhi::ImageUsage::ColorAttachment)
            .SetFormat(rhi::Format::R8G8B8A8Unorm)
            .SetImageType(rhi::ImageType::Image2D)
            .SetExtent(rhi::Extent3D({ RenderDevice::GetSwapchain()->GetImageExtent().width,
                                       RenderDevice::GetSwapchain()->GetImageExtent().height, 1 }))
            .SetMipLevels(1)
            .SetArrayLayers(1);
            
        auto resourceManagement = renderGraph.CreateResourceManagement();
        resourceManagement.RegisterTemporalResource(diffuseAlbedo, imageCreateInfo);
        resourceManagement.RegisterTemporalResource(normal, imageCreateInfo.SetFormat(rhi::Format::R32G32B32A32Sfloat));
        resourceManagement.RegisterTemporalResource(materialConstants, imageCreateInfo);
        resourceManagement.RegisterTemporalResource(worldPos, imageCreateInfo);
        resourceManagement.RegisterTemporalResource(depth, imageCreateInfo.SetUsage(rhi::ImageUsage::DepthStencilAttachment)
                                                                          .SetFormat(rhi::Format::D16Unorm));
        resourceManagement.RegisterTemporalResource(renderTarget, imageCreateInfo.SetUsage(rhi::ImageUsage::ColorAttachment)
                                                                                 .SetFormat(rhi::Format::R8G8B8A8Unorm));

        

        auto executor = renderGraph.CreateExecutor();*/
    }

}