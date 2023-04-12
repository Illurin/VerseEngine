#pragma once

#include "../base_pass.hpp"

namespace engine {

	struct RayTracingPassInfo {
		rhi::Device             device;
		ShaderCompiler*         shaderCompiler;

		rhi::Format             colorFormat;
		rhi::Extent2D           renderExtent;
		std::vector<rhi::Image> colorAttachments;
	};

	class RayTracingPass : public BasePass {
	public:
		RayTracingPass() {}
		explicit RayTracingPass(const RayTracingPassInfo& info) { Init(info); }

		void Init(const RayTracingPassInfo& info);
		void Destroy() override;

		virtual void PreparePassData(rhi::CommandBuffer commandBuffer);
		virtual void Begin(uint32_t frame, rhi::CommandBuffer commandBuffer) override;

	private:
		void BuildBuffers();
		void BuildAccelerationStructures();
		void BuildDescriptorLayout();
		void BuildDescriptorSets();
		void BuildPipeline();
		void BuildShaderBindingTable();

	private:
		RayTracingPassInfo                              info{};
		bool                                            passDataPrepared{ false };
													    
	private:									        
		rhi::Buffer                                     trianglesBuffer{ nullptr };
		rhi::Buffer                                     indexBuffer{ nullptr };
		rhi::Buffer                                     instancesBuffer{ nullptr };
		rhi::Buffer                                     bottomASBuffer{ nullptr };
		rhi::Buffer                                     topASBuffer{ nullptr };
		rhi::Buffer                                     scratchBuffer{ nullptr };
		rhi::Buffer                                     sbtBuffer{ nullptr };
													    
		std::vector<rhi::AccelerationStructureGeometry> bottomGeometries;
		std::vector<rhi::AccelerationStructureGeometry> topGeometries;
		rhi::AccelerationStructureBuildGeometryInfo     bottomASBuildGeometryInfo{};
		rhi::AccelerationStructureBuildGeometryInfo     topASBuildGeometryInfo{};
		rhi::AccelerationStructure                      bottomAS{ nullptr };
		rhi::AccelerationStructure                      topAS{ nullptr };
													    
		rhi::DescriptorPool                             descriptorPool;
		std::vector<rhi::DescriptorSet>                 descriptorSets;
		rhi::DescriptorSetLayout                        descriptorSetLayout{ nullptr };
		rhi::PipelineLayout                             pipelineLayout{ nullptr };

		rhi::Pipeline									pipeline{ nullptr };
		rhi::BufferStridedRegion                        raygenRegion;
		rhi::BufferStridedRegion                        missRegion;
		rhi::BufferStridedRegion                        hitRegion;
	};

}