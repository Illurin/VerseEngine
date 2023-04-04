#pragma once

#include "runtime/framework/system/render_system.hpp"

namespace engine {

	class Scene {
	public:
		void Start();
		void Update();
		void Clear();

		void AddSystem(ISystem* system) { systems.emplace_back(system); }

	private:
		ecs::registry registry;
		std::vector<ISystem*> systems;
	};

	class MainScene final : public Scene {
	public:
		MainScene();
	};

}