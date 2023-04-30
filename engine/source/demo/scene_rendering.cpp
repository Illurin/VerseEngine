#include "runtime/framework/scene/scene_manager.hpp"

#include "runtime/render/geometry.hpp"
#include "runtime/system/platform/input_system.hpp"
#include "runtime/system/time.hpp"
#include "runtime/resource/asset_loader/model/model_loader.hpp"

using namespace engine;

class ControlSystem : public ISystem {
public:
    void Update() {
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

        // Material constants

        MaterialConstants planeMaterialConstants;
        planeMaterialConstants.diffuseAlbedo = math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        planeMaterialConstants.fresnelR0 = math::Vector3f(0.3f, 0.3f, 0.3f);
        planeMaterialConstants.shiniess = 0.5f;

        MaterialConstants modelMaterialConstants;
        modelMaterialConstants.diffuseAlbedo = math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
        modelMaterialConstants.fresnelR0 = math::Vector3f(0.1f, 0.1f, 0.1f);
        modelMaterialConstants.shiniess = 0.1f;

        MaterialConstants lightMaterialConstants;
        lightMaterialConstants.diffuseAlbedo = math::Vector4f(1000.0f, 1.0f, 1.0f, 1.0f);
        lightMaterialConstants.fresnelR0 = math::Vector3f(0.3f, 0.3f, 0.3f);
        lightMaterialConstants.shiniess = 0.5f;

        // Object 0

        /*auto planeEntity = GetRegistry().create();

        GetRegistry().emplace<TransformComponent>(planeEntity, math::Vector3f(0.0f, 0.0f, 0.0f),
                                                               math::Vector3f(1.0f, 1.0f, 1.0f),
                                                               math::Vector3f(0.0f, 0.0f, 0.0f),
                                                               math::Vector3f(0.0f, 0.0f, 0.0f));

        std::vector<ObjectVertexData> vertices(4);
        vertices[0].localPos = math::Vector3f(-10.0f, 0.0f, -10.0f);
        vertices[0].normal   = math::Vector3f(0.0f, 1.0f, 0.0f);
        vertices[0].texCoord = math::Vector2f(0.0f, 1.0f);
        vertices[1].localPos = math::Vector3f(10.0f, 0.0f, -10.0f);
        vertices[1].normal = math::Vector3f(0.0f, 1.0f, 0.0f);
        vertices[1].texCoord = math::Vector2f(1.0f, 1.0f);
        vertices[2].localPos = math::Vector3f(-10.0f, 0.0f, 10.0f);
        vertices[2].normal = math::Vector3f(0.0f, 1.0f, 0.0f);
        vertices[2].texCoord = math::Vector2f(0.0f, 0.0f);
        vertices[3].localPos = math::Vector3f(10.0f, 0.0f, 10.0f);
        vertices[3].normal = math::Vector3f(0.0f, 1.0f, 0.0f);
        vertices[3].texCoord = math::Vector2f(1.0f, 0.0f);

        std::vector<uint32_t> indices = {
            0, 2, 1, 1, 2, 3
        };

        GetRegistry().emplace<MeshRendererComponent>(planeEntity, vertices, indices);
        GetRegistry().emplace<MaterialComponent>(planeEntity, planeMaterialConstants, Path::GetAbsolutePath("asset/brickTexture.jpg"), repeatSamplerInfo);*/
        
        // Object 1

        /*auto sphereEntity = CreateEntity();

        sphereEntity.AddComponent<TransformComponent>(math::Vector3f(8.0f, 6.0f, 0.0f),
                                                      math::Vector3f(1.0f, 1.0f, 1.0f),
                                                      math::Vector3f(0.0f, 0.0f, 0.0f),
                                                      math::Vector3f(0.0f, 0.0f, 0.0f));

        auto geosphereMesh = Geometry::CreateGeosphere(2.0f, 10);

        sphereEntity.AddComponent<MeshRendererComponent>(geosphereMesh.vertices, geosphereMesh.indices);
        sphereEntity.AddComponent<MaterialComponent>(lightMaterialConstants, Path::GetAbsolutePath("asset/brickTexture.jpg"), repeatSamplerInfo);*/

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
                meshEntity.AddComponent<MaterialComponent>(modelMaterialConstants,
                                                           mesh.material.diffuseMap.has_value() ? mesh.material.diffuseMap.value().path : Path::GetAbsolutePath("asset/brickTexture.jpg"),
                                                           mesh.material.diffuseMap.has_value() ? repeatSamplerInfo : borderSamplerInfo);
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

                    meshEntity.AddComponent<LightComponent>(mesh.material.emission, 0.3f, 0.7f);
                }
                materialConstants.fresnelR0 = mesh.material.specular;
                materialConstants.shiniess = 1.0f - mesh.material.roughness;

                meshEntity.AddComponent<MaterialComponent>(materialConstants,
                                                           mesh.material.diffuseMap.has_value() ? 
                                                              mesh.material.diffuseMap.value().path : 
                                                              Path::GetAbsolutePath("asset/default/albedo.jpg"),
                                                           repeatSamplerInfo);
            }
        }
    }
};

void SceneManager::Entry() {
    LoadScene<MainScene>();
}