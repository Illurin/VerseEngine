#pragma once

#include "render_data.hpp"

namespace engine {

    class Geometry {
    public:
        struct MeshData {
            std::vector<ObjectVertexData> vertices;
            std::vector<uint32_t> indices;
        };

        static ObjectVertexData Midpoint(const ObjectVertexData&, const ObjectVertexData&);
        static MeshData Subdivide(const MeshData& meshData);
        static MeshData CreateGeosphere(float radius, uint32_t subdivisionCount);
    };

}