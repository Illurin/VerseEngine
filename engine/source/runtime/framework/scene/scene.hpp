#pragma once

#include "runtime/framework/system/system.hpp"

namespace engine {

	class IScene {
	public:
		virtual ~IScene();

		void Start();
		void Update();

		ecs::registry& GetRegistry() { return registry; }
		
		void AddSystem(std::unique_ptr<ISystem> pSystem) { systems.emplace_back(std::move(pSystem)); }

	private:
		ecs::registry registry;
		std::vector<std::unique_ptr<ISystem>> systems;
	};

	class MainScene final : public IScene {
	public:
		MainScene();
		~MainScene() {}
	};

}