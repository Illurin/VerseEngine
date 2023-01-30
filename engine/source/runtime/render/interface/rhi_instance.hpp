#pragma once

#include "runtime/render/interface/d3d12/d3d12_rhi.hpp"
#include "runtime/render/interface/vulkan/vulkan_rhi.hpp"

namespace engine {
	namespace rhi {

		Instance CreateInstance(const InstanceCreateInfo&);

	}
}