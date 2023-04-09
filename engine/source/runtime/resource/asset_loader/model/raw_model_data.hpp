#pragma once

#include "runtime/math/math.hpp"
#include "runtime/system/file_system/path.hpp"

#include <optional>

namespace engine {
	namespace asset {

		struct RawModelVertexData {
			math::Vector3f position;
			math::Vector3f normal;
			math::Vector3f tangent;
			math::Vector2f texCoord;
		};

		struct RawModelTexture {
			std::filesystem::path path;
		};

		struct RawModelMaterial {
			std::optional<RawModelTexture> diffuseMap;
		};

		struct RawModelMesh {
			std::string name;
			std::vector<RawModelVertexData> vertices;
			std::vector<uint32_t> indices;
			RawModelMaterial material;
		};

	}
}