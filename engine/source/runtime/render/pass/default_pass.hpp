#pragma once

#include "base_pass.hpp"

namespace engine {

	struct DefaultVertexData {
		math::Vector3f worldPos;
		math::Vector3f color;
	};

	struct DefaultPassCreateInfo {
		rhi::Device device;
		rhi::Format colorFormat;
		rhi::Extent2D renderExtent;
		std::vector<rhi::Image> colorAttachments;
		uint32_t vertexCount;
		const DefaultVertexData* pVertexData;
		ShaderCompiler* shaderCompiler;
	};

	class DefaultPass {
	public:
		explicit DefaultPass(const DefaultPassCreateInfo&);
		~DefaultPass();

		void Draw(rhi::CommandBuffer commandBuffer, uint32_t imageIndex) const;

	private:
		void BuildBuffers();
		void BuildDescriptorLayout();
		void BuildDescriptorSets();
		void BuildRenderPass();
		void BuildPipeline();
		void BuildFramebuffers();

	private:
		rhi::Device device;
		rhi::Format colorFormat;
		rhi::Extent2D renderExtent;
		std::vector<rhi::Image> colorAttachments;
		uint32_t vertexCount;
		const DefaultVertexData* pVertexData;
		ShaderCompiler* shaderCompiler;

	private:
		rhi::Buffer vertexBuffer{ nullptr };
		rhi::DescriptorPool descriptorPool{ nullptr };
		rhi::DescriptorSetLayout descriptorSetLayout{ nullptr };
		rhi::RenderPass renderPass{ nullptr };
		rhi::Pipeline pipeline{ nullptr };
		std::vector<rhi::Framebuffer> framebuffers;
	};

}