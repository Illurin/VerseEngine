#include "screen_quad_pass.hpp"

namespace engine {

    ScreenQuadPass::ScreenQuadPass(rhi::Image shaderReadImage) {
        BuildSampler();
        BuildDescriptorLayout();
        BuildDescriptors(shaderReadImage);
        BuildRenderPass();
        BuildPipeline();
        BuildFramebuffers();
    }

    ScreenQuadPass::~ScreenQuadPass() {
        if (sampler) sampler->Destroy();
        if (descriptorSetLayout) descriptorSetLayout->Destroy();
        if (descriptorPool) descriptorPool->Destroy();
        if (pipelineLayout) pipelineLayout->Destroy();
        if (renderPass) renderPass->Destroy();
        if (pipeline) pipeline->Destroy();
        for (auto framebuffer : framebuffers) {
            framebuffer->Destroy();
        }
    }

    void ScreenQuadPass::BuildSampler() {
        auto samplerInfo = rhi::SamplerCreateInfo()
            .SetMinFilter(rhi::Filter::Linear)
            .SetMagFilter(rhi::Filter::Linear)
            .SetMipFilter(rhi::Filter::Linear)
            .SetAddressModeU(rhi::SamplerAddressMode::Repeat)
            .SetAddressModeV(rhi::SamplerAddressMode::Repeat)
            .SetAddressModeW(rhi::SamplerAddressMode::Repeat)
            .SetMinLod(0.0f)
            .SetMaxLod(1.0f);
        sampler = RenderDevice::GetDevice()->CreateSampler(samplerInfo);
    }

    void ScreenQuadPass::BuildDescriptorLayout() {
        std::vector<rhi::DescriptorSetLayoutBindingInfo> layoutBindings(2);

        layoutBindings[0].SetBinding(0)
                         .SetDescriptorType(rhi::DescriptorType::SampledImage)
                         .SetShaderStage(rhi::ShaderStage::Fragment);
        
        layoutBindings[1].SetBinding(1)
                         .SetDescriptorType(rhi::DescriptorType::Sampler)
                         .SetShaderStage(rhi::ShaderStage::Fragment);

        auto descriptorSetLayoutInfo = rhi::DescriptorSetLayoutCreateInfo()
            .SetBindings(layoutBindings);
        descriptorSetLayout = RenderDevice::GetDevice()->CreateDescriptorSetLayout(descriptorSetLayoutInfo);
    }

    void ScreenQuadPass::BuildDescriptors(rhi::Image shaderReadImage) {
        std::vector<rhi::DescriptorPoolSize> poolSizes = {
            { rhi::DescriptorType::SampledImage, 1 },
            { rhi::DescriptorType::Sampler, 1 }
        };

        auto descriptorPoolInfo = rhi::DescriptorPoolCreateInfo()
            .SetMaxSetCount(1)
            .SetPoolSizes(poolSizes);
        descriptorPool = RenderDevice::GetDevice()->CreateDescriptorPool(descriptorPoolInfo);

        auto descriptorSetAllocateInfo = rhi::DescriptorSetAllocateInfo()
            .SetDescriptorSetCount(1)
            .SetPDescriptorSetLayouts(&descriptorSetLayout);
        descriptorSet = descriptorPool->AllocateDescriptorSets(descriptorSetAllocateInfo)[0];

        auto shaderReadView = rhi::ImageViewInfo()
            .SetImage(shaderReadImage)
            .SetFormat(rhi::Format::R16G16B16A16Sfloat)
            .SetViewType(rhi::ImageViewType::Image2D)
            .SetSubresourceRange(rhi::ImageSubresourceRange()
                .SetImageAspect(rhi::ImageAspect::Color)
                .SetBaseMipLevel(0)
                .SetMipLevelCount(1)
                .SetBaseArrayLayer(0)
                .SetArrayLayerCount(1));

        descriptorSet->Write(0, rhi::DescriptorType::SampledImage, rhi::ImageLayout::ShaderReadOnly, shaderReadView);
        descriptorSet->Write(1, rhi::DescriptorType::Sampler, sampler);
    }

    void ScreenQuadPass::BuildRenderPass() {
        auto colorAttachmentDescription = rhi::AttachmentDescription()
            .SetFormat(RenderDevice::GetSwapchainFormat())
            .SetSampleCount(rhi::SampleCount::Count1)
            .SetLoadOp(rhi::AttachmentLoadOp::Clear)
            .SetStoreOp(rhi::AttachmentStoreOp::Store)
            .SetInitialLayout(rhi::ImageLayout::Undefined)
            .SetPassLayout(rhi::ImageLayout::ColorAttachment)
            .SetFinalLayout(rhi::ImageLayout::Present);

        auto renderPassInfo = rhi::RenderPassCreateInfo()
            .SetPipelineType(rhi::PipelineType::Graphics)
            .SetColorAttachmentCount(1)
            .SetPColorAttachments(&colorAttachmentDescription);

        renderPass = RenderDevice::GetDevice()->CreateRenderPass(renderPassInfo);
    }

    void ScreenQuadPass::BuildPipeline() {

        // Create pipeline layout

        auto pipelineLayoutInfo = rhi::PipelineLayoutCreateInfo()
            .SetDescriptorSetLayoutCount(1)
            .SetPDescriptorSetLayouts(&descriptorSetLayout);
        pipelineLayout = RenderDevice::GetDevice()->CreatePipelineLayout(pipelineLayoutInfo);

        // Compile shaders

        std::vector<uint8_t> shaderSource;

        auto shaderCompiler = RenderDevice::GetInstance()->CreateShaderCompiler();

        shaderCompiler.Compile(Path::GetAbsolutePath("shader/screen_quad/hlsl/vertex.hlsl"), L"main", ShaderCompileArgument::vs_profile);
        shaderCompiler.GetSourceObject(shaderSource);

        auto shaderModuleInfo = rhi::ShaderModuleCreateInfo()
            .SetSourceCode(shaderSource)
            .SetPEntryPoint("main")
            .SetShaderStage(rhi::ShaderStage::Vertex);
        auto vertexShaderModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

        shaderCompiler.Compile(Path::GetAbsolutePath("shader/screen_quad/hlsl/fragment.hlsl"), L"main", ShaderCompileArgument::ps_profile);
        shaderCompiler.GetSourceObject(shaderSource);

        shaderModuleInfo = rhi::ShaderModuleCreateInfo()
            .SetSourceCode(shaderSource)
            .SetPEntryPoint("main")
            .SetShaderStage(rhi::ShaderStage::Fragment);
        auto fragmentShaderModule = RenderDevice::GetDevice()->CreateShaderModule(shaderModuleInfo);

        // Pipeline shader stages

        std::array<rhi::ShaderModule, 2> shaderModules = { vertexShaderModule, fragmentShaderModule };

        auto shaderStageInfo = rhi::PipelineShaderStageInfo()
            .SetShaderModuleCount(static_cast<uint32_t>(shaderModules.size())).SetPShaderModules(shaderModules.data());

        // Pipeline input assembly

        auto inputAssemblyInfo = rhi::PipelineInputAssemblyInfo()
            .SetTopology(rhi::PrimitiveTopology::TriangleStrip)
            .SetPrimitivieRestart(false);

        // Pipeline rasterization

        auto rasterizationInfo = rhi::PipelineRasterizationInfo()
            .SetSmoothLine(true)
            .SetPolygonMode(rhi::PolygonMode::Solid)
            .SetCullMode(rhi::CullMode::None)
            .SetDepthClamp(false)
            .SetDepthBias(0.0f)
            .SetSlopeScaledDepthBias(0.0f);

        // Pipeline scissor & viewport

        auto scissor = rhi::Rect2D()
            .SetExtent(RenderDevice::GetSwapchain()->GetImageExtent())
            .SetOffset(rhi::Offset2D().SetX(0).SetY(0));

        auto viewport = rhi::Viewport()
            .SetWidth(static_cast<float>(RenderDevice::GetSwapchain()->GetImageExtent().width)).SetHeight(static_cast<float>(RenderDevice::GetSwapchain()->GetImageExtent().height))
            .SetX(0.0f).SetY(0.0f)
            .SetMinDepth(0.0f).SetMaxDepth(0.0f);

        auto viewportInfo = rhi::PipelineViewportInfo()
            .SetPScissors(&scissor).SetScissorCount(1)
            .SetPViewports(&viewport).SetViewportCount(1);

        // Pipeline color blend

        auto colorAttachmentInfo = rhi::ColorBlendAttachmentInfo()
            .SetBlendEnable(false);

        auto colorBlendInfo = rhi::PipelineColorBlendInfo()
            .SetPAttachments(&colorAttachmentInfo).SetAttachmentCount(1);

        // Create pipeline

        auto pipelineInfo = rhi::GraphicsPipelineCreateInfo()
            .SetShaderStageInfo(shaderStageInfo)
            .SetInputAssemblyInfo(inputAssemblyInfo)
            .SetRasterizationInfo(rasterizationInfo)
            .SetViewportInfo(viewportInfo)
            .SetColorBlendInfo(colorBlendInfo)
            .SetLayout(pipelineLayout)
            .SetRenderPass(renderPass);

        pipeline = RenderDevice::GetDevice()->CreateGraphicsPipeline(pipelineInfo);

        vertexShaderModule->Destroy();
        fragmentShaderModule->Destroy();
    }

    void ScreenQuadPass::BuildFramebuffers() {
        auto colorAttachments = RenderDevice::GetSwapchain()->GetImages();

        for (auto& colorAttachment : colorAttachments) {
            auto colorImageViewInfo = rhi::ImageViewInfo()
                .SetImage(colorAttachment)
                .SetFormat(RenderDevice::GetSwapchainFormat())
                .SetViewType(rhi::ImageViewType::Image2D)
                .SetSubresourceRange(rhi::ImageSubresourceRange()
                    .SetImageAspect(rhi::ImageAspect::Color)
                    .SetBaseMipLevel(0)
                    .SetMipLevelCount(1)
                    .SetBaseArrayLayer(0)
                    .SetArrayLayerCount(1));

            auto framebufferInfo = rhi::FramebufferCreateInfo()
                .SetWidth(RenderDevice::GetSwapchain()->GetImageExtent().width)
                .SetHeight(RenderDevice::GetSwapchain()->GetImageExtent().height)
                .SetLayers(1)
                .SetColorAttachmentCount(1)
                .SetPColorAttachments(&colorImageViewInfo)
                .SetRenderPass(renderPass);

            framebuffers.emplace_back(RenderDevice::GetDevice()->CreateFramebuffer(framebufferInfo));
        }
    }

    void ScreenQuadPass::Execute(uint32_t frame, rhi::CommandBuffer commandBuffer) {
        auto clearColorValue = rhi::ClearColorValue().SetColorValue(std::array<float, 4>({ 0.0f, 0.0f, 0.0f, 0.0f }));

        auto renderPassBeginInfo = rhi::RenderPassBeginInfo()
            .SetRenderPass(renderPass)
            .SetFramebuffer(framebuffers[frame])
            .SetPClearColorValues(&clearColorValue);
        commandBuffer->BeginRenderPass(renderPassBeginInfo);

        commandBuffer->BindPipeline(pipeline);
        commandBuffer->BindDescriptorSets(rhi::PipelineType::Graphics, pipelineLayout, 0, 1, &descriptorSet);

        commandBuffer->Draw(4, 1, 0, 0);

        commandBuffer->EndRenderPass();
    }

}