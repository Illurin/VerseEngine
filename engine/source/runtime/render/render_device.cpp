#include "render_device.hpp"

namespace engine {

	void RenderDevice::Initialize(const rhi::InstanceCreateInfo& instanceInfo) {
		instance = rhi::CreateInstance(instanceInfo);

		auto queueInfo = rhi::QueueCreateInfo()
			.SetQueueType(rhi::QueueType::Graphics);

		auto deviceInfo = rhi::DeviceCreateInfo()
			.SetPhysicalDeviceId(1)
			.SetPQueueCreateInfos(&queueInfo)
			.SetQueueCreateInfoCount(1)
			.SetDeviceExtension(rhi::DeviceExtension().SetRayTracingEnabled(false));

		device = instance->CreateDevice(deviceInfo);

		queue = device->GetQueue(0);
	}

	void RenderDevice::Clear() {
		if (swapchain) swapchain->Destroy();
		if (device) device->Destroy();
		if (instance) instance->Destroy();
	}

	void RenderDevice::SetSwapchain(const rhi::SwapchainCreateInfo& info) {
		if (info.format != rhi::Format::Undefined) swapchainFormat = info.format;

		auto swapchainInfo = info;
		swapchainInfo.format = swapchainFormat;

		if (!swapchain) swapchain = device->CreateSwapchain(swapchainInfo);
	}

}