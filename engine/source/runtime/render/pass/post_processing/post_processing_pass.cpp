#include "post_processing_pass.hpp"

namespace engine {

    PostProcessingPass::PostProcessingPass(rhi::Image shaderReadImage) {
        BuildStorageImage();
        BuildDescriptorLayout();
        BuildDescriptors(shaderReadImage);
        BuildPipeline();
    }

    PostProcessingPass::~PostProcessingPass() {
        if (descriptorPool) descriptorPool->Destroy();
        if (storageImageSetLayout) storageImageSetLayout->Destroy();
        if (combineImageSetLayout) combineImageSetLayout->Destroy();
        if (blurPipelineLayout) blurPipelineLayout->Destroy();
        if (extractPipelineLayout) extractPipelineLayout->Destroy();
        if (extractPipeline) extractPipeline->Destroy();
        if (bloomPipelineLayout) bloomPipelineLayout->Destroy();
        if (bloomPipeline) bloomPipeline->Destroy();
        for (auto storageImage : storageImages) {
            if (storageImage) storageImage->Destroy();
        }
        for (auto pipeline : blurPipelines) {
            if (pipeline) pipeline->Destroy();
        }
    }

    void PostProcessingPass::BuildStorageImage() {
        auto imageInfo = rhi::ImageCreateInfo()
            .SetFormat(rhi::Format::R16G16B16A16Sfloat)
            .SetImageType(rhi::ImageType::Image2D)
            .SetExtent(rhi::Extent3D().SetWidth(RenderDevice::GetSwapchain()->GetImageExtent().width)
                .SetHeight(RenderDevice::GetSwapchain()->GetImageExtent().height)
                .SetDepth(1))
            .SetArrayLayers(1)
            .SetMipLevels(1)
            .SetUsage(rhi::ImageUsage::SampledAndStorage);

        storageImages[0] = RenderDevice::GetDevice()->CreateImage(imageInfo);
        storageImages[1] = RenderDevice::GetDevice()->CreateImage(imageInfo);
    }

    void PostProcessingPass::BuildDescriptorLayout() {
        std::vector<rhi::DescriptorSetLayoutBindingInfo> layoutBindings(3);

        layoutBindings[0].SetBinding(0)
                         .SetDescriptorType(rhi::DescriptorType::StorageImage)
                         .SetShaderStage(rhi::ShaderStage::Compute);
        
        layoutBindings[1].SetBinding(1)
                         .SetDescriptorType(rhi::DescriptorType::StorageImage)
                         .SetShaderStage(rhi::ShaderStage::Compute);

        layoutBindings[2].SetBinding(2)
                         .SetDescriptorType(rhi::DescriptorType::StorageImage)
                         .SetShaderStage(rhi::ShaderStage::Compute);

        auto descriptorSetLayoutInfo = rhi::DescriptorSetLayoutCreateInfo()
            .SetBindingCount(2)
            .SetPBindings(layoutBindings.data());
        storageImageSetLayout = RenderDevice::GetDevice()->CreateDescriptorSetLayout(descriptorSetLayoutInfo);
        descriptorSetLayoutInfo.SetBindingCount(3);
        combineImageSetLayout = RenderDevice::GetDevice()->CreateDescriptorSetLayout(descriptorSetLayoutInfo);
    }

    void PostProcessingPass::BuildDescriptors(rhi::Image shaderReadImage) {
        std::vector<rhi::DescriptorPoolSize> poolSizes = {
            { rhi::DescriptorType::StorageImage, 9 }
        };

        auto descriptorPoolInfo = rhi::DescriptorPoolCreateInfo()
            .SetMaxSetCount(4)
            .SetPoolSizes(poolSizes);
        descriptorPool = RenderDevice::GetDevice()->CreateDescriptorPool(descriptorPoolInfo);

        std::vector<rhi::DescriptorSetLayout> layouts = {
            storageImageSetLayout, storageImageSetLayout, storageImageSetLayout,
            combineImageSetLayout
        };

        auto descriptorSetAllocateInfo = rhi::DescriptorSetAllocateInfo()
            .SetDescriptorSetLayouts(layouts);
        storageImageDescriptorSets = descriptorPool->AllocateDescriptorSets(descriptorSetAllocateInfo);
        
        std::array<rhi::ImageViewInfo, 3> imageViews;
        for (uint32_t i = 0; i < 3; i++) {
            imageViews[i] = rhi::ImageViewInfo()
                .SetImage(i == 0 ? shaderReadImage : storageImages[i - 1])
                .SetFormat(rhi::Format::R16G16B16A16Sfloat)
                .SetViewType(rhi::ImageViewType::Image2D)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseMipLevel(0)
                    .SetMipLevelCount(1)
                    .SetBaseArrayLayer(0)
                    .SetArrayLayerCount(1));
        }

        storageImageDescriptorSets[0]->Write(0, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViews[0]);
        storageImageDescriptorSets[0]->Write(1, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViews[1]);
        storageImageDescriptorSets[1]->Write(0, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViews[1]);
        storageImageDescriptorSets[1]->Write(1, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViews[2]);
        storageImageDescriptorSets[2]->Write(0, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViews[2]);
        storageImageDescriptorSets[2]->Write(1, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViews[1]);
        storageImageDescriptorSets[3]->Write(0, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViews[0]);
        storageImageDescriptorSets[3]->Write(1, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViews[1]);
        storageImageDescriptorSets[3]->Write(1, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViews[2]);
    }

    void PostProcessingPass::BuildPipeline() {

        // Create pipeline layout

        auto blurPipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
            .SetDescriptorSetLayoutCount(1)
            .SetPDescriptorSetLayouts(&storageImageSetLayout);
        blurPipelineLayout = RenderDevice::GetDevice()->CreatePipelineLayout(blurPipelineLayoutInfo);

        auto extractPipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
            .SetDescriptorSetLayoutCount(1)
            .SetPDescriptorSetLayouts(&storageImageSetLayout);
        extractPipelineLayout = RenderDevice::GetDevice()->CreatePipelineLayout(blurPipelineLayoutInfo);

        auto bloomPipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
            .SetDescriptorSetLayoutCount(1)
            .SetPDescriptorSetLayouts(&combineImageSetLayout);
        bloomPipelineLayout = RenderDevice::GetDevice()->CreatePipelineLayout(bloomPipelineLayoutInfo);

        // Compile shaders

        std::vector<uint8_t> shaderSource;

        auto shaderCompiler = RenderDevice::GetInstance()->CreateShaderCompiler();

        shaderCompiler.Compile(Path::GetAbsolutePath("shader/post_processing/hlsl/compute_blur_h.hlsl"), L"main", ShaderCompileArgument::cs_profile);
        shaderCompiler.GetSourceObject(shaderSource);

        auto shaderModuleInfo = rhi::ShaderModuleCreateInfo()
            .SetSourceCode(shaderSource)
            .SetPEntryPoint("main")
            .SetShaderStage(rhi::ShaderStage::Compute);
        auto computeBlurHModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

        shaderCompiler.Compile(Path::GetAbsolutePath("shader/post_processing/hlsl/compute_blur_v.hlsl"), L"main", ShaderCompileArgument::cs_profile);
        shaderCompiler.GetSourceObject(shaderSource);

        shaderModuleInfo = rhi::ShaderModuleCreateInfo()
            .SetSourceCode(shaderSource)
            .SetPEntryPoint("main")
            .SetShaderStage(rhi::ShaderStage::Compute);
        auto computeBlurVModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

        shaderCompiler.Compile(Path::GetAbsolutePath("shader/post_processing/hlsl/compute_extract.hlsl"), L"main", ShaderCompileArgument::cs_profile);
        shaderCompiler.GetSourceObject(shaderSource);

        shaderModuleInfo = rhi::ShaderModuleCreateInfo()
            .SetSourceCode(shaderSource)
            .SetPEntryPoint("main")
            .SetShaderStage(rhi::ShaderStage::Compute);
        auto computeExtractModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

        shaderCompiler.Compile(Path::GetAbsolutePath("shader/post_processing/hlsl/compute_bloom.hlsl"), L"main", ShaderCompileArgument::cs_profile);
        shaderCompiler.GetSourceObject(shaderSource);

        shaderModuleInfo = rhi::ShaderModuleCreateInfo()
            .SetSourceCode(shaderSource)
            .SetPEntryPoint("main")
            .SetShaderStage(rhi::ShaderStage::Compute);
        auto computeBloomModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

        // Create pipeline
        
        auto pipelineInfo = rhi::ComputePipelineCreateInfo()
            .SetLayout(blurPipelineLayout)
            .SetShaderModule(computeBlurHModule);
        blurPipelines[0] = RenderDevice::GetDevice()->CreateComputePipeline(pipelineInfo);

        pipelineInfo.SetShaderModule(computeBlurVModule);
        blurPipelines[1] = RenderDevice::GetDevice()->CreateComputePipeline(pipelineInfo);

        pipelineInfo.SetLayout(extractPipelineLayout)
                    .SetShaderModule(computeExtractModule);
        extractPipeline = RenderDevice::GetDevice()->CreateComputePipeline(pipelineInfo);

        pipelineInfo.SetLayout(bloomPipelineLayout)
                    .SetShaderModule(computeBloomModule);
        bloomPipeline = RenderDevice::GetDevice()->CreateComputePipeline(pipelineInfo);

        computeBlurHModule->Destroy();
        computeBlurVModule->Destroy();
        computeExtractModule->Destroy();
        computeBloomModule->Destroy();
    }

    void PostProcessingPass::Prepare(rhi::CommandBuffer commandBuffer) {
        auto imageMemoryBarrier = rhi::ImageMemoryBarrierInfo()
            .SetImage(storageImages[0])
            .SetOldLayout(rhi::ImageLayout::Undefined)
            .SetNewLayout(rhi::ImageLayout::General)
            .SetSubresourceRange(rhi::ImageSubresourceRange()
                .SetImageAspect(rhi::ImageAspect::Color)
                .SetBaseMipLevel(0)
                .SetMipLevelCount(1)
                .SetBaseArrayLayer(0)
                .SetArrayLayerCount(1));
        commandBuffer->ResourceBarrier(imageMemoryBarrier);

        imageMemoryBarrier.SetImage(storageImages[1])
                          .SetNewLayout(rhi::ImageLayout::ShaderReadOnly);
        commandBuffer->ResourceBarrier(imageMemoryBarrier);
    }

    void PostProcessingPass::Execute(rhi::CommandBuffer commandBuffer) {

        // Prepare resource barriers

        std::array<rhi::ImageMemoryBarrierInfo, 2> storageImageBarriers;

        storageImageBarriers[0].SetImage(storageImages[0])
                               .SetOldLayout(rhi::ImageLayout::General)
                               .SetNewLayout(rhi::ImageLayout::General)
                               .SetSubresourceRange(rhi::ImageSubresourceRange()
                                   .SetImageAspect(rhi::ImageAspect::Color)
                                   .SetBaseMipLevel(0)
                                   .SetMipLevelCount(1)
                                   .SetBaseArrayLayer(0)
                                   .SetArrayLayerCount(1));

        storageImageBarriers[1] = storageImageBarriers[0];
        storageImageBarriers[1].SetImage(storageImages[1]);

        auto sampledToStorageImageBarrier = storageImageBarriers[1];
        sampledToStorageImageBarrier.SetOldLayout(rhi::ImageLayout::ShaderReadOnly)
                                    .SetNewLayout(rhi::ImageLayout::General);

        auto storageToSampledImageBarrier = storageImageBarriers[1];
        storageToSampledImageBarrier.SetOldLayout(rhi::ImageLayout::General)
                                    .SetNewLayout(rhi::ImageLayout::ShaderReadOnly);
        
        // Calculate dispatch group count

        auto width = RenderDevice::GetSwapchain()->GetImageExtent().width;
        auto height = RenderDevice::GetSwapchain()->GetImageExtent().height;
        auto groupCountX = static_cast<uint32_t>(std::ceil(width / 32.0));
        auto groupCountY = static_cast<uint32_t>(std::ceil(height / 32.0));
        auto blurGroupCountX = static_cast<uint32_t>(std::ceil(width / 256.0));
        auto blurGroupCountY = static_cast<uint32_t>(std::ceil(height / 256.0));
            
        // Start dispatching

        commandBuffer->ResourceBarrier(sampledToStorageImageBarrier);

        // shaderRead -> storage image 0

        commandBuffer->BindPipeline(extractPipeline);
        commandBuffer->BindDescriptorSets(rhi::PipelineType::Compute, extractPipelineLayout, 0, 1, &storageImageDescriptorSets[0]);
        commandBuffer->Dispatch(groupCountX, groupCountY, 1);
        commandBuffer->ResourceBarrier(storageImageBarriers[0]);

        // storage image 0 -> storage image 1

        commandBuffer->BindPipeline(blurPipelines[0]);
        commandBuffer->BindDescriptorSets(rhi::PipelineType::Compute, blurPipelineLayout, 0, 1, &storageImageDescriptorSets[1]);
        commandBuffer->Dispatch(blurGroupCountX, height, 1);
        commandBuffer->ResourceBarrier(storageImageBarriers[1]);

        // storage image 1 -> storage image 0

        commandBuffer->BindPipeline(blurPipelines[1]);
        commandBuffer->BindDescriptorSets(rhi::PipelineType::Compute, blurPipelineLayout, 0, 1, &storageImageDescriptorSets[2]);
        commandBuffer->Dispatch(width, blurGroupCountY, 1);
        commandBuffer->ResourceBarrier(storageImageBarriers[0]);

        // storage image 0 -> storage image 1
        
        commandBuffer->BindPipeline(bloomPipeline);
        commandBuffer->BindDescriptorSets(rhi::PipelineType::Compute, bloomPipelineLayout, 0, 1, &storageImageDescriptorSets[3]);
        commandBuffer->Dispatch(groupCountX, groupCountY, 1);
        commandBuffer->ResourceBarrier(storageToSampledImageBarrier);
    }

}