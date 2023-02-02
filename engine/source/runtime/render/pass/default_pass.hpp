#pragma once

#include "runtime/render/interface/rhi.hpp"

namespace engine {

	class DefaultPass {
	public:
		DefaultPass();
		~DefaultPass();

		struct VertexData {

		};

		void PrepareRenderPass();
		void PreparePipeline();
		void PrepareFramebuffer();

		void Draw(rhi::CommandBuffer commandBuffer);

	private:
		rhi::RenderPass renderPass{ nullptr };
		rhi::Pipeline pipeline{ nullptr };
		rhi::Framebuffer framebuffer{ nullptr };
	};

}