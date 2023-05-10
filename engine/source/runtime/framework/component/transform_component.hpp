#pragma once

#include "component.hpp"

#include "runtime/math/math.hpp"

namespace engine {

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
            
        bool dirty{ true };
        
        math::Vector3f position;
        math::Vector3f scale;
        math::Vector3f localEulerAngle;
        math::Vector3f globalEulerAngle;
    };

}