#include "scene.hpp"

#include "runtime/framework/system/resource_system.hpp"
#include "runtime/framework/system/render_system.hpp"

namespace engine {

    IScene::IScene() {

        // Systems

        AddSystem<ResourceSystem>();
        AddSystem<RenderSystem>();

        // Environment

        Entity::AddSingletonComponent<EnvironmentComponent>() = 
            EnvironmentComponent(math::Vector3f(0.01f, 0.01f, 0.01f),
                                 Path::GetAbsolutePath("asset/skybox.png"));

        // Camera

        /*CameraComponent cameraComponent(true, Camera());

        auto cameraEntity = CreateEntity();
        cameraEntity.AddComponent<CameraComponent>(cameraComponent);
        cameraEntity.AddComponent<ControllerComponent>(2.0f, 2.0f);*/
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