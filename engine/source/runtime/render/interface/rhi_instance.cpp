#include "rhi_instance.hpp"

namespace engine {
	namespace rhi {

		Instance CreateInstance(const InstanceCreateInfo& info) {
			Instance instance{ nullptr };
			std::string apiName = info.pApiName;

			if (apiName == "Vulkan") {
#ifdef _VULKAN
				instance = new VkWrapperInstance();
#endif // _VULKAN
			}
			else if (apiName == "Direct3D12") {
#ifdef _DIRECT3D12
				instance = new D3D12WrapperInstance();
#endif // _DIRECT3D12
			}

			if (!instance) {
				throw std::runtime_error("Cannot find graphics API backend");
			}
			instance->Init(info);
			return instance;
		}

	}
}