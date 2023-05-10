#pragma once

#include "component.hpp"

namespace engine {

    struct MaterialComponent : IComponent {
        MaterialComponent() = default;

        MaterialComponent(uint32_t resource) : resource(resource) {}

        uint32_t resource{ ~0u };
    };

}