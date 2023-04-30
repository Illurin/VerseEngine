#pragma once

#include "runtime/math/math.hpp"
#include "runtime/render/camera.hpp"
#include "runtime/render/render_data.hpp"

namespace engine {

    struct IComponent {};

    struct TransformComponent : IComponent {
        TransformComponent() = default;
        TransformComponent(math::Vector3f position,
                           math::Vector3f scale,
                           math::Vector3f localEulerAngle,
                           math::Vector3f globalEulerAngle) :
            position(position),
            scale(scale),
            localEulerAngle(localEulerAngle),
            globalEulerAngle(globalEulerAngle) {}

        math::Vector3f position;
        math::Vector3f scale;
        math::Vector3f localEulerAngle;
        math::Vector3f globalEulerAngle;

        uint32_t index{ ~0u };
        bool dirty{ true };
    };

    struct MeshRendererComponent : IComponent {
        MeshRendererComponent() = default;
        MeshRendererComponent(const std::vector<ObjectVertexData>& vertices,
                              const std::vector<uint32_t>& indices) :
            vertices(vertices), indices(indices) {}
        MeshRendererComponent(const std::vector<ObjectVertexData>& vertices,
                              const std::vector<uint32_t>& indices,
                              RenderingType renderingType) :
            vertices(vertices), indices(indices), renderingType(renderingType) {}

        std::vector<ObjectVertexData> vertices;
        std::vector<uint32_t> indices;

        uint32_t vertexOffset{ 0 };
        uint32_t firstIndex{ 0 };
        RenderingType renderingType{ RenderingType::Forward };
    };


    struct SkyboxRendererComponent : IComponent {
        SkyboxRendererComponent() = default;

        uint32_t vertexOffset{ 0 };
        uint32_t firstIndex{ 0 };
        uint32_t indexCount{ 0 };
    };

    struct MaterialComponent : IComponent {
        MaterialComponent() = default;
        MaterialComponent(MaterialConstants constants,
                          std::filesystem::path texturePath,
                          rhi::SamplerCreateInfo samplerInfo) :
            constants(constants),
            texturePath(texturePath),
            samplerInfo(samplerInfo) {}

        MaterialConstants constants{};
        std::filesystem::path texturePath;
        rhi::SamplerCreateInfo samplerInfo{};
        
        rhi::Image texture{ nullptr };
        rhi::Sampler sampler{ nullptr };
        uint32_t index{ ~0u };
    };

    struct CameraComponent : IComponent {
        CameraComponent() = default;
        CameraComponent(const Camera& camera) :
            camera(camera) {}
        CameraComponent(bool active, const Camera& camera) :
            active(active), camera(camera) {}

        bool active{ true };
        Camera camera;

        bool dirty{ true };
    };

    struct ControllerComponent : IComponent {
        ControllerComponent() = default;
        ControllerComponent(float moveSpeed, float rotateSpeed) :
            moveSpeed(moveSpeed), rotateSpeed(rotateSpeed) {}

        float moveSpeed;
        float rotateSpeed;
    };

    struct LightComponent : IComponent {
        LightComponent() = default;
        LightComponent(math::Vector3f color, math::Vector3f direction, float intensity) :
            type(LightType::Directional), color(color), direction(direction), intensity(intensity) {}
        LightComponent(math::Vector3f color, float intensity, float attenuation) :
            type(LightType::Point), color(color), intensity(intensity), attenuation(attenuation) {}
        LightComponent(math::Vector3f color, math::Vector3f direction, float intensity, float attenuation, float spotPower) :
            type(LightType::Spot), color(color), direction(direction), intensity(intensity), attenuation(attenuation), spotPower(spotPower) {}

        LightType type{ LightType::Directional };
        math::Vector3f color;
        math::Vector3f direction;
        float intensity{ 0.0f };
        float attenuation{ 0.0f };
        float spotPower{ 0.0f };

        uint32_t index{ ~0u };
        bool dirty{ true };
    };

    struct EnvironmentComponent : IComponent {
        EnvironmentComponent() = default;
        EnvironmentComponent(math::Vector3f ambientLight, std::filesystem::path skyboxTexturePath) :
            ambientLight(ambientLight), skyboxTexturePath(skyboxTexturePath) {}

        math::Vector3f ambientLight;
        std::filesystem::path skyboxTexturePath;

        rhi::Image skyboxTexture{ nullptr };
        rhi::Sampler skyboxSampler{ nullptr };
        bool dirty{ true };
    };

    struct PostProcessingComponent : IComponent {
        PostProcessingComponent() = default;
        PostProcessingComponent(float exposure, float gamma) :
            exposure(exposure), gamma(gamma) {}

        float exposure{ 0.0f };
        float gamma{ 0.0f };

        rhi::Buffer uniformBuffer{ nullptr };
        rhi::DescriptorSet descriptorSet{ nullptr };
    };
}