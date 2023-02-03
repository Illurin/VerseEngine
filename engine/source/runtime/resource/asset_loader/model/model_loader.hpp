#pragma once

#include "assimp_loader.hpp"

namespace engine {
	namespace asset {

		struct VertexData {
			math::Vector3f position;
		};

		struct RawModel {
			std::vector<VertexData> vertices;
			std::vector<uint32_t> indices;
		};

		class ModelLoader {
		public:
			static RawModel Load(const char* path) {
				AssimpModelLoader loader(path);
				auto& meshes = loader.GetMeshes();

				RawModel rawModel;
				for (auto& mesh : meshes) {
					for (auto& vertex : mesh.vertices) {
						VertexData vertexData;
						vertexData.position = vertex.position;
						rawModel.vertices.emplace_back(vertexData);
					}

					for (auto& index : mesh.indices) {
						rawModel.indices.emplace_back(index + rawModel.vertices.size());
					}
				}

				return rawModel;
			}
		};

	}
}