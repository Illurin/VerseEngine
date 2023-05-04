#pragma once

#include "runtime/math/math.hpp"

namespace engine {

    struct TransformData {
        math::Vector3f position;
        math::Vector3f scale;
        math::Vector3f localEulerAngle;
        math::Vector3f globalEulerAngle;
    };

}