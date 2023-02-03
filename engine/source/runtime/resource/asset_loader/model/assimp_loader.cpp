#include "assimp_loader.hpp"

namespace engine {
	namespace asset {

		AssimpModelLoader::AssimpModelLoader(const char* path) {
			Assimp::Importer importer;
			auto scene = importer.ReadFile(path, 
				aiProcess_JoinIdenticalVertices |
				aiProcess_GenNormals |
				aiProcess_CalcTangentSpace |
				aiProcess_FlipWindingOrder);
			directory = Path(path).GetParentPath();
			ProcessNode(scene, scene->mRootNode);
		}

		void AssimpModelLoader::ProcessNode(const aiScene* scene, aiNode* node) {
			for (uint32_t i = 0; i < node->mNumMeshes; i++) {
				auto mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.emplace_back(ProcessMesh(scene, mesh));
			}

			for (uint32_t i = 0; i < node->mNumChildren; i++) {
				ProcessNode(scene, node->mChildren[i]);
			}
		}

		AssimpModelLoader::Mesh AssimpModelLoader::ProcessMesh(const aiScene* scene, aiMesh* mesh) {
			Mesh result;

			result.name = mesh->mName.C_Str();

			for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
				Vertex vertex;
				vertex.position[0] = mesh->mVertices[i].x;
				vertex.position[1] = mesh->mVertices[i].y;
				vertex.position[2] = mesh->mVertices[i].z;

				vertex.normal[0] = mesh->mNormals[i].x;
				vertex.normal[1] = mesh->mNormals[i].y;
				vertex.normal[2] = mesh->mNormals[i].z;

				vertex.tangent[0] = mesh->mTangents[i].x;
				vertex.tangent[1] = mesh->mTangents[i].y;
				vertex.tangent[2] = mesh->mTangents[i].z;

				if (mesh->mTextureCoords[0]) {
					vertex.texCoord[0] = mesh->mTextureCoords[0][i].x;
					vertex.texCoord[1] = mesh->mTextureCoords[0][i].y;
				}
				else {
					vertex.texCoord = { 0.0f, 0.0f };
				}

				result.vertices.emplace_back(vertex);
			}

			for (size_t i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (size_t j = 0; j < face.mNumIndices; j++)
					result.indices.push_back(face.mIndices[j]);
			}

			if (mesh->mMaterialIndex >= 0) {
				auto mat = scene->mMaterials[mesh->mMaterialIndex];
				result.material.diffuseMap = ProcessTexture(mat, aiTextureType_DIFFUSE);
			}
			
			return result;
		}

		AssimpModelLoader::Texture* AssimpModelLoader::ProcessTexture(aiMaterial* mat, aiTextureType type) {
			if (mat->GetTextureCount(type) > 0) {
				aiString str;
				mat->GetTexture(type, 0, &str);
				auto texturePath = directory + str.C_Str();

				for (auto& texture : textures) {
					if (texture.path == texturePath) {
						return &texture;
					}
				}

				Texture texture = { texturePath };
				textures.emplace_back(texture);
				return &textures.back();
			}
			return nullptr;
		}

	}
}