#pragma once

#include "system.hpp"

namespace engine {

    class ResourceSystem final : public ISystem {
    public:
        ResourceSystem() {}
        ~ResourceSystem();

        void Start() override;
        void Update() override;

    private:
        rhi::Sampler AddSampler(rhi::SamplerCreateInfo);
        void BuildMaterials();

    private:
        std::unordered_map<std::string, rhi::Image>  images;
        std::unordered_map<std::string, rhi::Image>  cubemaps;
        std::vector<rhi::SamplerCreateInfo>          samplerInfos;
        std::vector<rhi::Sampler>                    samplers;
    };

}