#include "runtime/framework/scene/scene_manager.hpp"
#include "runtime/resource/resource_manager.hpp"
#include "runtime/render/geometry.hpp"
#include "runtime/system/platform/input_system.hpp"
#include "runtime/system/time.hpp"
#include "runtime/resource/asset_loader/model/model_loader.hpp"

using namespace engine;

struct PlayerControlComponent : IComponent {
    float velocity{ 10.0f };
};

class PlayerControlSystem : public ISystem {
public:
    void Update() override {
        for (auto [entity, transform, playerControl] : GetEntities<TransformComponent, PlayerControlComponent>().each()) {
            auto movement = playerControl.velocity * static_cast<float>(Time::GetDeltaTime());

            if (input::GetKey(input::KeyCode::D)) transform.data.position.x() += movement;
            if (input::GetKey(input::KeyCode::A)) transform.data.position.x() -= movement;
            if (input::GetKey(input::KeyCode::W)) transform.data.position.y() += movement;
            if (input::GetKey(input::KeyCode::S)) transform.data.position.y() -= movement;

            transform.dirty = true;
        }
    }
};

class DemoScene : public IScene {
public:
    DemoScene() {

        AddSystem<PlayerControlSystem>();
        
        // Ambient light and sky box

        Entity::GetSingletonComponents<EnvironmentComponent>() =
            EnvironmentComponent({ math::Vector3f(0.1f, 0.1f, 0.1f),
                                   Path::GetAbsolutePath("asset/skybox.png") });

        // Set camera properties

        Camera camera;
        camera.SetPosition(math::Vector3f(0.0f, 0.0f, 0.0f));
        camera.LookAt(math::Vector3f(0.0f, 0.0f, -1.0f));

        // Create camera Entity

        auto cameraEntity = CreateEntity();
        cameraEntity.AddComponent<CameraComponent>(camera);

        // Create new object

        auto objectEntity = CreateEntity();

        // Add transform component (translation, scaling, rotation)

        objectEntity.AddComponent<TransformComponent>(TransformData({ math::Vector3f(0.0f, 0.0f, -10.0f),
                                                                      math::Vector3f(1.0f, 1.0f, 1.0f),
                                                                      math::Vector3f::Zero(),
                                                                      math::Vector3f::Zero() })); 

        // Add mesh renderer component

        auto geo = Geometry::CreateGeosphere(1.0f, 5);
        objectEntity.AddComponent<MeshRendererComponent>(MeshRendererData({ geo.vertices, geo.indices }));

        // Add material

        // Sampler properties
        auto objectSamplerInfo = rhi::SamplerCreateInfo()
            .SetMinFilter(rhi::Filter::Linear)
            .SetMagFilter(rhi::Filter::Linear)
            .SetMipFilter(rhi::Filter::Linear);

        // Material properties
        MaterialData objectMaterialData;
        objectMaterialData.diffuseAlbedo = math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        objectMaterialData.fresnelR0 = math::Vector3f(0.3f, 0.3f, 0.3f);
        objectMaterialData.roughness = 0.5f;
        objectMaterialData.diffuseMap = Path::GetAbsolutePath("asset/brickTexture.jpg");
        objectMaterialData.normalMap = Path::GetAbsolutePath("asset/default/normal.jpg");

        // Add material component
        objectEntity.AddComponent<MaterialComponent>(ResourceManager::CreateMaterial(objectMaterialData));

        // Player control component
        objectEntity.AddComponent<PlayerControlComponent>();
    }
};