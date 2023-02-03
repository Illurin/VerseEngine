#pragma once

#include "runtime/math/math.hpp"
#include "runtime/system/file_system/path.hpp"

#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

namespace engine {
	namespace asset {

		class AssimpModelLoader {
		public:
			AssimpModelLoader(const char* path);

			struct Vertex {
				math::Vector3f position;
				math::Vector3f normal;
				math::Vector3f tangent;
				math::Vector2f texCoord;
			};

			struct Texture {
				std::string path;
			};

			struct Material {
				Texture* diffuseMap;
			};

			struct Mesh {
				std::string name;
				std::vector<Vertex> vertices;
				std::vector<uint32_t> indices;
				Material material{ nullptr };
			};

			const std::vector<Mesh>& GetMeshes() const { return meshes; }
			const std::vector<Texture>& GetTextures() const { return textures; }
			const std::vector<Material>& GetMaterials() const { return materials; }

		private:
			void ProcessNode(const aiScene* scene, aiNode* node);
			Mesh ProcessMesh(const aiScene* scene, aiMesh* node);
			Texture* ProcessTexture(aiMaterial* material, aiTextureType textureType);

			std::vector<Mesh> meshes;
			std::vector<Texture> textures;
			std::vector<Material> materials;

			std::string directory;
		};

	}
}