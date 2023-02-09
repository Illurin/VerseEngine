#pragma once

#include "base_pass.hpp"

#include "runtime/resource/asset_loader/image/image_loader.hpp"

namespace engine {

	struct DebugPassInfo {
		rhi::Device             device;
		ShaderCompiler*         shaderCompiler;

		rhi::Format             colorFormat;
		rhi::Extent2D           renderExtent;
		std::vector<rhi::Image> colorAttachments;
	};

	class DebugPass : public BasePass {
	public:
		DebugPass() {}
		explicit DebugPass(const DebugPassInfo& info) { Init(info); }

		void Init(const DebugPassInfo& info);
		void Destroy() override;

		void PreparePassData(rhi::CommandBuffer commandBuffer) override;
		void Draw(uint32_t frame, rhi::CommandBuffer commandBuffer) override;

	private:
		void BuildBuffers();
		void BuildSamplers();
		void BuildImages();
		void BuildDescriptorLayout();
		void BuildDescriptorSets();
		void BuildRenderPass();
		void BuildPipeline();
		void BuildFramebuffers();

	private:
		DebugPassInfo                   info{};
		bool                            passDataPrepared{ false };

	private:
		rhi::Buffer                     vertexBuffer{ nullptr };
		rhi::Buffer                     passUniformBuffer{ nullptr };
		rhi::Buffer                     uploaderBuffer{ nullptr };
		rhi::Image                      textureImage{ nullptr };
		rhi::Sampler                    sampler{ nullptr };

		rhi::DescriptorPool             descriptorPool{ nullptr };
		std::vector<rhi::DescriptorSet> descriptorSets;
		rhi::DescriptorSetLayout        descriptorSetLayout{ nullptr };
		rhi::PipelineLayout             pipelineLayout{ nullptr };

		rhi::RenderPass                 renderPass{ nullptr };
		rhi::Pipeline                   pipeline{ nullptr };
		std::vector<rhi::Framebuffer>   framebuffers;
	};

}