#pragma once

#include "runtime/render/render_data.hpp"

namespace engine {

    struct MeshRendererData {
        std::vector<ObjectVertexData> vertices;
        std::vector<uint32_t> indices;
        RenderingType renderingType{ RenderingType::Forward };
    };

    struct LightData {
        LightType type{ LightType::Directional };
        math::Vector3f color;
        math::Vector3f direction;
        float intensity{ 0.0f };
        float attenuation{ 0.0f };
        float spotPower{ 0.0f };
    };

    struct EnvironmentData {
        math::Vector3f ambientLight;
        std::filesystem::path skyboxTexturePath;
    };

    struct PostProcessingData {
        float exposure{ 0.0f };
        float gamma{ 0.0f };
    };

}