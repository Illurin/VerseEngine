#pragma once

#include "runtime/math/math.hpp"
#include "runtime/render/camera.hpp"
#include "runtime/resource/data_type/data.hpp"

namespace engine {

    struct IComponent {};

    struct TransformComponent : IComponent {
        TransformComponent() = default;
        TransformComponent(const TransformData& data) : data(data) {}
            
        uint32_t id{ ~0u };
        bool dirty{ true };
        TransformData data;
    };

    struct MeshRendererComponent : IComponent {
        MeshRendererComponent() = default;
        MeshRendererComponent(const MeshRendererData& data) : data(data) {}

        uint32_t vertexOffset{ 0 };
        uint32_t firstIndex{ 0 };
        MeshRendererData data;
    };

    struct SkyboxRendererComponent : IComponent {
        SkyboxRendererComponent() = default;

        uint32_t vertexOffset{ 0 };
        uint32_t firstIndex{ 0 };
        uint32_t indexCount{ 0 };
    };

    struct MaterialComponent : IComponent {
        MaterialComponent() = default;
        MaterialComponent(uint32_t resource) : resource(resource) {}

        uint32_t resource;
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

    struct LightComponent : IComponent {
        LightComponent() = default;
        LightComponent(const LightData& data) : data(data) {}

        uint32_t id{ ~0u };
        bool dirty{ true };
        LightData data;
    };

    struct EnvironmentComponent : IComponent {
        EnvironmentComponent() = default;
        EnvironmentComponent(const EnvironmentData& data) : data(data) {}

        bool dirty{ true };
        uint32_t skyboxTextureResource{ ~0u };
        rhi::Sampler skyboxSampler{ nullptr };
        EnvironmentData data;
    };

}