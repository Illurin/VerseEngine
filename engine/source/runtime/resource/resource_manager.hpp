#pragma once

#include "data_type/data.hpp"

namespace engine {

    struct TextureResource {
        bool preLoad{ true };

        bool isCubemap{ false };
        std::filesystem::path path;

        rhi::Image image{ nullptr };
    };

    struct MaterialResource {
        bool preLoad{ true };

        MaterialData data{};

        uint32_t diffuseMapResource{ ~0u };
        uint32_t normalMapResource{ ~0u };
        rhi::Buffer uniformBuffer{ nullptr };
        rhi::Sampler sampler{ nullptr };
        rhi::DescriptorSet descriptorSet{ nullptr };
    };

    class ResourceManager {
    public:
        static uint32_t ImportTexture(std::filesystem::path path);
        static uint32_t ImportCubemap(std::filesystem::path path);
        static TextureResource& GetTexture(uint32_t id);

        static uint32_t CreateMaterial(const MaterialData&);
        static MaterialResource& GetMaterial(uint32_t id);

        static void LoadResources();
        static void Clear();

    private:
        inline static std::vector<TextureResource> textureResources;
        inline static std::vector<MaterialResource> materialResources;
    };

}