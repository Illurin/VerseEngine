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

    float moveSpeed{ 1.0f };
    float rotateSpeed{ 1.0f };
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
        
        auto& environmentComponent = Entity::AddSingletonComponent<EnvironmentComponent>();
        environmentComponent.ambientLight = math::Vector3f(0.5f, 0.5f, 0.5f);
        environmentComponent.skyboxTextureResource = ResourceManager::ImportCubemap(Path::GetAbsolutePath("asset/skybox.png"));

        // Camera

        Camera camera;
        camera.SetPosition(math::Vector3f(2.0f, 1.0f, 0.0f));

        auto cameraEntity = CreateEntity();
        cameraEntity.AddComponent<CameraComponent>(camera);
        cameraEntity.AddComponent<ControllerComponent>(2.0f, 2.0f);

        // Default textures

        auto defaultAlbedoTexture = ResourceManager::ImportTexture(Path::GetAbsolutePath("asset/default/albedo.jpg"));
        auto defaultNormalTexture = ResourceManager::ImportTexture(Path::GetAbsolutePath("asset/default/normal.jpg"));

        // Light

        /*auto lightEntity = CreateEntity();

        lightEntity.AddComponent<TransformComponent>(math::Vector3f(0.0f, 2.0f, 0.0f),
                                                     math::Vector3f(1.0f, 1.0f, 1.0f),
                                                     math::Vector3f::Zero(),
                                                     math::Vector3f::Zero());

        lightEntity.AddComponent<LightComponent>(math::Vector3f(1.0f, 1.0f, 1.0f),
                                                 1.0f,
                                                 0.3f);*/

        // Plane

        /*auto planeEntity = CreateEntity();

        planeEntity.AddComponent<TransformComponent>(math::Vector3f(0.0f, 0.1f, 0.0f),
                                                     math::Vector3f(1.0f, 1.0f, 1.0f),
                                                     math::Vector3f::Zero(),
                                                     math::Vector3f::Zero());

        auto brickDiffuseTexture = ResourceManager::ImportTexture(Path::GetAbsolutePath("asset/brickTexture.jpg"));
        auto brickNormalTexture = ResourceManager::ImportTexture(Path::GetAbsolutePath("asset/brickTexture_normal.jpg"));

        MaterialConstants brickMaterialConstants{ math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
                                                  math::Vector3f(0.3f, 0.3f, 0.3f),
                                                  0.5f };
        auto brickMaterial = ResourceManager::CreateMaterial("brickMaterial", brickMaterialConstants,
                                                             brickDiffuseTexture, brickNormalTexture);
        planeEntity.AddComponent<MaterialComponent>(brickMaterial);

        std::vector<ObjectVertexData> planeVertices(4);

        planeVertices[0].localPos << -5.0f, 0.0f, -5.0f;
        planeVertices[0].normal << 0.0f, 1.0f, 0.0f;
        planeVertices[0].tangent << 1.0f, 0.0f, 0.0f;
        planeVertices[0].texCoord << 0.0f, 0.0f;

        planeVertices[1].localPos << 5.0f, 0.0f, -5.0f;
        planeVertices[1].normal << 0.0f, 1.0f, 0.0f;
        planeVertices[1].tangent << 1.0f, 0.0f, 0.0f;
        planeVertices[1].texCoord << 1.0f, 0.0f;

        planeVertices[2].localPos << -5.0f, 0.0f, 5.0f;
        planeVertices[2].normal << 0.0f, 1.0f, 0.0f;
        planeVertices[2].tangent << 1.0f, 0.0f, 0.0f;
        planeVertices[2].texCoord << 0.0f, 1.0f;

        planeVertices[3].localPos << 5.0f, 0.0f, 5.0f;
        planeVertices[3].normal << 0.0f, 1.0f, 0.0f;
        planeVertices[3].tangent << 1.0f, 0.0f, 0.0f;
        planeVertices[3].texCoord << 1.0f, 1.0f;

        std::vector<uint32_t> planeIndices = {
            0, 1, 2, 1, 2, 3
        };

        planeEntity.AddComponent<MeshRendererComponent>(planeVertices, planeIndices);*/

        // Marisa model

        auto marisaRawModel = asset::ModelLoader::Load(Path::GetAbsolutePath("asset/model.fbx"));
        
        for (auto& node : marisaRawModel.scene.nodes) {
            TransformComponent tranformComponent(math::Vector3f(0.0f, 0.0f, 0.0f),
                                                 math::Vector3f(0.05f, 0.05f, 0.05f),
                                                 math::Vector3f(-90.0f, 0.0f, 0.0f),
                                                 math::Vector3f::Zero());

            for (auto& mesh : node.meshes) {
                auto meshEntity = CreateEntity();

                meshEntity.AddComponent<TransformComponent>(tranformComponent);

                std::vector<ObjectVertexData> meshVertices(mesh.vertices.size());
                for (size_t i = 0; i < mesh.vertices.size(); i++) {
                    meshVertices[i].localPos = mesh.vertices[i].position;
                    meshVertices[i].normal = mesh.vertices[i].normal;
                    meshVertices[i].tangent = mesh.vertices[i].tangent;
                    meshVertices[i].texCoord = mesh.vertices[i].texCoord;
                }
                meshEntity.AddComponent<MeshRendererComponent>(meshVertices, mesh.indices);

                MaterialConstants materialConstants{
                    math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
                    math::Vector3f(0.1f, 0.1f, 0.1f),
                    0.9f
                };

                auto diffuseMap = mesh.material.diffuseMap.has_value() ? 
                                  ResourceManager::ImportTexture(mesh.material.diffuseMap.value().path) :
                                  defaultAlbedoTexture;

                auto materialResource = ResourceManager::CreateMaterial(mesh.material.name, materialConstants,
                                                                        diffuseMap, defaultNormalTexture);
                meshEntity.AddComponent<MaterialComponent>(materialResource);
            }
        }

        // Deferred scene

        auto deferredSceneModel = asset::ModelLoader::Load(Path::GetAbsolutePath("asset/Deferred/Deferred.gltf"));

        for (auto& node : deferredSceneModel.scene.nodes) {
            TransformComponent tranformComponent(node.transform.position,
                                                 node.transform.scale,
                                                 node.transform.rotation,
                                                 math::Vector3f::Zero());
            for (auto& mesh : node.meshes) {
                auto meshEntity = CreateEntity();

                meshEntity.AddComponent<TransformComponent>(tranformComponent);

                std::vector<ObjectVertexData> meshVertices(mesh.vertices.size());
                for (size_t i = 0; i < mesh.vertices.size(); i++) {
                    meshVertices[i].localPos = mesh.vertices[i].position;
                    meshVertices[i].normal = mesh.vertices[i].normal;
                    meshVertices[i].tangent = mesh.vertices[i].tangent;
                    meshVertices[i].texCoord = mesh.vertices[i].texCoord;
                }
                meshEntity.AddComponent<MeshRendererComponent>(meshVertices, mesh.indices);

                MaterialConstants materialConstants;
                materialConstants.diffuseAlbedo << mesh.material.baseColor.x(),
                                                   mesh.material.baseColor.y(),
                                                   mesh.material.baseColor.z(),
                                                   1.0f;
                if (mesh.material.emission.norm() > 0.0f) {
                    materialConstants.diffuseAlbedo << mesh.material.emission.x() * 5.0f,
                                                       mesh.material.emission.y() * 5.0f,
                                                       mesh.material.emission.z() * 5.0f,
                                                       1.0f;

                    meshEntity.AddComponent<LightComponent>(mesh.material.emission,
                                                            0.3f,
                                                            0.7f);
                }
                materialConstants.fresnelR0 = mesh.material.specular;
                materialConstants.roughness = mesh.material.roughness;

                auto diffuseMap = mesh.material.diffuseMap.has_value() ? 
                                  ResourceManager::ImportTexture(mesh.material.diffuseMap.value().path) :
                                  defaultAlbedoTexture;

                auto normalMap = mesh.material.normalMap.has_value() ? 
                                  ResourceManager::ImportTexture(mesh.material.normalMap.value().path) :
                                  defaultNormalTexture;

                auto materialResource = ResourceManager::CreateMaterial(mesh.material.name, materialConstants,
                                                                        diffuseMap, normalMap);
                meshEntity.AddComponent<MaterialComponent>(materialResource);
            }
        }
    }
};

void SceneManager::Entry() {
    LoadScene<MainScene>();
}