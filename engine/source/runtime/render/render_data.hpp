#pragma once

#include "runtime/math/math.hpp"
#include "interface/rhi.hpp"

namespace engine {

	struct ColorQuadVertexData {
		math::Vector2f position;
		math::Vector3f color;
	};

	struct TexQuadVertexData {
		math::Vector2f position;
		math::Vector2f texCoord;
	};

	struct ObjectVertexData {
		math::Vector3f worldPos;
		math::Vector3f normal;
		math::Vector2f texCoord;
	};

	struct PassConstants {
		math::Matrix4f viewMatrix;
		math::Matrix4f projMatrix;
	};

	struct ObjectConstants {
		std::array<std::array<float, 4>, 3> worldMatrix;
	};

	struct MaterialConstants {
		math::Vector3f diffuseAlbedo;
		math::Vector3f fresnelR0;
		float shiniess;
	};

}