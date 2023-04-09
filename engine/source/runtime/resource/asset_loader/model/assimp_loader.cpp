#include "assimp_loader.hpp"

namespace engine {
	namespace asset {

		AssimpModelLoader::AssimpModelLoader(std::filesystem::path path) {
			Assimp::Importer importer;
			auto scene = importer.ReadFile(path.string().c_str(),
				aiProcess_Triangulate |
				aiProcess_FlipUVs |
				aiProcess_JoinIdenticalVertices |
				aiProcess_GenNormals |
				aiProcess_CalcTangentSpace);
			directory = path.parent_path();
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

		RawModelMesh AssimpModelLoader::ProcessMesh(const aiScene* scene, aiMesh* mesh) {
			RawModelMesh result;

			result.name = mesh->mName.C_Str();

			for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
				RawModelVertexData vertex;
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
					vertex.texCoord = { -1.0f, -1.0f };
				}

				result.vertices.emplace_back(vertex);
			}

			for (std::size_t i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				for (std::size_t j = 0; j < face.mNumIndices; j++)
					result.indices.push_back(face.mIndices[j]);
			}

			if (mesh->mMaterialIndex >= 0) {
				auto mat = scene->mMaterials[mesh->mMaterialIndex];
				auto textures = ProcessTexture(mat, aiTextureType_DIFFUSE);
				if (textures.empty())
					result.material.diffuseMap = std::nullopt;
				else
					result.material.diffuseMap = textures[0];
			}
			
			return result;
		}

		std::vector<RawModelTexture> AssimpModelLoader::ProcessTexture(aiMaterial* mat, aiTextureType type) {
			std::vector<RawModelTexture> textures(mat->GetTextureCount(type));
			for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
				aiString str;
				mat->GetTexture(type, 0, &str);
				auto texturePath = directory / str.C_Str();
				RawModelTexture texture = { texturePath };
				textures[i] = texture;
			}
			return textures;
		}
	}
}