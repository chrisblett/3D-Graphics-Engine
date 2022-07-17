#include "Debug.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <cassert>
#include <cstddef>
#include <vector>

GLuint g_normalsVaoID;
GLuint g_normalsBufferID;

namespace Debug::Mesh
{
	// This function will return an array of vec3 objects where every 2 elements
	// consist of the starting position and the endpoint of a line that represents 
	// a vertex normal vector. This function will combine the positions and endpoints 
	// array so that each element of 'positions' and 'endpoints' are next to each other,
	// this is how OpenGL expects points to be laid out in order to connect a line between them.
	std::vector<glm::vec3> GenerateNormalLines(const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec3>& endpoints)
	{
		int combinedSize = static_cast<int>(positions.size() + endpoints.size());

		std::vector<glm::vec3> combined;
		combined.resize(combinedSize);

		// Because both arrays must be the same size, 
		// the original size is just half of the combined size
		const int originalSize = static_cast<int>(combinedSize) / 2;

		// i keeps track of the current elem in positions and normals
		// j keeps track of the current elem in combined
		int j = 0;
		for(int i = 0; i < originalSize; ++i)
		{
			combined[j] = positions[i];
			combined[static_cast<std::size_t>(j) + 1] = endpoints[i];

			// Each iteration we are adding two new elements so we need to 
			// advance the index by two.
			j += 2;
		}

		return combined;
	}

	void CreateNormals(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& normals)
	{
		assert(positions.size() == normals.size() && "positions and normals array size must match!");

		// Generate an endpoint array since normal vectors alone have no position
		// This will allow for drawing the normal vector at the vertex position.
		std::vector<glm::vec3> endpoint;
		endpoint.resize(normals.size());

		for(int i = 0; i < endpoint.size(); ++i)
		{
			endpoint[i] = positions[i] + normals[i];
		}

		std::vector<glm::vec3> combined = GenerateNormalLines(positions, endpoint);

		glGenVertexArrays(1, &g_normalsVaoID);
		glBindVertexArray(g_normalsVaoID);

		glGenBuffers(1, &g_normalsBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, g_normalsBufferID);

		// Since combined is a pointer, we need to deference it to access the size of the first element.
		glBufferData(GL_ARRAY_BUFFER, sizeof(combined[0]) * combined.size(),
			&combined[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
}
