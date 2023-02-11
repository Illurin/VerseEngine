#include "vulkan_util.hpp"

#ifdef _VULKAN

bool VulkanUtil::MemoryTypeFromProperties(
	vk::PhysicalDeviceMemoryProperties memProp,
	uint32_t typeBits, vk::MemoryPropertyFlags requirementMask,
	uint32_t& typeIndex) {
	for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
		if ((typeBits & (1 << i)) && (memProp.memoryTypes[i].propertyFlags & requirementMask) == requirementMask) {
			typeIndex = i;
			return true;
		}
	}
	return false;
}

bool VulkanUtil::CheckValidationLayerSupport(const std::vector<const char*>& validationLayers) {
	uint32_t layerCount = 0;
	auto result = vk::enumerateInstanceLayerProperties(&layerCount, static_cast<vk::LayerProperties*>(nullptr));

	if (layerCount == 0)
		return false;

	std::vector<vk::LayerProperties> availableLayers(layerCount);
	result = vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}
	return true;
}

#endif // _VULKAN