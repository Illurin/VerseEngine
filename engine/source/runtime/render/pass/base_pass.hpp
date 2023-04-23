#pragma once

#include "runtime/render/render_device.hpp"
#include "runtime/render/render_data.hpp"
#include "runtime/math/math.hpp"

namespace engine {

    class BasePass {
    public:
        virtual void Destroy() = 0;
        virtual void Begin(uint32_t frame, rhi::CommandBuffer commandBuffer) = 0;
    };

    class IRenderPass {
    public:
        virtual ~IRenderPass() { }
    };

}