#pragma once

#include "runtime/render/interface/rhi.hpp"
#include "runtime/render/render_data.hpp"
#include "runtime/math/math.hpp"

namespace engine {

	class BasePass {
	public:
		virtual void Destroy() = 0;

		virtual void PreparePassData(rhi::CommandBuffer commandBuffer) = 0;
		virtual void Draw(uint32_t frame, rhi::CommandBuffer commandBuffer) = 0;
	};

}