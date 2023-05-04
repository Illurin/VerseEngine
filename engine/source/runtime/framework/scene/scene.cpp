#include "scene.hpp"

#include "runtime/framework/system/render_system.hpp"

namespace engine {

    IScene::IScene() {

        // Systems

        AddSystem<RenderSystem>();

        // Environment

        auto& environmanet = Entity::AddSingletonComponent<EnvironmentComponent>();
        environmanet.data.ambientLight = math::Vector3f(1.0f, 1.0f, 1.0f);
        environmanet.data.skyboxTexturePath = Path::GetAbsolutePath("asset/default/cubemap.jpg");
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