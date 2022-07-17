#ifndef MESH_UTIL_H
#define MESH_UTIL_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <array>
#include <vector>

class MeshUtil
{
	static constexpr int VERTICES_PER_FACE = 3;

	static glm::vec3 ComputeNormal(int curFaceVertex, int curVertex, const std::vector<glm::vec3>& positions,
		const std::array<int, VERTICES_PER_FACE>& faceIndices);

	static std::array<int, VERTICES_PER_FACE> GetFaceIndices(const std::vector<GLuint>& indices, int curIndicesIndex);

public:
	static std::vector<glm::vec3> GenerateVertexNormals(const std::vector<glm::vec3>& positions,
		const std::vector<GLuint>& indices);
};

#endif
