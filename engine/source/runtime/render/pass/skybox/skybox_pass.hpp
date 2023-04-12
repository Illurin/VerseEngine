#pragma once

#include "../base_pass.hpp"

namespace engine {

	class SkyboxPass {
	public:
		SkyboxPass(rhi::DescriptorSetLayout passDescriptorSetLayout, rhi::RenderPass renderPass);
		~SkyboxPass();

		rhi::PipelineLayout GetPipelineLayout() const { return pipelineLayout; }
		rhi::Pipeline       GetPipeline()       const { return pipeline; }

	private:
		void BuildPipeline(rhi::DescriptorSetLayout, rhi::RenderPass);

	private:
		rhi::PipelineLayout                   pipelineLayout{ nullptr };
		rhi::Pipeline                         pipeline{ nullptr };
	};

}