#pragma once

#include "ecs.hpp"

namespace engine {

    class Entity {
    public:
        Entity(ecs::registry& registry) : registry(registry) {
            entity = registry.create();
        }

        // Registry

        ecs::entity GetEntity() { return entity; }
        ecs::registry& GetRegistry() { return registry; }
        static ecs::registry& GetGlobalRegistry() { return globalRegistry; }

        // Entity Component

        template<typename Type, typename... Args>
        decltype(auto) AddComponent(Args&&... args) {
            return registry.emplace<Type>(entity, std::forward<Args>(args)...);
        }
        
        template<typename... Type>
        decltype(auto) GetComponents(const Entity& entity) {
            return registry.get<Type...>(entity);
        }

        // Singleton Component

        template<typename Type, typename... Args>
        static decltype(auto) AddSingletonComponent(Args&& ...args) {
            return globalRegistry.ctx().emplace<Type>(std::forward<Args>(args)...);
        }

        template<typename... Type>
        static decltype(auto) GetSingletonComponents() {
            return globalRegistry.ctx().get<Type...>();
        }

    private:
        ecs::registry& registry;
        ecs::entity entity;

        inline static ecs::registry globalRegistry;
    };

}