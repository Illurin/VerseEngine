#pragma once

#include "raw_model_data.hpp"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace engine {
    namespace asset {

        class AssimpModelLoader {
        public:
            AssimpModelLoader(std::filesystem::path path);

            const RawModelScene& GetScene() const { return scene; }

        private:
            void ProcessNode(const aiScene* scene, aiNode* node);
            RawModelMesh ProcessMesh(const aiScene* scene, aiMesh* node);
            std::vector<RawModelTexture> ProcessTexture(aiMaterial* material, aiTextureType textureType);

            RawModelScene scene;
            std::vector<RawModelTexture> textures;

            std::filesystem::path directory;
        };

    }
}