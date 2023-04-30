#pragma once

#include "runtime/framework/system/system.hpp"

namespace engine {

    class IScene {
    public:
        IScene();
        virtual ~IScene() {}

        // Engine call

        void Start();
        void Update();

        // ECS

        ecs::registry& GetRegistry();
        Entity         CreateEntity();

        // System

        template<typename Type>
        void AddSystem() {
            systems.emplace_back(std::make_unique<Type>());
            systems.back()->pRegistry = &registry;
        }

    private:
        ecs::registry registry;
        std::vector<std::unique_ptr<ISystem>> systems;
    };

}