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

            if (input::GetKey(input::KeyCode::D)) transform.position.x() += movement;
            if (input::GetKey(input::KeyCode::A)) transform.position.x() -= movement;
            if (input::GetKey(input::KeyCode::W)) transform.position.y() += movement;
            if (input::GetKey(input::KeyCode::S)) transform.position.y() -= movement;

            transform.dirty = true;
        }
    }
};

class DemoScene : public IScene {
public:
    DemoScene() {

        AddSystem<PlayerControlSystem>();
        
        // Ambient light and sky box

        auto& environmentComponent = Entity::AddSingletonComponent<EnvironmentComponent>();
        environmentComponent.ambientLight = math::Vector3f(1.0f, 1.0f, 1.0f);
        environmentComponent.skyboxTextureResource = ResourceManager::ImportCubemap(Path::GetAbsolutePath("asset/skybox.png"));

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

        objectEntity.AddComponent<TransformComponent>(math::Vector3f(0.0f, 0.0f, -10.0f),
                                                      math::Vector3f(1.0f, 1.0f, 1.0f),
                                                      math::Vector3f::Zero(),
                                                      math::Vector3f::Zero()); 

        // Add mesh renderer component

        auto geo = Geometry::CreateGeosphere(1.0f, 5);
        objectEntity.AddComponent<MeshRendererComponent>(geo.vertices, geo.indices);

        // Add material
        
        // Material properties
        MaterialConstants objectMaterialConstants;
        objectMaterialConstants.diffuseAlbedo = math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        objectMaterialConstants.fresnelR0 = math::Vector3f(0.3f, 0.3f, 0.3f);
        objectMaterialConstants.roughness = 0.5f;

        auto diffuseMap = ResourceManager::ImportTexture(Path::GetAbsolutePath("asset/brickTexture.jpg"));
        auto normalMap = ResourceManager::ImportTexture(Path::GetAbsolutePath("asset/default/normal.jpg"));

        // Add material component
        auto materialResource = ResourceManager::CreateMaterial("objectMaterial", objectMaterialConstants,
                                                                diffuseMap, normalMap);
        objectEntity.AddComponent<MaterialComponent>(materialResource);

        // Player control component
        objectEntity.AddComponent<PlayerControlComponent>();
    }
};