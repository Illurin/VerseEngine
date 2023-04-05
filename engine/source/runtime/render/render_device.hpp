#pragma once

#include "interface/rhi_instance.hpp"

namespace engine {

	class RenderDevice {
	public:
		static void Initialize(const rhi::InstanceCreateInfo&);
		static void Clear();

		static void SetSwapchain(const rhi::SwapchainCreateInfo&);

		static rhi::Format GetSwapchainFormat() { return swapchainFormat; }

		static rhi::Instance GetInstance() { return instance; }
		static rhi::Device GetDevice() { return device; }
		static rhi::Queue GetQueue() { return queue; }
		static rhi::Swapchain GetSwapchain() { return swapchain; }

	private:
		static rhi::Format swapchainFormat;
		
		static rhi::Instance instance;
		static rhi::Device device;
		static rhi::Queue queue;
		static rhi::Swapchain swapchain;
	};

}