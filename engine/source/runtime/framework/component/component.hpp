#pragma once

#include "runtime/math/math.hpp"
#include "runtime/render/camera.hpp"
#include "runtime/render/render_data.hpp"

namespace engine {

	struct IComponent { };

	struct TransformComponent : public IComponent {
		math::Vector3f position;
		math::Vector3f scale;
		math::Vector3f localEulerAngle;
		math::Vector3f globalEulerAngle;

		rhi::Buffer uniformBuffer{ nullptr };
		rhi::DescriptorSet descriptorSet{ nullptr };
	};

	struct MeshRendererComponent : public IComponent {
		std::vector<ObjectVertexData> vertices;
		std::vector<uint32_t> indices;

		uint32_t vertexOffset{ 0 };
		uint32_t firstIndex{ 0 };
	};

	struct MaterialComponent : public IComponent {
		MaterialConstants constants;
		rhi::Image texture{ nullptr };
		rhi::Sampler sampler{ nullptr };

		rhi::Buffer uniformBuffer{ nullptr };
		rhi::DescriptorSet descriptorSet{ nullptr };
	};

	struct CameraComponent : public IComponent {
		bool active{ false };
		Camera camera;
	};

}