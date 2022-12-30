#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // _WIN32

#include <array>
#include <vector>
#include <cstdint>

namespace engine {

	/*--------------------handle--------------------*/ 

	class RHIDevice {};
	class RHIQueue {};
	class RHISwapchain{};
	class RHIBuffer {};
	class RHIBufferView {};
	class RHIImage {};
	class RHIImageView {};
	class RHIGraphicsPipeline {};

	/*--------------------info--------------------*/

	struct RHIExtent2D {
		RHIExtent2D& SetWidth(uint32_t width) { this->width = width; return *this; }
		RHIExtent2D& SetHeight(uint32_t height) { this->height = height; return *this; }

		uint32_t width;
		uint32_t height;
	};

	struct RHIExtent3D {
		RHIExtent3D& SetWidth(uint32_t width) { this->width = width; return *this; }
		RHIExtent3D& SetHeight(uint32_t height) { this->height = height; return *this; }
		RHIExtent3D& SetDepth(uint32_t depth) { this->depth = depth; return *this; }

		uint32_t width;
		uint32_t height;
		uint32_t depth;
	};

	struct RHIInstanceInitInfo {
		RHIInstanceInitInfo& SetApplicationName(const char* applicationName) { this->applicationName = applicationName; return *this; }
		RHIInstanceInitInfo& SetApplicationVersion(uint32_t applicationVersion) { this->applicationVersion = applicationVersion; return *this; }

		const char* applicationName;
		uint32_t applicationVersion;
	};

	struct RHIPhysicalDeviceInfo {
		RHIPhysicalDeviceInfo& SetDeviceID(uint32_t deviceID) { this->deviceID = deviceID; return *this; }
		RHIPhysicalDeviceInfo& SetDeviceName(const char* deviceName) { this->deviceName = deviceName; return *this; }
		RHIPhysicalDeviceInfo& SetApiVersion(uint32_t apiVersion) { this->apiVersion = apiVersion; return *this; }

		uint32_t deviceID;
		const char* deviceName;
		uint32_t apiVersion;
	};
	
	struct RHIDeviceCreateInfo {
		RHIDeviceCreateInfo& SetPhysicalDeviceID(uint32_t physicalDeviceID) { this->physicalDeviceID = physicalDeviceID; return *this; }

		uint32_t physicalDeviceID;
	};

	struct RHIPlatformInfo {
#ifdef _WIN32
		RHIPlatformInfo& SetHInstance(HINSTANCE hInstance) { this->hInstance = hInstance; return *this; }
		RHIPlatformInfo& SetHWnd(HWND hWnd) { this->hWnd = hWnd; return *this; }

		HINSTANCE hInstance;
		HWND hWnd;
#endif // _WIN32
	};

	struct RHISwapchainCreateInfo {
		RHISwapchainCreateInfo& SetFormat(RHIFormat format) { this->format = format; return *this; }
		RHISwapchainCreateInfo& SetFrameCount(uint32_t frameCount) { this->frameCount = frameCount; return *this; }
		RHISwapchainCreateInfo& SetPlatformInfo(RHIPlatformInfo platformIsnfo) { this->platformInfo = platformInfo; return *this; }

		RHIFormat format;
		uint32_t frameCount;
		RHIExtent2D imageExtent;
		RHIPlatformInfo platformInfo;
	};

	struct RHIBufferCreateInfo {
		
	};

	struct RHIBufferViewCreateInfo {

	};

	struct RHIImageCreateInfo {
		
	};
	
	struct RHIImageViewCreateInfo {
		
	};

	/*--------------------enum--------------------*/

	enum class RHIFormat {
		Undefined,
		R8Unorm, R8G8Unorm, R8G8B8Unorm, R8G8B8A8Unorm,
		R8Snorm, R8G8Snorm, R8G8B8Snorm, R8G8B8A8Snorm,
		R8Uint, R8G8Uint, R8G8B8Uint, R8G8B8A8Uint,
		R8Sint, R8G8Sint, R8G8B8Sint, R8G8B8A8Sint,
		R16Unorm, R16G16Unorm, R16G16B16Unorm, R16G16B16A16Unorm,
		R16Snorm, R16G16Snorm, R16G16B16Snorm, R16G16B16A16Snorm,
		R16Uint, R16G16Uint, R16G16B16Uint, R16G16B16A16Uint,
		R16Sint, R16G16Sint, R16G16B16Sint, R16G16B16A16Sint,
		R16Sfloat, R16G16Sfloat, R16G16B16Sfloat, R16G16B16A16Sfloat,
		R32Uint, R32G32Uint, R32G32B32Uint, R32G32B32A32Uint,
		R32Sint, R32G32Sint, R32G32B32Sint, R32G32B32A32Sint,
		R32Sfloat, R32G32Sfloat, R32G32B32Sfloat, R32G32B32A32Sfloat,
		R64Uint, R64G64Uint, R64G64B64Uint, R64G64B64A64Uint,
		R64Sint, R64G64Sint, R64G64B64Sint, R64G64B64A64Sint,
		R64Sfloat, R64G64Sfloat, R64G64B64Sfloat, R64G64B64A64Sfloat,
		D16Unorm, D32Sfloat, S8Uint, D16UnormS8Uint, D24UnormS8Uint, D32SfloatS8Uint
	};

}