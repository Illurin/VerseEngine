#pragma once

#include "runtime/framework/system/render_system.hpp"

namespace engine {

	class IScene {
	public:
		virtual ~IScene();

		void Start();
		void Update();

		ecs::registry& GetRegistry() { return registry; }

		void SetCamera(Camera& camera) { this->camera = camera; }
		void AddSystem(std::unique_ptr<ISystem> pSystem) { systems.emplace_back(std::move(pSystem)); }

	private:
		Camera camera;
		ecs::registry registry;
		std::vector<std::unique_ptr<ISystem>> systems;
	};

	class MainScene final : public IScene {
	public:
		MainScene();
		~MainScene();

	private:
		rhi::Sampler sampler;
		rhi::Image image;
	};

}