#include "deferred_pass.hpp"

namespace engine {

    DeferredPass::DeferredPass() {
        BuildImages();
        BuildDescriptorSetLayout();
        BuildDescriptors();
        BuildRenderPasses();
        BuildPipelines();
        BuildFramebuffers();
    }

    DeferredPass::~DeferredPass() {
        renderTarget->Destroy();
        gBuffer.depth->Destroy();
        gBuffer.normal->Destroy();
        gBuffer.worldPos->Destroy();
        gBuffer.diffuseAlbedo->Destroy();
        gBuffer.material->Destroy();
        descriptorPool->Destroy();
        outputRenderPass->Destroy();
        outputPipelineLayout->Destroy();
        outputPipeline->Destroy();
        shadingRenderPass->Destroy();
        shadingDescriptorSetLayout->Destroy();
        shadingPipelineLayout->Destroy();
        shadingPipeline->Destroy();
    }

    void DeferredPass::BuildImages() {
        auto imageInfo = rhi::ImageCreateInfo()
            .SetFormat(rhi::Format::R16G16B16A16Sfloat)
            .SetImageType(rhi::ImageType::Image2D)
            .SetExtent(rhi::Extent3D().SetWidth(RenderDevice::GetSwapchain()->GetImageExtent().width)
                .SetHeight(RenderDevice::GetSwapchain()->GetImageExtent().height)
                .SetDepth(1))
            .SetArrayLayers(1)
            .SetMipLevels(1)
            .SetUsage(rhi::ImageUsage::ColorAttachmentAndStorage);
        renderTarget = RenderDevice::GetDevice()->CreateImage(imageInfo);

        imageInfo.SetFormat(rhi::Format::D16Unorm)
                 .SetUsage(rhi::ImageUsage::DepthStencilAttachmentAndSampled);
        gBuffer.depth = RenderDevice::GetDevice()->CreateImage(imageInfo);

        imageInfo.SetFormat(rhi::Format::R8G8B8A8Unorm)
                 .SetUsage(rhi::ImageUsage::ColorAttachmentAndSampled);
        gBuffer.diffuseAlbedo = RenderDevice::GetDevice()->CreateImage(imageInfo);

        imageInfo.SetFormat(rhi::Format::R32G32B32Sfloat);
        gBuffer.normal = RenderDevice::GetDevice()->CreateImage(imageInfo);
        gBuffer.worldPos = RenderDevice::GetDevice()->CreateImage(imageInfo);
        gBuffer.material = RenderDevice::GetDevice()->CreateImage(imageInfo);
    }

    void DeferredPass::BuildDescriptorSetLayout() {
        std::vector<rhi::DescriptorSetLayoutBindingInfo> objectLayoutBindings;

        for (uint32_t i = 0; i < 5; i++) {
            objectLayoutBindings.emplace_back(rhi::DescriptorSetLayoutBindingInfo()
                .SetBinding(i)
                .SetDescriptorType(rhi::DescriptorType::SampledImage)
                .SetShaderStage(rhi::ShaderStage::Fragment));
        }

        objectLayoutBindings.emplace_back(rhi::DescriptorSetLayoutBindingInfo()
            .SetBinding(5)
            .SetDescriptorType(rhi::DescriptorType::Sampler)
            .SetShaderStage(rhi::ShaderStage::Fragment));

        shadingDescriptorSetLayout = RenderDevice::GetDevice()->CreateDescriptorSetLayout(rhi::DescriptorSetLayoutCreateInfo().SetBindings(objectLayoutBindings));
    }

    void DeferredPass::BuildDescriptors() {
        
    }

    void DeferredPass::BuildRenderPasses() {

    }

    void DeferredPass::BuildPipelines() {

    }

    void DeferredPass::BuildFramebuffers() {

    }

}