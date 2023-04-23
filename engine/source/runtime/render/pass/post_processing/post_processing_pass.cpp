#include "post_processing_pass.hpp"

namespace engine {

    PostProcessingPass::PostProcessingPass(rhi::Image shaderReadImage) {
        BuildStorageImage();
        BuildDescriptorLayout();
        BuildDescriptors(shaderReadImage);
        BuildPipeline();
    }

    PostProcessingPass::~PostProcessingPass() {
        if (storageImage) storageImage->Destroy();
        if (descriptorSetLayout) descriptorSetLayout->Destroy();
        if (descriptorPool) descriptorPool->Destroy();
        if (pipelineLayout) pipelineLayout->Destroy();
        if (pipeline) pipeline->Destroy();
    }

    void PostProcessingPass::BuildStorageImage() {
        auto imageInfo = rhi::ImageCreateInfo()
            .SetFormat(rhi::Format::R8G8B8A8Unorm)
            .SetImageType(rhi::ImageType::Image2D)
            .SetExtent(rhi::Extent3D().SetWidth(RenderDevice::GetSwapchain()->GetImageExtent().width)
                .SetHeight(RenderDevice::GetSwapchain()->GetImageExtent().height)
                .SetDepth(1))
            .SetArrayLayers(1)
            .SetMipLevels(1)
            .SetUsage(rhi::ImageUsage::SampledAndStorage)
            .SetImageLayout(rhi::ImageLayout::General);
        storageImage = RenderDevice::GetDevice()->CreateImage(imageInfo);
    }

    void PostProcessingPass::BuildDescriptorLayout() {
        std::vector<rhi::DescriptorSetLayoutBindingInfo> layoutBindings(2);

        layoutBindings[0].SetBinding(0)
                         .SetDescriptorType(rhi::DescriptorType::StorageImage)
                         .SetShaderStage(rhi::ShaderStage::Compute);
        
        layoutBindings[1].SetBinding(1)
                         .SetDescriptorType(rhi::DescriptorType::StorageImage)
                         .SetShaderStage(rhi::ShaderStage::Compute);

        auto descriptorSetLayoutInfo = rhi::DescriptorSetLayoutCreateInfo()
            .SetBindings(layoutBindings);
        descriptorSetLayout = RenderDevice::GetDevice()->CreateDescriptorSetLayout(descriptorSetLayoutInfo);
    }

    void PostProcessingPass::BuildDescriptors(rhi::Image shaderReadImage) {
        auto descriptorSetCount = static_cast<uint32_t>(RenderDevice::GetSwapchain()->GetImages().size());

        std::vector<rhi::DescriptorPoolSize> poolSizes = {
            { rhi::DescriptorType::StorageImage, 2 * descriptorSetCount }
        };

        auto descriptorPoolInfo = rhi::DescriptorPoolCreateInfo()
            .SetMaxSetCount(descriptorSetCount)
            .SetPoolSizes(poolSizes);
        descriptorPool = RenderDevice::GetDevice()->CreateDescriptorPool(descriptorPoolInfo);

        std::vector<rhi::DescriptorSetLayout> layouts(descriptorSetCount);
        for (auto& layout : layouts) {
            layout = descriptorSetLayout;
        }

        auto descriptorSetAllocateInfo = rhi::DescriptorSetAllocateInfo()
            .SetDescriptorSetLayouts(layouts);
        descriptorSets = descriptorPool->AllocateDescriptorSets(descriptorSetAllocateInfo);

        auto shaderReadWrite = rhi::ImageViewInfo()
            .SetImage(shaderReadImage)
            .SetFormat(rhi::Format::R8G8B8A8Unorm)
            .SetViewType(rhi::ImageViewType::Image2D)
            .SetSubresourceRange(rhi::ImageSubresourceRange()
                .SetImageAspect(rhi::ImageAspect::Color)
                .SetBaseMipLevel(0)
                .SetMipLevelCount(1)
                .SetBaseArrayLayer(0)
                .SetArrayLayerCount(1));

        for (uint32_t i = 0; i < descriptorSetCount; i++) {
            auto renderTargetWrite = shaderReadWrite;
            renderTargetWrite.SetImage(storageImage)
                             .SetFormat(rhi::Format::R8G8B8A8Unorm);

            descriptorSets[i]->Write(0, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, shaderReadWrite);
            descriptorSets[i]->Write(1, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, renderTargetWrite);
        }
    }

    void PostProcessingPass::BuildPipeline() {

        // Create pipeline layout

        auto pipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
            .SetDescriptorSetLayoutCount(1)
            .SetPDescriptorSetLayouts(&descriptorSetLayout);

        pipelineLayout = RenderDevice::GetDevice()->CreatePipelineLayout(pipelineLayoutInfo);

        // Compile shaders

        std::vector<uint8_t> shaderSource;

        auto shaderCompiler = RenderDevice::GetInstance()->CreateShaderCompiler();

        shaderCompiler.Compile(Path::GetAbsolutePath("shader/post_processing/hlsl/compute.hlsl"), L"main", ShaderCompileArgument::cs_profile);
        shaderCompiler.GetSourceObject(shaderSource);

        auto shaderModuleInfo = rhi::ShaderModuleCreateInfo()
            .SetSourceCode(shaderSource)
            .SetPEntryPoint("main")
            .SetShaderStage(rhi::ShaderStage::Compute);

        auto computeShaderModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

        // Create pipeline

        auto pipelineInfo = rhi::ComputePipelineCreateInfo()
            .SetShaderModule(computeShaderModule)
            .SetLayout(pipelineLayout);

        pipeline = RenderDevice::GetDevice()->CreateComputePipeline(pipelineInfo);
    }

}