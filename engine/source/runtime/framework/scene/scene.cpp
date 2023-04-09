#include "scene.hpp"

#include "runtime/framework/system/control_system.hpp"
#include "runtime/framework/system/resource_system.hpp"
#include "runtime/framework/system/render_system.hpp"

#include "runtime/resource/asset_loader/model/model_loader.hpp"

namespace engine {

	void IScene::Start() {
		for (auto& system : systems) {
			system->Start(registry);
		}
	}

	void IScene::Update() {
		for (auto& system : systems) {
			system->Update(registry);
		}
	}

	IScene::~IScene() {
		auto transformView = registry.view<TransformComponent>();
		transformView.each([](TransformComponent& transform) {
			if (transform.uniformBuffer) transform.uniformBuffer->Destroy();
		});

		auto materialView = registry.view<MaterialComponent>();
		materialView.each([](MaterialComponent& material) {
			if (material.uniformBuffer) material.uniformBuffer->Destroy();
		});
	}

	MainScene::MainScene() {

		// Camera

		auto aspect = static_cast<float>(RenderDevice::GetSwapchain()->GetImageExtent().width)
			        / static_cast<float>(RenderDevice::GetSwapchain()->GetImageExtent().height);
		CameraComponent cameraComponent(true, Camera(aspect), math::Vector3f(0.2f, 0.2f, 0.2f));
		cameraComponent.camera.SetPosition(math::Vector3f(0.0f, 5.0f, 5.0f));
		cameraComponent.camera.LookAt(math::Vector3f(0.0f, 5.0f, 5.0f), math::Vector3f(0.0f, 0.0f, 0.0f), math::Vector3f::UnitY());

		auto cameraEntity = GetRegistry().create();
		GetRegistry().emplace<CameraComponent>(cameraEntity, cameraComponent);
		GetRegistry().emplace<ControllerComponent>(cameraEntity, 10.0f, 2.0f);

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
		planeMaterialConstants.shiniess = 0.8f;

		MaterialConstants modelMaterialConstants;
		modelMaterialConstants.diffuseAlbedo = math::Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
		modelMaterialConstants.fresnelR0 = math::Vector3f(0.8f, 0.8f, 0.8f);
		modelMaterialConstants.shiniess = 0.3f;

		// Object 0

		auto planeEntity = GetRegistry().create();

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
		GetRegistry().emplace<MaterialComponent>(planeEntity, planeMaterialConstants, Path::GetAbsolutePath("asset/brickTexture.jpg"), repeatSamplerInfo);

		// Marisa model

		auto marisaRawModel = asset::ModelLoader::Load(Path::GetAbsolutePath("asset/model.fbx"));

		for (auto& mesh : marisaRawModel.meshes) {
			auto meshEntity = GetRegistry().create();

			GetRegistry().emplace<TransformComponent>(meshEntity, math::Vector3f(0.0f, 0.0f, 0.0f),
			                                                      math::Vector3f(0.5f, 0.5f, 0.5f),
														          math::Vector3f(-90.0f, 0.0f, 0.0f),
			                                                      math::Vector3f(0.0f, 0.0f, 0.0f));
			std::vector<ObjectVertexData> meshVertices(mesh.vertices.size());
			for (size_t i = 0; i < mesh.vertices.size(); i++) {
				meshVertices[i].localPos = mesh.vertices[i].position;
				meshVertices[i].normal = mesh.vertices[i].normal;
				meshVertices[i].texCoord = mesh.vertices[i].texCoord;
			}
			GetRegistry().emplace<MeshRendererComponent>(meshEntity, meshVertices, mesh.indices);
			GetRegistry().emplace<MaterialComponent>(meshEntity, modelMaterialConstants,
				                                     mesh.material.diffuseMap.has_value() ? mesh.material.diffuseMap.value().path : Path::GetAbsolutePath("asset/brickTexture.jpg"),
				                                     mesh.material.diffuseMap.has_value() ? repeatSamplerInfo : borderSamplerInfo);
		}

		// Systems

		AddSystem(std::make_unique<ControlSystem>());
		AddSystem(std::make_unique<ResourceSystem>());
		AddSystem(std::make_unique<RenderSystem>());
	}

}