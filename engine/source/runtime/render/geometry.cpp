#include "geometry.hpp"

namespace engine {

	ObjectVertexData Geometry::Midpoint(const ObjectVertexData& lhs, const ObjectVertexData& rhs) {
		ObjectVertexData vertex;
		vertex.localPos = 0.5f * (lhs.localPos + rhs.localPos);
		vertex.normal   = 0.5f * (lhs.normal + rhs.normal);
		vertex.texCoord = 0.5f * (lhs.texCoord + rhs.texCoord);
		return vertex;
	}

	Geometry::MeshData Geometry::Subdivide(const MeshData& meshData) {
		MeshData result;

		//       v1
		//       *
		//      / \
		//     /   \
		//  m0*-----*m1
		//   / \   / \
		//  /   \ /   \
		// *-----*-----*
		// v0    m2     v2

		uint32_t triangleCount = static_cast<uint32_t>(meshData.indices.size()) / 3;
		for (uint32_t i = 0; i < triangleCount; i++) {
			ObjectVertexData v0 = meshData.vertices[meshData.indices[i * 3]];
			ObjectVertexData v1 = meshData.vertices[meshData.indices[i * 3 + 1]];
			ObjectVertexData v2 = meshData.vertices[meshData.indices[i * 3 + 2]];

			ObjectVertexData m0 = Midpoint(v0, v1);
			ObjectVertexData m1 = Midpoint(v1, v2);
			ObjectVertexData m2 = Midpoint(v0, v2);

			result.vertices.emplace_back(v0);
			result.vertices.emplace_back(v1);
			result.vertices.emplace_back(v2);
			result.vertices.emplace_back(m0);
			result.vertices.emplace_back(m1);
			result.vertices.emplace_back(m2);

			result.indices.emplace_back(i * 6 + 0);
			result.indices.emplace_back(i * 6 + 3);
			result.indices.emplace_back(i * 6 + 5);

			result.indices.emplace_back(i * 6 + 3);
			result.indices.emplace_back(i * 6 + 4);
			result.indices.emplace_back(i * 6 + 5);

			result.indices.emplace_back(i * 6 + 5);
			result.indices.emplace_back(i * 6 + 4);
			result.indices.emplace_back(i * 6 + 2);

			result.indices.emplace_back(i * 6 + 3);
			result.indices.emplace_back(i * 6 + 1);
			result.indices.emplace_back(i * 6 + 4);
		}

		return result;
	}

	Geometry::MeshData Geometry::CreateGeosphere(float radius, uint32_t subdivisionCount) {
		MeshData meshData;
		subdivisionCount = (subdivisionCount > 6) ? 6 : subdivisionCount;

		// Subdivide icosahedron

		const float x = 0.525731f;
		const float z = 0.850651f;

		std::vector<math::Vector3f> pos = {
			math::Vector3f(-x,   0.0f,  z   ), math::Vector3f(x,     0.0f,  z   ),
			math::Vector3f(-x,   0.0f, -z   ), math::Vector3f(x,     0.0f, -z   ),
			math::Vector3f(0.0f,  z,    x   ), math::Vector3f(0.0f,  z,    -x   ),
			math::Vector3f(0.0f, -z,    x   ), math::Vector3f(0.0f, -z,    -x   ),
			math::Vector3f(z,     x,    0.0f), math::Vector3f(-z,    x,     0.0f),
			math::Vector3f(z,    -x,    0.0f), math::Vector3f(-z,   -x,     0.0f)
		};

		std::vector<UINT> indices = {
			1,  4,  0,    4,  9, 0,    4, 5,  9,    8, 5, 4,     1,  8, 4,
			1,  10, 8,    10, 3, 8,    8, 3,  5,    3, 2, 5,     3,  7, 2,
			3,  10, 7,    10, 6, 7,    6, 11, 7,    6, 0, 11,    6,  1, 0,
			10, 1,  6,    11, 0, 9,    2, 11, 9,    5, 2, 9,     11, 2, 7
		};

		meshData.vertices.resize(12);
		meshData.indices = indices;

		for (uint32_t i = 0; i < 12; i++)
			meshData.vertices[i].localPos = pos[i];

		for (uint32_t i = 0; i < subdivisionCount; i++)
			meshData = Subdivide(meshData);

		for (UINT i = 0; i < meshData.vertices.size(); i++) {

			// Project vertices onto a sphere

			meshData.vertices[i].normal = meshData.vertices[i].localPos.normalized();
			meshData.vertices[i].localPos = radius * meshData.vertices[i].normal;

			// Calculate texture coords

			float theta = atan2f(meshData.vertices[i].localPos.z(), meshData.vertices[i].localPos.x());

			if (theta < 0.0f) theta += math::Pi<float>();

			float phi = acosf(meshData.vertices[i].localPos.y() / radius);

			meshData.vertices[i].texCoord = math::Vector2f(theta / (2.0f * math::Pi<float>()),
			                                               phi   /         math::Pi<float>());

			//meshData.vertices[i].tangent.x = -radius * sinf(phi) * sinf(theta);
			//meshData.vertices[i].tangent.y = 0.0f;
			//meshData.vertices[i].tangent.z = +radius * sinf(phi) * cosf(theta);

			//meshData.vertices[i].tangent = glm::normalize(meshData.vertices[i].tangent);
		}
		return meshData;
	}

}