#pragma once

#include "component.hpp"

#include "runtime/render/camera.hpp"

namespace engine {

    struct CameraComponent : IComponent {
        CameraComponent() = default;

        CameraComponent(const Camera& camera) :
            camera(camera) {}

        CameraComponent(const Camera& camera, bool active) :
            camera(camera), active(active) {}

        bool dirty{ true };

        Camera camera;
        bool active{ true };
    };

    struct LightComponent : IComponent {
        LightComponent() = default;

        LightComponent(math::Vector3f color, math::Vector3f direction, float intensity) :
            type(LightType::Directional), color(color), direction(direction),
            intensity(intensity) {}

        LightComponent(math::Vector3f color, float intensity, float attenuation) :
            type(LightType::Point), color(color), intensity(intensity), attenuation(attenuation) {}

        LightComponent(math::Vector3f color, math::Vector3f direction, float intensity, float attenuation, float spotPower) :
            type(LightType::Point), color(color), direction(direction),
            intensity(intensity), attenuation(attenuation), spotPower(spotPower) {}

        bool dirty{ true };

        LightType type{ LightType::Directional };
        math::Vector3f color;
        math::Vector3f direction;
        float intensity{ 0.0f };
        float attenuation{ 0.0f };
        float spotPower{ 0.0f };
    };

    struct EnvironmentComponent : IComponent {
        EnvironmentComponent() = default;

        EnvironmentComponent(math::Vector3f ambientLight, uint32_t skyboxTextureResource) :
            ambientLight(ambientLight), skyboxTextureResource(skyboxTextureResource) {}

        bool dirty{ true };

        math::Vector3f ambientLight;
        uint32_t skyboxTextureResource{ ~0u };
    };

}