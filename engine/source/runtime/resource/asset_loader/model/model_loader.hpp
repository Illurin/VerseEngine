#pragma once

#include "assimp_loader.hpp"

namespace engine {
    namespace asset {

        struct RawModel {
            RawModelScene scene;
        };

        class ModelLoader {
        public:
            static RawModel Load(std::filesystem::path path) {
                AssimpModelLoader loader(path);
                auto& scene = loader.GetScene();
                return { scene };
            }
        };

    }
}