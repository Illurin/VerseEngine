#pragma once

#include "vulkan_util.hpp"

namespace engine {

	class VkWrapperDevice final : public RHIDevice {
	public:
		VkWrapperDevice& SetPhysicalDevice(vk::PhysicalDevice physicalDevice) { this->physicalDevice = physicalDevice; return *this; }
		VkWrapperDevice& SetDevice(vk::Device device) { this->device = device; return *this; }
		VkWrapperDevice& SetQueueFamilyIndex(uint32_t queueFamilyIndex) { this->queueFamilyIndex = queueFamilyIndex; return *this; }

		vk::PhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
		vk::Device GetDevice() const { return device; }
		uint32_t GetQueueFamilyIndex() const { return queueFamilyIndex; }

	private:
		vk::PhysicalDevice physicalDevice;
		vk::Device device;
		uint32_t queueFamilyIndex;
	};

	class VkWrapperSwapchain final : public RHISwapchain {
	public:
		VkWrapperSwapchain& SetSurface(vk::SurfaceKHR surface) { this->surface = surface; return *this; }
		VkWrapperSwapchain& SetSwapchain(vk::SwapchainKHR swapchain) { this->swapchain = swapchain; return *this; }

		vk::SurfaceKHR GetSurface() const { return surface; }
		vk::SwapchainKHR GetSwapchain() const { return swapchain; }

	private:
		vk::SurfaceKHR surface;
		vk::SwapchainKHR swapchain;
	};

	class VkEnumFormat {
	public:
		VkEnumFormat(RHIFormat format) {
			switch (format) {
			case RHIFormat::Undefined: format_ = vk::Format::eUndefined; break;
			case RHIFormat::R8Unorm: format_ = vk::Format::eR8Unorm; break;
			case RHIFormat::R8G8Unorm: format_ = vk::Format::eR8G8Unorm; break;
			case RHIFormat::R8G8B8Unorm: format_ = vk::Format::eR8G8B8Unorm; break;
			case RHIFormat::R8G8B8A8Unorm: format_ = vk::Format::eR8G8B8A8Unorm; break;
			case RHIFormat::R8Snorm: format_ = vk::Format::eR8Snorm; break;
			case RHIFormat::R8G8Snorm: format_ = vk::Format::eR8G8Snorm; break;
			case RHIFormat::R8G8B8Snorm: format_ = vk::Format::eR8G8B8Snorm; break;
			case RHIFormat::R8G8B8A8Snorm: format_ = vk::Format::eR8G8B8A8Snorm; break;
			case RHIFormat::R8Uint: format_ = vk::Format::eR8Uint; break;
			case RHIFormat::R8G8Uint: format_ = vk::Format::eR8G8Uint; break;
			case RHIFormat::R8G8B8Uint: format_ = vk::Format::eR8G8B8Uint; break;
			case RHIFormat::R8G8B8A8Uint: format_ = vk::Format::eR8G8B8A8Uint; break;
			case RHIFormat::R8Sint: format_ = vk::Format::eR8Sint; break;
			case RHIFormat::R8G8Sint: format_ = vk::Format::eR8G8Sint; break;
			case RHIFormat::R8G8B8Sint: format_ = vk::Format::eR8G8B8Sint; break;
			case RHIFormat::R8G8B8A8Sint: format_ = vk::Format::eR8G8B8A8Sint; break;
			case RHIFormat::R16Unorm: format_ = vk::Format::eR16Unorm; break;
			case RHIFormat::R16G16Unorm: format_ = vk::Format::eR16G16Unorm; break;
			case RHIFormat::R16G16B16Unorm: format_ = vk::Format::eR16G16B16Unorm; break;
			case RHIFormat::R16G16B16A16Unorm: format_ = vk::Format::eR16G16B16A16Unorm; break;
			case RHIFormat::R16Snorm: format_ = vk::Format::eR16Snorm; break;
			case RHIFormat::R16G16Snorm: format_ = vk::Format::eR16G16Snorm; break;
			case RHIFormat::R16G16B16Snorm: format_ = vk::Format::eR16G16B16Snorm; break;
			case RHIFormat::R16G16B16A16Snorm: format_ = vk::Format::eR16G16B16A16Snorm; break;
			case RHIFormat::R16Uint: format_ = vk::Format::eR16Uint; break;
			case RHIFormat::R16G16Uint: format_ = vk::Format::eR16G16Uint; break;
			case RHIFormat::R16G16B16Uint: format_ = vk::Format::eR16G16B16Uint; break;
			case RHIFormat::R16G16B16A16Uint: format_ = vk::Format::eR16G16B16A16Uint; break;
			case RHIFormat::R16Sint: format_ = vk::Format::eR16Sint; break;
			case RHIFormat::R16G16Sint: format_ = vk::Format::eR16G16Sint; break;
			case RHIFormat::R16G16B16Sint: format_ = vk::Format::eR16G16B16Sint; break;
			case RHIFormat::R16G16B16A16Sint: format_ = vk::Format::eR16G16B16A16Sint; break;
			case RHIFormat::R16Sfloat: format_ = vk::Format::eR16Sfloat; break;
			case RHIFormat::R16G16Sfloat: format_ = vk::Format::eR16G16Sfloat; break;
			case RHIFormat::R16G16B16Sfloat: format_ = vk::Format::eR16G16B16Sfloat; break;
			case RHIFormat::R16G16B16A16Sfloat: format_ = vk::Format::eR16G16B16A16Sfloat; break;
			case RHIFormat::R32Uint: format_ = vk::Format::eR32Uint; break;
			case RHIFormat::R32G32Uint: format_ = vk::Format::eR32G32Uint; break;
			case RHIFormat::R32G32B32Uint: format_ = vk::Format::eR32G32B32Uint; break;
			case RHIFormat::R32G32B32A32Uint: format_ = vk::Format::eR32G32B32A32Uint; break;
			case RHIFormat::R32Sint: format_ = vk::Format::eR32Sint; break;
			case RHIFormat::R32G32Sint: format_ = vk::Format::eR32G32Sint; break;
			case RHIFormat::R32G32B32Sint: format_ = vk::Format::eR32G32B32Sint; break;
			case RHIFormat::R32G32B32A32Sint: format_ = vk::Format::eR32G32B32A32Sint; break;
			case RHIFormat::R32Sfloat: format_ = vk::Format::eR32Sfloat; break;
			case RHIFormat::R32G32Sfloat: format_ = vk::Format::eR32G32Sfloat; break;
			case RHIFormat::R32G32B32Sfloat: format_ = vk::Format::eR32G32B32Sfloat; break;
			case RHIFormat::R32G32B32A32Sfloat: format_ = vk::Format::eR32G32B32A32Sfloat; break;
			case RHIFormat::R64Uint: format_ = vk::Format::eR64Uint; break;
			case RHIFormat::R64G64Uint: format_ = vk::Format::eR64G64Uint; break;
			case RHIFormat::R64G64B64Uint: format_ = vk::Format::eR64G64B64Uint; break;
			case RHIFormat::R64G64B64A64Uint: format_ = vk::Format::eR64G64B64A64Uint; break;
			case RHIFormat::R64Sint: format_ = vk::Format::eR64Sint; break;
			case RHIFormat::R64G64Sint: format_ = vk::Format::eR64G64Sint; break;
			case RHIFormat::R64G64B64Sint: format_ = vk::Format::eR64G64B64Sint; break;
			case RHIFormat::R64G64B64A64Sint: format_ = vk::Format::eR64G64B64A64Sint; break;
			case RHIFormat::R64Sfloat: format_ = vk::Format::eR64Sfloat; break;
			case RHIFormat::R64G64Sfloat: format_ = vk::Format::eR64G64Sfloat; break;
			case RHIFormat::R64G64B64Sfloat: format_ = vk::Format::eR64G64B64Sfloat; break;
			case RHIFormat::R64G64B64A64Sfloat: format_ = vk::Format::eR64G64B64A64Sfloat; break;
			case RHIFormat::D16Unorm: format_ = vk::Format::eD16Unorm; break;
			case RHIFormat::D32Sfloat: format_ = vk::Format::eD32Sfloat; break;
			case RHIFormat::S8Uint: format_ = vk::Format::eS8Uint; break;
			case RHIFormat::D16UnormS8Uint: format_ = vk::Format::eD16UnormS8Uint; break;
			case RHIFormat::D24UnormS8Uint: format_ = vk::Format::eD24UnormS8Uint; break;
			case RHIFormat::D32SfloatS8Uint: format_ = vk::Format::eD32SfloatS8Uint; break;
			}
		}

		vk::Format Get() const { return format_; }

	private:
		vk::Format format_{ vk::Format::eUndefined };
	};

}