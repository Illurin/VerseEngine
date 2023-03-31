#pragma once

#include "runtime/system/global_config.hpp"
#include "runtime/system/time.hpp"
#include "runtime/system/platform/platform_info.hpp"
#include "runtime/system/platform/input_system.hpp"
#include "runtime/render/interface/rhi_instance.hpp"
#include "runtime/math/math.hpp"
#include "runtime/resource/asset_loader/image/image_loader.hpp"
#include "runtime/render/render_pass.hpp"

namespace engine {

	class Engine {
	public:
		explicit Engine(const char* configPath);
		~Engine() = default;

		void Start(const PlatformInfo& platformInfo);
		void Run();
		void Exit();

	private:
		void Tick();
		void LogicalTick();
		void RendererTick();

		void CalculateDeltaTime();
		void CalculateFps();

	private:
		std::chrono::time_point<std::chrono::steady_clock> timePoint;

	private:
		rhi::Extent2D renderExtent{ 0, 0 };
		rhi::Format swapchainFormat{ rhi::Format::B8G8R8A8Unorm };
		uint32_t frameCount{ 2 };

	private:
		rhi::Instance instance{ nullptr };
		rhi::Device device{ nullptr };
		rhi::Queue queue{ nullptr };
		rhi::Swapchain swapchain{ nullptr };
		rhi::CommandPool commandPool{ nullptr };
		std::vector<rhi::CommandBuffer> commandBuffers;
		rhi::Fence fence{ nullptr };

	private:
		DebugPass defaultPass;
		RayTracingPass rayTracingPass;
	};

}