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

            const std::vector<RawModelMesh>& GetMeshes() const { return meshes; }

        private:
            void ProcessNode(const aiScene* scene, aiNode* node);
            RawModelMesh ProcessMesh(const aiScene* scene, aiMesh* node);
            std::vector<RawModelTexture> ProcessTexture(aiMaterial* material, aiTextureType textureType);

            std::vector<RawModelMesh> meshes;
            std::vector<RawModelTexture> textures;

            std::filesystem::path directory;
        };

    }
}