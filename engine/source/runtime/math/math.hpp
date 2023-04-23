#pragma once

#include "projection.hpp"

namespace engine {
    namespace math {

        template<typename T>
        T Pi() {
            return static_cast<T>(EIGEN_PI);
        }

        template<typename T>
        T UpperAlign(T size, T alignSize) {
            return (size + alignSize - 1) & (~(alignSize - 1));
        }

    }
}