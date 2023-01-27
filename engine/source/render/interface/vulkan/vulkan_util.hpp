#pragma once

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#endif // _WIN32

#define VK_ENABLE_BETA_EXTENSIONS
#include "vulkan/vulkan.hpp"

#include <cstdlib>
#include <array>
#include <vector>
#include <stdexcept>
#include <fstream>

class VulkanUtil {
public:
	static bool MemoryTypeFromProperties(
		vk::PhysicalDeviceMemoryProperties memProp,
		uint32_t typeBits,
		vk::MemoryPropertyFlags requirementMask,
		uint32_t& typeIndex);

	static void CreateBuffer(
		vk::PhysicalDevice physicalDevice,
		vk::Device device,
		vk::DeviceSize size,
		vk::BufferUsageFlags usage,
		vk::Buffer& buffer,
		vk::DeviceMemory& memory);

	static void CreateImage(
		vk::PhysicalDevice physicalDevice,
		vk::Device device,
		uint32_t width,
		uint32_t height,
		uint32_t mipLevels,
		vk::Format format,
		vk::ImageUsageFlags usage,
		vk::Image& image,
		vk::DeviceMemory& memory);

	static vk::ShaderModule CreateShaderModule(vk::Device device, const char* path);

	static vk::CommandBuffer BeginSingleTimeCommand(vk::Device device, vk::CommandPool cmdPool);
	static void EndSingleTimeCommand(vk::Device device, vk::CommandPool cmdPool, vk::CommandBuffer cmd, vk::Queue queue);

	static bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);

#ifdef _DEBUG
	static VKAPI_ATTR vk::Bool32 VKAPI_CALL DebugCallback(
		vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		vk::DebugUtilsMessageTypeFlagsEXT messageType,
		const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {
#ifdef _WIN32
		OutputDebugStringA(pCallbackData->pMessage);
#endif // _WIN32
		return VK_FALSE;
	}
#endif // _DEBUG
};
