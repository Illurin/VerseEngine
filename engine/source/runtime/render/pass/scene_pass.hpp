#pragma once

#include "base_pass.hpp"

namespace engine {

	class ScenePass {
	public:
		ScenePass();
		~ScenePass();

		void Begin(uint32_t frame, rhi::CommandBuffer commandBuffer);

		std::vector<rhi::DescriptorSetLayout> GetDescriptorSetLayouts() const { return descriptorSetLayouts; }
		rhi::PipelineLayout                   GetPipelineLayout()       const { return pipelineLayout; }
		rhi::RenderPass                       GetRenderPass()           const { return renderPass; }

	private:
		void BuildDescriptorLayout();
		void BuildRenderPass();
		void BuildPipeline();
		void BuildDepthImage();
		void BuildFramebuffers();

	private:
		std::vector<rhi::DescriptorSetLayout> descriptorSetLayouts;
		rhi::PipelineLayout                   pipelineLayout{ nullptr };
		rhi::RenderPass                       renderPass{ nullptr };
		rhi::Pipeline                         pipeline{ nullptr };
		rhi::Image                            depthImage{ nullptr };
		std::vector<rhi::Framebuffer>         framebuffers;
	};

}