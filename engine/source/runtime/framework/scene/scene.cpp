#include "scene.hpp"

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
		auto samplerInfo = rhi::SamplerCreateInfo()
			.SetMinFilter(rhi::Filter::Linear)
			.SetMagFilter(rhi::Filter::Linear)
			.SetMipFilter(rhi::Filter::Linear)
			.SetAddressModeU(rhi::SamplerAddressMode::Repeat)
			.SetAddressModeV(rhi::SamplerAddressMode::Repeat)
			.SetAddressModeW(rhi::SamplerAddressMode::Repeat)
			.SetMinLod(0.0f)
			.SetMaxLod(1.0f);
		sampler = RenderDevice::GetDevice()->CreateSampler(samplerInfo);

		TransformComponent transform = {};
		transform.position = math::Vector3f(0.0f, 0.0f, 0.0f);
		transform.scale = math::Vector3f(1.0f, 1.0f, 1.0f);
		transform.localEulerAngle = math::Vector3f(0.0f, 0.0f, 0.0f);
		transform.globalEulerAngle = math::Vector3f(0.0f, 0.0f, 0.0f);

		std::vector<ObjectVertexData> vertices(3);
		vertices[0].worldPos = math::Vector3f(-0.5f, -0.5f, 0.0f);
		vertices[1].worldPos = math::Vector3f(0.5f, -0.5f, 0.0f);
		vertices[2].worldPos = math::Vector3f(0.0f, 0.5f, 0.0f);

		MeshRendererComponent meshRenderer = {};
		meshRenderer.vertices = vertices;
		meshRenderer.indices.emplace_back(0);
		meshRenderer.indices.emplace_back(1);
		meshRenderer.indices.emplace_back(2);

		auto rawImage = asset::ImageLoader::Load(Path("asset/brickTexture.jpg").GetAbsolutePath().c_str(), 32);
		image = asset::ImageLoader::SetupImage(rawImage, RenderDevice::GetDevice(), RenderDevice::GetQueue());

		MaterialComponent material = {};
		material.sampler = sampler;
		material.texture = image;

		auto entity = GetRegistry().create();
		GetRegistry().emplace<TransformComponent>(entity, transform);
		GetRegistry().emplace<MeshRendererComponent>(entity, meshRenderer);
		GetRegistry().emplace<MaterialComponent>(entity, material);

		AddSystem(std::make_unique<RenderSystem>());
	}

	MainScene::~MainScene() {
		sampler->Destroy();
		image->Destroy();
	}

}