#include "raytracing_pass.hpp"

namespace engine {

    RayTracingPass::RayTracingPass() {
        BuildDescriptorLayout();
        BuildDescriptorSets();
        BuildPipeline();
        BuildShaderBindingTable();
    }

    RayTracingPass::~RayTracingPass() {
        sbtBuffer->Destroy();
        descriptorSetLayout->Destroy();
        pipeline->Destroy();
        pipelineLayout->Destroy();
    }

    void RayTracingPass::BuildDescriptorLayout() {
        std::vector<rhi::DescriptorSetLayoutBindingInfo> layoutBindings(2);

        layoutBindings[0] = rhi::DescriptorSetLayoutBindingInfo()
            .SetBinding(0)
            .SetDescriptorType(rhi::DescriptorType::AccelerationStructure)
            .SetShaderStage(rhi::ShaderStage::Raygen);
        layoutBindings[1] = rhi::DescriptorSetLayoutBindingInfo()
            .SetBinding(1)
            .SetDescriptorType(rhi::DescriptorType::StorageImage)
            .SetShaderStage(rhi::ShaderStage::Raygen);

        auto descriptorSetLayoutInfo = rhi::DescriptorSetLayoutCreateInfo()
            .SetBindings(layoutBindings);
        descriptorSetLayout = info.device->CreateDescriptorSetLayout(descriptorSetLayoutInfo);
    }

    void RayTracingPass::BuildDescriptorSets() {
        auto descriptorSetCount = static_cast<uint32_t>(info.colorAttachments.size());

        std::vector<rhi::DescriptorPoolSize> poolSizes = {
            { rhi::DescriptorType::AccelerationStructure, 1 },
            { rhi::DescriptorType::StorageImage, descriptorSetCount }
        };

        auto descriptorPoolInfo = rhi::DescriptorPoolCreateInfo()
            .SetMaxSetCount(descriptorSetCount)
            .SetPoolSizes(poolSizes);
        descriptorPool = info.device->CreateDescriptorPool(descriptorPoolInfo);

        std::array descriptorSetLayouts = {
            descriptorSetLayout, descriptorSetLayout
        };

        auto descriptorSetAllocateInfo = rhi::DescriptorSetAllocateInfo()
            .SetDescriptorSetCount(descriptorSetCount)
            .SetPDescriptorSetLayouts(descriptorSetLayouts.data());
        descriptorSets = descriptorPool->AllocateDescriptorSets(descriptorSetAllocateInfo);

        for (uint32_t i = 0; i < descriptorSetCount; i++) {
            auto imageViewInfo = rhi::ImageViewInfo()
                .SetImage(info.colorAttachments[i])
                .SetFormat(info.colorFormat)
                .SetViewType(rhi::ImageViewType::Image2D)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseMipLevel(0)
                    .SetMipLevelCount(1)
                    .SetBaseArrayLayer(0)
                    .SetArrayLayerCount(1));

            descriptorSets[i]->Write(0, rhi::DescriptorType::AccelerationStructure, topAS);
            descriptorSets[i]->Write(1, rhi::DescriptorType::StorageImage, rhi::ImageLayout::General, imageViewInfo);
        }
    }

    void RayTracingPass::BuildPipeline() {

        // Create pipeline layout

        auto pipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
            .SetDescriptorSetLayoutCount(1)
            .SetPDescriptorSetLayouts(&descriptorSetLayout);
        pipelineLayout = info.device->CreatePipelineLayout(pipelineLayoutInfo);

        // Create shader module

        std::vector<rhi::ShaderModule> shaderModules(4);
        rhi::ShaderModuleCreateInfo shaderModuleInfo;
        std::vector<uint8_t> sourceObject;

        info.shaderCompiler->SetSpirvRayTracing();

        info.shaderCompiler->Compile(Path::GetAbsolutePath("shader/raytracing/hlsl/raygen.hlsl"), L"main", ShaderCompileArgument::lib_profile);
        info.shaderCompiler->GetSourceObject(sourceObject);
        shaderModuleInfo.SetSourceCode(sourceObject)
                        .SetPEntryPoint("main")
                        .SetShaderStage(rhi::ShaderStage::Raygen);
        shaderModules[0] = info.device->CreateShaderModule(shaderModuleInfo);

        info.shaderCompiler->Compile(Path::GetAbsolutePath("shader/raytracing/hlsl/miss.hlsl"), L"main", ShaderCompileArgument::lib_profile);
        info.shaderCompiler->GetSourceObject(sourceObject);
        shaderModuleInfo.SetSourceCode(sourceObject)
            .SetShaderStage(rhi::ShaderStage::Miss);
        shaderModules[1] = info.device->CreateShaderModule(shaderModuleInfo);

        info.shaderCompiler->Compile(Path::GetAbsolutePath("shader/raytracing/hlsl/anyhit.hlsl"), L"main", ShaderCompileArgument::lib_profile);
        info.shaderCompiler->GetSourceObject(sourceObject);
        shaderModuleInfo.SetSourceCode(sourceObject)
                        .SetShaderStage(rhi::ShaderStage::AnyHit);
        shaderModules[2] = info.device->CreateShaderModule(shaderModuleInfo);

        info.shaderCompiler->Compile(Path::GetAbsolutePath("shader/raytracing/hlsl/closesthit.hlsl"), L"main", ShaderCompileArgument::lib_profile);
        info.shaderCompiler->GetSourceObject(sourceObject);
        shaderModuleInfo.SetSourceCode(sourceObject)
                        .SetShaderStage(rhi::ShaderStage::ClosestHit);
        shaderModules[3] = info.device->CreateShaderModule(shaderModuleInfo);

        auto shaderStageInfo = rhi::PipelineShaderStageInfo()
            .SetShaderModules(shaderModules);
        
        std::vector<rhi::RayTracingShaderGroupCreateInfo> shaderGroups(3);
        shaderGroups[0].SetType(rhi::RayTracingShaderGroupType::General)
                       .SetGeneralShader(0)
                       .SetAnyHitShader(rhi::shaderUnused)
                       .SetClosestHitShader(rhi::shaderUnused)
                       .SetIntersectionShader(rhi::shaderUnused);
        shaderGroups[1].SetType(rhi::RayTracingShaderGroupType::General)
                       .SetGeneralShader(1)
                       .SetAnyHitShader(rhi::shaderUnused)
                       .SetClosestHitShader(rhi::shaderUnused)
                       .SetIntersectionShader(rhi::shaderUnused);
        shaderGroups[2].SetType(rhi::RayTracingShaderGroupType::TrianglesHitGroup)
                       .SetGeneralShader(rhi::shaderUnused)
                       .SetAnyHitShader(2)
                       .SetClosestHitShader(3)
                       .SetIntersectionShader(rhi::shaderUnused);

        // Create pipeline

        auto pipelineInfo = rhi::RayTracingPipelineCreateInfo()
            .SetShaderStageInfo(shaderStageInfo)
            .SetShaderGroups(shaderGroups)
            .SetMaxRayRecursionDepth(10)
            .SetLayout(pipelineLayout);
        pipeline = info.device->CreateRayTracingPipeline(pipelineInfo);

        for (auto& shaderModule : shaderModules) {
            shaderModule->Destroy();
        }
    }

    void RayTracingPass::BuildShaderBindingTable() {
        auto shaderGroupHandles = info.device->GetRayTracingShaderGroupHandles(pipeline, 0, 3);
        auto requirement = info.device->GetRayTracingShaderGroupHandleRequirement();

        auto handleSizeAligned = math::UpperAlign(
            requirement.shaderGroupHandleSize,
            requirement.shaderGroupHandleAlignment);

        const uint32_t missGroupCount = 1;
        const uint32_t hitGroupCount = 1;

        raygenRegion = rhi::BufferStridedRegion()
            .SetSize(math::UpperAlign(handleSizeAligned, requirement.shaderGroupBaseAlignment))
            .SetStride(math::UpperAlign(handleSizeAligned, requirement.shaderGroupBaseAlignment))
            .SetOffset(0);
        missRegion = rhi::BufferStridedRegion()
            .SetSize(math::UpperAlign(missGroupCount * handleSizeAligned, requirement.shaderGroupBaseAlignment))
            .SetStride(handleSizeAligned)
            .SetOffset(raygenRegion.size);
        hitRegion = rhi::BufferStridedRegion()
            .SetSize(math::UpperAlign(hitGroupCount * handleSizeAligned, requirement.shaderGroupBaseAlignment))
            .SetStride(handleSizeAligned)
            .SetOffset(raygenRegion.size + missRegion.size);

        auto sbtBufferSize = raygenRegion.size + missRegion.size + hitRegion.size;
        auto sbtBufferInfo = rhi::BufferCreateInfo()
            .SetSize(sbtBufferSize)
            .SetUsage(rhi::BufferUsage::ShaderBindingTable);
        sbtBuffer = info.device->CreateBuffer(sbtBufferInfo);

        raygenRegion.SetBuffer(sbtBuffer);
        missRegion.SetBuffer(sbtBuffer);
        hitRegion.SetBuffer(sbtBuffer);

        auto pSrc = shaderGroupHandles.data();
        auto pDst = reinterpret_cast<uint8_t*>(sbtBuffer->Map());

        std::copy(pSrc, pSrc + requirement.shaderGroupHandleSize, pDst);
        pSrc += requirement.shaderGroupHandleSize;
        pDst += raygenRegion.size;

        for (uint32_t i = 0; i < missGroupCount; i++) {
            std::copy(pSrc, pSrc + requirement.shaderGroupHandleSize, pDst + handleSizeAligned * i);
            pSrc += requirement.shaderGroupHandleSize;
        }
        pDst += missRegion.size;

        for (uint32_t i = 0; i < hitGroupCount; i++) {
            std::copy(pSrc, pSrc + requirement.shaderGroupHandleSize, pDst + handleSizeAligned * i);
            pSrc += requirement.shaderGroupHandleSize;
        }

        sbtBuffer->Unmap();
    }

    void RayTracingPass::PreparePassData(rhi::CommandBuffer commandBuffer) {
        if (passDataPrepared)
            return;

        passDataPrepared = true;

        bottomASBuildGeometryInfo.SetDstAccelerationStructure(bottomAS)
            .SetScratchData(scratchBuffer);
        topASBuildGeometryInfo.SetDstAccelerationStructure(topAS)
            .SetScratchData(scratchBuffer);

        auto bottomASBuildRangeInfo = rhi::AccelerationStructureBuildRangeInfo()
            .SetFirstVertex(0)
            .SetPrimitiveCount(1);
        auto topASBuildRangeInfo = rhi::AccelerationStructureBuildRangeInfo()
            .SetFirstVertex(0)
            .SetPrimitiveCount(1);

        std::array<rhi::AccelerationStructureBuildRangeInfo*, 1> bottomRangeInfos = { &bottomASBuildRangeInfo };
        std::array<rhi::AccelerationStructureBuildRangeInfo*, 1> topRangeInfos = { &topASBuildRangeInfo };

        commandBuffer->BuildAccelerationStructures(1, &bottomASBuildGeometryInfo, bottomRangeInfos.data());
        commandBuffer->BuildAccelerationStructures(1, &topASBuildGeometryInfo, topRangeInfos.data());

        for (auto& image : info.colorAttachments) {
            auto imageMemoryBarrier = rhi::ImageMemoryBarrierInfo()
                .SetImage(image)
                .SetOldLayout(rhi::ImageLayout::Undefined)
                .SetNewLayout(rhi::ImageLayout::Present)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseMipLevel(0)
                    .SetMipLevelCount(1)
                    .SetBaseArrayLayer(0)
                    .SetArrayLayerCount(1));
            commandBuffer->ResourceBarrier(imageMemoryBarrier);
        }
    }

    void RayTracingPass::Begin(uint32_t frame, rhi::CommandBuffer commandBuffer) {
        PreparePassData(commandBuffer);

        auto imageMemoryBarrier = rhi::ImageMemoryBarrierInfo()
            .SetImage(info.colorAttachments[frame])
            .SetOldLayout(rhi::ImageLayout::Present)
            .SetNewLayout(rhi::ImageLayout::General)
            .SetSubresourceRange(rhi::ImageSubresourceRange()
                .SetImageAspect(rhi::ImageAspect::Color)
                .SetBaseMipLevel(0)
                .SetMipLevelCount(1)
                .SetBaseArrayLayer(0)
                .SetArrayLayerCount(1));
        commandBuffer->ResourceBarrier(imageMemoryBarrier);

        commandBuffer->BindPipeline(pipeline);
        commandBuffer->BindDescriptorSets(rhi::PipelineType::RayTracing, pipelineLayout, 0, 1, &descriptorSets[frame]);

        commandBuffer->TraceRays(raygenRegion, missRegion, hitRegion, rhi::BufferStridedRegion(),
                                 info.renderExtent.width, info.renderExtent.height, 1);

        imageMemoryBarrier.SetOldLayout(rhi::ImageLayout::General)
                          .SetNewLayout(rhi::ImageLayout::Present);
        commandBuffer->ResourceBarrier(imageMemoryBarrier);
    }

}