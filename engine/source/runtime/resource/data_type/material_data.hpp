#pragma once

#include "runtime/render/render_data.hpp"

namespace engine {

    struct MaterialData {
        std::string name;

        math::Vector4f diffuseAlbedo;
        math::Vector3f fresnelR0;
        float roughness{ 0.0f };

        std::filesystem::path diffuseMap{ Path::GetAbsolutePath("asset/default/albedo.jpg") };
        std::filesystem::path normalMap{ Path::GetAbsolutePath("asset/default/normal.jpg") };

        rhi::SamplerCreateInfo samplerInfo{};
    };

}