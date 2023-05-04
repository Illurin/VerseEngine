#pragma once

#include "runtime/math/math.hpp"
#include "interface/rhi.hpp"

namespace engine {

    constexpr auto maxDirectionalLights = 1;
    constexpr auto maxPointLights = 100;
    constexpr auto maxSpotLights = 1;
    constexpr auto maxLightCount = maxDirectionalLights + maxPointLights + maxSpotLights;

    enum class LightType {
        Directional = 0,
        Point,
        Spot
    };

    constexpr auto lightTypeCount = static_cast<uint32_t>(LightType::Spot) + 1;
    constexpr std::array<uint32_t, lightTypeCount> typedLightCounts = {
        maxDirectionalLights, maxPointLights, maxSpotLights
    };

    enum class RenderingType {
        Forward,
        Deferred
    };

    struct ColorQuadVertexData {
        math::Vector2f position;
        math::Vector3f color;
    };

    struct TexQuadVertexData {
        math::Vector2f position;
        math::Vector2f texCoord;
    };

    struct ObjectVertexData {
        math::Vector3f localPos;
        math::Vector3f normal;
        math::Vector3f tangent;
        math::Vector2f texCoord;
    };

    struct LightConstants {
        math::Vector3f color;
        float intensity;
        math::Vector3f direction;
        float attenuation;
        math::Vector3f position;
        float spotPower;
    };

    struct PassConstants {
        math::Matrix4f viewMatrix;
        math::Matrix4f projMatrix;
        math::Vector4f eyePos;
        math::Vector4f ambientLight;
        std::array<LightConstants, maxLightCount> lights;
    };

    struct ObjectConstants {
        math::Matrix4f worldMatrix;
    };

    struct MaterialConstants {
        math::Vector4f diffuseAlbedo;
        math::Vector3f fresnelR0;
        float roughness;
    };

}