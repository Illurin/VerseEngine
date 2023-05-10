#pragma once

#include "runtime/render/render_data.hpp"

namespace engine {

    struct TextureResource {
        bool preLoad{ true };

        bool isCubemap{ false };
        std::filesystem::path path;
        rhi::SamplerCreateInfo samplerInfo{};

        rhi::Image image{ nullptr };
        rhi::Sampler sampler{ nullptr };
    };

    struct MaterialResource {
        bool preLoad{ true };

        std::string name;

        MaterialConstants constants;

        uint32_t diffuseMap{ ~0u };
        uint32_t normalMap{ ~0u };

        rhi::Buffer uniformBuffer{ nullptr };
        rhi::DescriptorSet descriptorSet{ nullptr };
    };

    class ResourceManager {
    public:
        static uint32_t          ImportTexture  (std::filesystem::path path);
        static uint32_t          ImportCubemap  (std::filesystem::path path);
        static void              SetImageSampler(uint32_t texture, const rhi::SamplerCreateInfo&);
        static TextureResource&  GetTexture     (uint32_t id);

        static uint32_t          CreateMaterial (std::string name, MaterialConstants constants,
                                                 uint32_t diffuseMap = ~0u, uint32_t normalMap = ~0u);
        static MaterialResource& GetMaterial    (uint32_t id);

        static void              LoadResources  ();
        static void              Clear          ();

    private:
        inline static std::vector<TextureResource> textureResources;
        inline static std::vector<MaterialResource> materialResources;
    };

}