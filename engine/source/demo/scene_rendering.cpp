#include "runtime/framework/scene/scene_manager.hpp"

#include "runtime/system/platform/input_system.hpp"
#include "runtime/resource/resource_manager.hpp"
#include "runtime/resource/asset_loader/model/model_loader.hpp"
#include "runtime/render/geometry.hpp"
#include "runtime/system/time.hpp"

using namespace engine;

struct ControllerComponent : IComponent {
    ControllerComponent() = default;
    ControllerComponent(float moveSpeed, float rotateSpeed) :
        moveSpeed(moveSpeed), rotateSpeed(rotateSpeed) {}

    float moveSpeed;
    float rotateSpeed;
};

class ControlSystem : public ISystem {
public:
    void Update() override {
        for (auto [entity, camera, controller] : GetEntities<CameraComponent, const ControllerComponent>().each()) {
            auto movement = controller.moveSpeed * static_cast<float>(Time::GetDeltaTime());

            if (input::GetKey(input::KeyCode::D)) { camera.camera.Strafe(movement);  camera.dirty = true; }
            if (input::GetKey(input::KeyCode::A)) { camera.camera.Strafe(-movement); camera.dirty = true; }
            if (input::GetKey(input::KeyCode::W)) { camera.camera.Walk(movement);    camera.dirty = true; }
            if (input::GetKey(input::KeyCode::S)) { camera.camera.Walk(-movement);   camera.dirty = true; }

            auto rotation = controller.rotateSpeed * static_cast<float>(Time::GetDeltaTime());

            if (input::GetKey(input::KeyCode::RightArrow)) { camera.camera.RotateY(-rotation);  camera.dirty = true; }
            if (input::GetKey(input::KeyCode::LeftArrow)) { camera.camera.RotateY(rotation); camera.dirty = true; }
            if (input::GetKey(input::KeyCode::UpArrow)) { camera.camera.Pitch(rotation);    camera.dirty = true; }
            if (input::GetKey(input::KeyCode::DownArrow)) { camera.camera.Pitch(-rotation);   camera.dirty = true; }
        }
    }
};

class MainScene : public IScene {
public:
    MainScene() {

        // System
        
        AddSystem<ControlSystem>();

        // Environment
        
        Entity::GetSingletonComponents<EnvironmentComponent>() =
            EnvironmentComponent({ math::Vector3f(0.1f, 0.1f, 0.1f),
                                   Path::GetAbsolutePath("asset/skybox.png") });

        // Camera

        Camera camera;
        camera.SetPosition(math::Vector3f(2.0f, 1.0f, 0.0f));

        auto cameraEntity = CreateEntity();
        cameraEntity.AddComponent<CameraComponent>(camera);
        cameraEntity.AddComponent<ControllerComponent>(2.0f, 2.0f);
        
        // Sampler

        auto repeatSamplerInfo = rhi::SamplerCreateInfo()
            .SetMinFilter(rhi::Filter::Linear)
            .SetMagFilter(rhi::Filter::Linear)
            .SetMipFilter(rhi::Filter::Linear)
            .SetAddressModeU(rhi::SamplerAddressMode::Repeat)
            .SetAddressModeV(rhi::SamplerAddressMode::Repeat)
            .SetAddressModeW(rhi::SamplerAddressMode::Repeat)
            .SetMinLod(0.0f)
            .SetMaxLod(1.0f);

        auto borderSamplerInfo = rhi::SamplerCreateInfo()
            .SetMinFilter(rhi::Filter::Linear)
            .SetMagFilter(rhi::Filter::Linear)
            .SetMipFilter(rhi::Filter::Linear)
            .SetAddressModeU(rhi::SamplerAddressMode::Border)
            .SetAddressModeV(rhi::SamplerAddressMode::Border)
            .SetAddressModeW(rhi::SamplerAddressMode::Border)
            .SetBorderColor(rhi::BorderColor::OpaqueBlack)
            .SetMinLod(0.0f)
            .SetMaxLod(1.0f);

        // Marisa model

        auto marisaRawModel = asset::ModelLoader::Load(Path::GetAbsolutePath("asset/model.fbx"));
        
        for (auto& node : marisaRawModel.scene.nodes) {
            TransformData tranformData{ math::Vector3f(0.0f, 0.0f, 0.0f),
                                        math::Vector3f(0.05f, 0.05f, 0.05f),
                                        math::Vector3f(-90.0f, 0.0f, 0.0f),
                                        math::Vector3f::Zero() };

            for (auto& mesh : node.meshes) {
                auto meshEntity = CreateEntity();

                meshEntity.AddComponent<TransformComponent>(tranformData);

                std::vector<ObjectVertexData> meshVertices(mesh.vertices.size());
                for (size_t i = 0; i < mesh.vertices.size(); i++) {
                    meshVertices[i].localPos = mesh.vertices[i].position;
                    meshVertices[i].normal = mesh.vertices[i].normal;
                    meshVertices[i].tangent = mesh.vertices[i].tangent;
                    meshVertices[i].texCoord = mesh.vertices[i].texCoord;
                }
                meshEntity.AddComponent<MeshRendererComponent>(MeshRendererData({ meshVertices, mesh.indices }));

                MaterialData materialData{
                    mesh.material.name,
                    math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
                    math::Vector3f(0.1f, 0.1f, 0.1f),
                    0.9f,
                    mesh.material.diffuseMap.has_value() ? mesh.material.diffuseMap.value().path
                                                         : Path::GetAbsolutePath("asset/default/albedo.jpg"),
                    mesh.material.normalMap.has_value() ? mesh.material.normalMap.value().path
                                                         : Path::GetAbsolutePath("asset/default/normal.jpg"),
                    repeatSamplerInfo
                };

                meshEntity.AddComponent<MaterialComponent>(ResourceManager::CreateMaterial(materialData));
            }
        }

        // Deferred scene

        auto deferredSceneModel = asset::ModelLoader::Load(Path::GetAbsolutePath("asset/Deferred/Deferred.gltf"));

        for (auto& node : deferredSceneModel.scene.nodes) {
            TransformData tranformData{ node.transform.position,
                                        node.transform.scale,
                                        node.transform.rotation,
                                        math::Vector3f::Zero() };
            for (auto& mesh : node.meshes) {
                auto meshEntity = CreateEntity();

                meshEntity.AddComponent<TransformComponent>(tranformData);

                std::vector<ObjectVertexData> meshVertices(mesh.vertices.size());
                for (size_t i = 0; i < mesh.vertices.size(); i++) {
                    meshVertices[i].localPos = mesh.vertices[i].position;
                    meshVertices[i].normal = mesh.vertices[i].normal;
                    meshVertices[i].tangent = mesh.vertices[i].tangent;
                    meshVertices[i].texCoord = mesh.vertices[i].texCoord;
                }
                meshEntity.AddComponent<MeshRendererComponent>(MeshRendererData({ meshVertices, mesh.indices }));

                MaterialData materialData;
                materialData.name = mesh.material.name;
                materialData.diffuseAlbedo << mesh.material.baseColor.x(),
                                                   mesh.material.baseColor.y(),
                                                   mesh.material.baseColor.z(),
                                                   1.0f;
                if (mesh.material.emission.norm() > 0.0f) {
                    materialData.diffuseAlbedo << mesh.material.emission.x() * 5.0f,
                                                       mesh.material.emission.y() * 5.0f,
                                                       mesh.material.emission.z() * 5.0f,
                                                       1.0f;

                    LightData lightData;
                    lightData.type = LightType::Point;
                    lightData.color = mesh.material.emission;
                    lightData.intensity = 0.3f;
                    lightData.attenuation = 0.7f;
                    meshEntity.AddComponent<LightComponent>(lightData);
                }
                materialData.fresnelR0 = mesh.material.specular;
                materialData.roughness = mesh.material.roughness;
                materialData.diffuseMap = mesh.material.diffuseMap.has_value() ?
                                          mesh.material.diffuseMap.value().path :
                                          Path::GetAbsolutePath("asset/default/albedo.jpg");
                materialData.normalMap = mesh.material.normalMap.has_value() ?
                                         mesh.material.normalMap.value().path :
                                         Path::GetAbsolutePath("asset/default/normal.jpg");

                meshEntity.AddComponent<MaterialComponent>(ResourceManager::CreateMaterial(materialData));
            }
        }
    }
};

void SceneManager::Entry() {
    LoadScene<MainScene>();
}