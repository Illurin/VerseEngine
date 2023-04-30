#include "runtime/framework/scene/scene_manager.hpp"
#include "runtime/render/geometry.hpp"
#include "runtime/system/platform/input_system.hpp"
#include "runtime/system/time.hpp"
#include "runtime/resource/asset_loader/model/model_loader.hpp"

using namespace engine;

struct PlayerControlComponent {
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
        
        // 环境光和天空盒

        Entity::GetSingletonComponents<EnvironmentComponent>() =
            EnvironmentComponent(math::Vector3f(1.0f, 1.0f, 1.0f),
                Path::GetAbsolutePath("asset/skybox.png"));

        // 设置摄像机属性

        Camera camera;
        camera.SetPosition(math::Vector3f(0.0f, 0.0f, 0.0f));
        camera.LookAt(math::Vector3f(0.0f, 0.0f, -1.0f), math::Vector3f::UnitY());

        // 创建摄像机实体

        auto cameraEntity = CreateEntity();
        cameraEntity.AddComponent<CameraComponent>(camera);

        // 新建物体

        auto objectEntity = CreateEntity();

        // 添加 Transform 组件（位移，缩放，旋转）

        objectEntity.AddComponent<TransformComponent>(math::Vector3f(0.0f, 0.0f, -10.0f),
                                                      math::Vector3f(1.0f, 1.0f, 1.0f),
                                                      math::Vector3f::Zero(),
                                                      math::Vector3f::Zero());

        // 给物体添加球体渲染网格

        auto geo = Geometry::CreateGeosphere(1.0f, 5);
        objectEntity.AddComponent<MeshRendererComponent>(geo.vertices, geo.indices);

        // 给物体添加材质

        // 采样器属性
        auto objectSamplerInfo = rhi::SamplerCreateInfo()
            .SetMinFilter(rhi::Filter::Linear)
            .SetMagFilter(rhi::Filter::Linear)
            .SetMipFilter(rhi::Filter::Linear);

        // 材质属性
        MaterialConstants objectMaterialConstants;
        objectMaterialConstants.diffuseAlbedo = math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        objectMaterialConstants.fresnelR0 = math::Vector3f(0.3f, 0.3f, 0.3f);
        objectMaterialConstants.shiniess = 0.5f;

        // 贴图路径
        auto objectTexturePath = Path::GetAbsolutePath("asset/brickTexture.jpg");

        // 添加材质组件
        objectEntity.AddComponent<MaterialComponent>(objectMaterialConstants,
                                                     objectTexturePath, 
                                                     objectSamplerInfo);

        // 操控
        objectEntity.AddComponent<PlayerControlComponent>();
    }
};

void SceneManager::Entry() {
    LoadScene<DemoScene>();
}