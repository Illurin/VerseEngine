#include "scene.hpp"

namespace engine {

	void Scene::Start() {
		for (auto& system : systems) {
			system->Start(registry);
		}
	}

	void Scene::Update() {
		for (auto& system : systems) {
			system->Update(registry);
		}
	}

	void Scene::Clear() {
		auto transformView = registry.view<TransformComponent>();

		transformView.each([](TransformComponent& transform) {
			if (transform.uniformBuffer) transform.uniformBuffer->Destroy();
		});
	}

	MainScene::MainScene() {

	}

}