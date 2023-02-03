#pragma once

#include "runtime/system/global_config.hpp"
#include "runtime/system/platform/platform_info.hpp"
#include "runtime/render/interface/rhi_instance.hpp"
#include "runtime/math/math.hpp"
#include "runtime/resource/asset_loader/image/image_loader.hpp"
#include "runtime/resource/asset_loader/model/model_loader.hpp"

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
		rhi::Extent2D renderExtent{ 0, 0 };
		rhi::Format swapchainFormat{ rhi::Format::B8G8R8A8Unorm };
		uint32_t frameCount{ 2 };

	private:
		rhi::Instance instance{ nullptr };
		rhi::Device device{ nullptr };
		rhi::Queue queue{ nullptr };
		rhi::Swapchain swapchain{ nullptr };
		rhi::RenderPass renderPass{ nullptr };
		rhi::Pipeline pipeline{ nullptr };
		rhi::CommandPool commandPool{ nullptr };
		std::vector<rhi::CommandBuffer> commandBuffers;
		std::vector<rhi::Framebuffer> framebuffers;
		rhi::Fence fence{ nullptr };

	private:
		rhi::Buffer vertexBuffer{ nullptr };
	};

}