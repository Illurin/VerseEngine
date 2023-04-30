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
        inline static rhi::Format swapchainFormat{ rhi::Format::B8G8R8A8Unorm };
        
        inline static rhi::Instance instance{ nullptr };
        inline static rhi::Device device{ nullptr };
        inline static rhi::Queue queue{ nullptr };
        inline static rhi::Swapchain swapchain{ nullptr };
    };

}