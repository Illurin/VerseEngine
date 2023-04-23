#pragma once

#include "runtime/math/math.hpp"
#include "runtime/render/camera.hpp"
#include "runtime/render/render_data.hpp"

namespace engine {

    struct IComponent { };

    struct TransformComponent : public IComponent {
        TransformComponent() = default;
        TransformComponent(math::Vector3f position,
                           math::Vector3f scale,
                           math::Vector3f localEulerAngle,
                           math::Vector3f globalEulerAngle) :
            position(position),
            scale(scale),
            localEulerAngle(localEulerAngle),
            globalEulerAngle(globalEulerAngle) { }

        math::Vector3f position;
        math::Vector3f scale;
        math::Vector3f localEulerAngle;
        math::Vector3f globalEulerAngle;

        rhi::Buffer uniformBuffer{ nullptr };
        rhi::DescriptorSet descriptorSet{ nullptr };
        bool dirty{ true };
    };

    struct MeshRendererComponent : public IComponent {
        MeshRendererComponent() = default;
        MeshRendererComponent(const std::vector<ObjectVertexData>& vertices,
                              const std::vector<uint32_t>& indices) :
            vertices(vertices), indices(indices) { }

        std::vector<ObjectVertexData> vertices;
        std::vector<uint32_t> indices;

        uint32_t vertexOffset{ 0 };
        uint32_t firstIndex{ 0 };
    };


    struct SkyboxRendererComponent : public IComponent {
        SkyboxRendererComponent() = default;

        uint32_t vertexOffset{ 0 };
        uint32_t firstIndex{ 0 };
        uint32_t indexCount{ 0 };
    };

    struct MaterialComponent : public IComponent {
        MaterialComponent() = default;
        MaterialComponent(MaterialConstants constants,
                          std::filesystem::path texturePath,
                          rhi::SamplerCreateInfo samplerInfo) :
            constants(constants),
            texturePath(texturePath),
            samplerInfo(samplerInfo) { }

        MaterialConstants constants;
        std::filesystem::path texturePath;
        rhi::SamplerCreateInfo samplerInfo;
        
        rhi::Image texture{ nullptr };
        rhi::Sampler sampler{ nullptr };
        rhi::Buffer uniformBuffer{ nullptr };
        rhi::DescriptorSet descriptorSet{ nullptr };
    };

    struct CameraComponent : public IComponent {
        CameraComponent() = default;
        CameraComponent(bool active, const Camera& camera) :
            active(active), camera(camera) { }

        bool active{ false };
        Camera camera;

        bool dirty{ true };
    };

    struct ControllerComponent : public IComponent {
        ControllerComponent() = default;
        ControllerComponent(float moveSpeed, float rotateSpeed) :
            moveSpeed(moveSpeed), rotateSpeed(rotateSpeed) { }

        float moveSpeed;
        float rotateSpeed;
    };

    struct EnvironmentComponent : public IComponent {
        EnvironmentComponent() = default;
        EnvironmentComponent(math::Vector3f ambientLight, std::filesystem::path skyboxTexturePath) :
            ambientLight(ambientLight), skyboxTexturePath(skyboxTexturePath) { }

        math::Vector3f ambientLight;
        std::filesystem::path skyboxTexturePath;

        rhi::Image skyboxTexture{ nullptr };
        rhi::Sampler skyboxSampler{ nullptr };
        bool dirty{ true };
    };
}