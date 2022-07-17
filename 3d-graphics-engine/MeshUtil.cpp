#include "MeshUtil.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <array>
#include <cstddef>
#include <iostream>
#include <vector>

std::vector<glm::vec3> MeshUtil::GenerateVertexNormals(const std::vector<glm::vec3>& positions, const std::vector<GLuint>& indices)
{
	std::vector<glm::vec3> normals;
	normals.resize(positions.size());

	// Keep track of the current index in the normals array
	int curNormalsIndex = 0;

	// Keeps track of the current index in the indices array,
	// this will point to the first vertex index of a face
	int curIndicesIndex = 0;

	// This will count from the first vertex up until the last vertex,
	// this will be incremented by one every time a vertex is processed.
	int count = 0;

#if SHOW_NORMAL_OUTPUT
	std::cout << "Generating surface normals\n";
#endif

	while(curIndicesIndex < indices.size())
	{
		// Get the indices for the current face
		auto faceIndices = MeshUtil::GetFaceIndices(indices, curIndicesIndex);

		// Generate a normal for each vertex in the face
		for(int i = 0; i < VERTICES_PER_FACE; ++i)
		{
			// Stores the index of the current vertex in the positions array
			const int curVertex = faceIndices[i];

			// This means we have already seen the vertex before, skip
			// These values should be the same if we always visit unique vertices
			if(count != curVertex)
			{
#if SHOW_NORMAL_OUTPUT
				std::cout << "Vertex " << curVertex << " already has a normal, skipping\n";
#endif
				continue;
			}

			glm::vec3& normal = normals[curNormalsIndex];

			normal = ComputeNormal(i, curVertex, positions, faceIndices);

#if SHOW_NORMAL_OUTPUT
			// Debug output
			std::cout << "Vertex " << curVertex 
				<< " normal generated (" << normal.x << ", " << normal.y << ", " << normal.z << ")\n";
#endif
			count++;
			curNormalsIndex++;
		}

		// Go to the start of the next face in the indices
		curIndicesIndex += VERTICES_PER_FACE;
	}

	return normals;
}

// This function will generate a normal for a vertex using the vertices of the face
// that vertex is part of.
// curFaceVertex is a counter keeping track of the current vertex within a face
// and curVertex is the index of the current vertex of the overall mesh
glm::vec3 MeshUtil::ComputeNormal(int curFaceVertex, int curVertex, const std::vector<glm::vec3>& positions,
	const std::array<int, VERTICES_PER_FACE>& faceIndices)
{
	// The vertex position of the current vertex
	const glm::vec3& vertexPos = positions[curVertex];

	int nextVertexInFaceIndex = (curFaceVertex + 1) % VERTICES_PER_FACE;
	int prevVertexInFaceIndex = (curFaceVertex + 2) % VERTICES_PER_FACE;

	glm::vec3 first  = positions[faceIndices[nextVertexInFaceIndex]] - vertexPos;
	glm::vec3 second = positions[faceIndices[prevVertexInFaceIndex]] - vertexPos;

	return{ glm::normalize(glm::cross(first, second)) };
}

std::array<int, MeshUtil::VERTICES_PER_FACE> MeshUtil::GetFaceIndices(const std::vector<GLuint>& indices, int curIndicesIndex)
{
	std::array<int, VERTICES_PER_FACE> faceIndices;

	for(int i = 0; i < VERTICES_PER_FACE; ++i)
	{
		faceIndices[i] = indices[static_cast<std::size_t>(curIndicesIndex) + i];
	}

	return faceIndices;
}