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
            std::string name;

            math::Vector3f baseColor;
            math::Vector3f specular;
            math::Vector3f emission;
            float metallic{ 0.0f };
            float roughness{ 0.0f };

            std::optional<RawModelTexture> diffuseMap;
            std::optional<RawModelTexture> normalMap;
        };

        struct RawModelMesh {
            std::string name;

            std::vector<RawModelVertexData> vertices;
            std::vector<uint32_t> indices;
            RawModelMaterial material;
        };

        struct RawModelTransform {
            math::Vector3f position;
            math::Vector3f scale;
            math::Vector3f rotation;
        };

        struct RawModelNode {
            std::string name;

            RawModelTransform transform;
            std::vector<RawModelMesh> meshes;
        };

        struct RawModelScene {
            std::vector<RawModelNode> nodes;
        };

    }
}