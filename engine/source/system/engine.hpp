#pragma once

#include "system/global_config.hpp"
#include "system/platform/platform_info.hpp"
#include "render/interface/vulkan/vulkan_rhi.hpp"

namespace engine {

	class Engine {
	public:
		explicit Engine(const char* configPath);
		~Engine() = default;

		void Start(const PlatformInfo& platformInfo);
		void Run();
		void Exit();

		double GetDeltaTime() const { return deltaTime; }
		double GetFPS() const { return fps; }

	private:
		void Tick();
		void LogicalTick();
		void RendererTick();

		void CalculateDeltaTime();
		void CalculateFPS();

	private:
		std::chrono::time_point<std::chrono::steady_clock> timePoint;
		double deltaTime{ 0.0f };
		double fps{ 0.0f };

	private:
		uint32_t renderWidth{ 0 };
		uint32_t renderHeight{ 0 };
		uint32_t frameCount{ 2 };

	private:
		rhi::Instance instance{ nullptr };
		rhi::Device device{ nullptr };
		rhi::Swapchain swapchain{ nullptr };
		rhi::Pipeline pipeline{ nullptr };
		rhi::CommandPool commandPool{ nullptr };
		std::vector<rhi::CommandBuffer> commandBuffers;
	};

}