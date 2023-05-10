#pragma once

#include <cstdint>

namespace engine {

    struct IComponent {
        uint32_t id{ ~0u };
    };

}