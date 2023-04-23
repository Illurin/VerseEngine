#pragma once

#include "system.hpp"

namespace engine {

    class ResourceSystem final : public ISystem {
    public:
        ResourceSystem() {}
        ~ResourceSystem();

        void Start(ecs::registry& registry) override;
        void Update(ecs::registry& registry) override;

    private:
        void BuildMaterials(ecs::registry& registry);

    private:
        std::unordered_map<std::string, rhi::Image>  images;
        std::unordered_map<std::string, rhi::Image>  cubemaps;
        std::vector<rhi::Sampler>                    samplers;
    };

}