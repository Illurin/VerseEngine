#pragma once

#include "runtime/math/math.hpp"

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

	struct ObjectConstants {
		math::Matrix4f worldMatrix;
	};

	struct PassConstants {
		math::Matrix4f viewMatrix;
		math::Matrix4f projMatrix;
	};

}