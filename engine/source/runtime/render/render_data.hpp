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
        math::Vector3f localPos;
        math::Vector3f normal;
        math::Vector3f tangent;
        math::Vector2f texCoord;
    };

    struct PassConstants {
        math::Matrix4f viewMatrix;
        math::Matrix4f projMatrix;
        math::Vector4f eyePos;
        math::Vector3f ambientLight;
    };

    struct ObjectConstants {
        math::Matrix4f worldMatrix;
    };

    struct MaterialConstants {
        math::Vector4f diffuseAlbedo;
        math::Vector3f fresnelR0;
        float shiniess;
    };

}