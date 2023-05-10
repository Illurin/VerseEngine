#include "scene.hpp"

#include "runtime/framework/component/scene_component.hpp"
#include "runtime/framework/system/render_system.hpp"

namespace engine {

    IScene::IScene() {

        // Systems

        AddSystem<RenderSystem>();
    }

    void IScene::Start() {
        for (auto& system : systems) {
            system->Start();
        }
    }

    void IScene::Update() {
        for (auto& system : systems) {
            system->Update();
        }
    }

    ecs::registry& IScene::GetRegistry() {
        return registry;
    }

    Entity IScene::CreateEntity() {
        return Entity(registry);
    }

}