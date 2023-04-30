#pragma once

#include "runtime/framework/component/component.hpp"
#include "runtime/framework/ecs/entity.hpp"

namespace engine {

    class IScene;

    class ISystem {
    public:
        virtual ~ISystem() {}

        // Engine call

        virtual void Start() {}
        virtual void Update() {}

        // ECS

        ecs::registry& GetRegistry() { return *pRegistry; }

        template<typename Type, typename... Other>
        decltype(auto) GetEntities() { return pRegistry->view<Type, Other...>(); }

    private:
        friend class IScene;
        ecs::registry* pRegistry;
    };

}