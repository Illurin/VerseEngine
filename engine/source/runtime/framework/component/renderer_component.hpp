#pragma once

#include "component.hpp"

#include "runtime/render/render_data.hpp"

namespace engine {

    struct MeshRendererComponent : IComponent {
        MeshRendererComponent() = default;

        MeshRendererComponent(const std::vector<ObjectVertexData>& vertices,
                              const std::vector<uint32_t>& indices) :
            vertices(vertices), indices(indices) {}

        MeshRendererComponent(const std::vector<ObjectVertexData>& vertices,
                              const std::vector<uint32_t>& indices,
                              RenderingType renderingType) :
            vertices(vertices), indices(indices), renderingType(renderingType){}

        uint32_t vertexOffset{ 0 };
        uint32_t firstIndex{ 0 };
        uint32_t indexCount{ 0 };
        
        std::vector<ObjectVertexData> vertices;
        std::vector<uint32_t> indices;
        RenderingType renderingType{ RenderingType::Forward };
    };

    struct SkyboxRendererComponent : IComponent {
        SkyboxRendererComponent() = default;

        uint32_t vertexOffset{ 0 };
        uint32_t firstIndex{ 0 };
        uint32_t indexCount{ 0 };
    };

    struct PostProcessingComponent : IComponent {
        PostProcessingComponent() = default;

        PostProcessingComponent(float exposure, float gamma) :
            exposure(exposure), gamma(gamma) {}

        float exposure{ 0.0f };
        float gamma{ 0.0f };
    };

}