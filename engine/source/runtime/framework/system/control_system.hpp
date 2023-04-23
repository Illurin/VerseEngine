#pragma once

#include "system.hpp"

namespace engine {

    class ControlSystem final : public ISystem {
    public:
        ControlSystem() {}
        ~ControlSystem() {}

        void Start(ecs::registry& registry) override;
        void Update(ecs::registry& registry) override;
    };

}