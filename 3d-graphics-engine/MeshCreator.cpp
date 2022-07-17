#include "Mesh.h"
#include "MeshCreator.h"
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <cassert>
#include <iostream>
#include <vector>

Mesh* MeshCreator::CreateCube()
{
	glm::vec3 positions[]
	{
		// front face
		glm::vec3(-1.0f,  1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f,  1.0f, 1.0f),

		// back face
		glm::vec3(1.0f,  1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f),

		// top face
		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f,  1.0f),
		glm::vec3(1.0f, 1.0f,  1.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),

		// bottom face
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),

		// left face
		glm::vec3(-1.0f,  1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f),

		// right face
		glm::vec3(1.0f,  1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f,  1.0f, -1.0f)
	};

	GLuint indices[]
	{
		// front face
		0, 1, 2,
		2, 3, 0,

		// back face
		4, 5, 6,
		6, 7, 4,

		// top face
		 8, 9, 10,
		10, 11, 8,

		// bottom
		12, 13, 14,
		14, 15, 12,

		// left
		16, 17, 18,
		18, 19, 16,

		// right
		20, 21, 22,
		22, 23, 20
	};

	std::vector<glm::vec3> positionsVec(std::begin(positions), std::end(positions));
	std::vector<GLuint> indicesVec(std::begin(indices), std::end(indices));

	return new Mesh(positionsVec, indicesVec);
}

Mesh* MeshCreator::CreatePyramid()
{
	// Define all the vertex position attributes for a pyramid.
	// Some vertices share the same position, but have a different texture coordinate.
	// There should be no duplicate vertices (vertices with the same attributes).

	glm::vec3 positions[]
	{
		// base
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),

		// front
		glm::vec3(0.0f,  1.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 1.0f),

		// back
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),

		// left
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f,  1.0f),

		// right
		glm::vec3(0.0f,  1.0f,  0.0f),
		glm::vec3(1.0f, -1.0f,  1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f)
	};

	glm::vec2 texCoords[]
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),

		glm::vec2(0.5f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),

		glm::vec2(0.5f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),

		glm::vec2(0.5f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),

		glm::vec2(0.5f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f)
	};

	GLuint indices[]
	{
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		7, 8, 9,
		10, 11, 12,
		13, 14, 15
	};

	std::vector<glm::vec3> positionsVec(std::begin(positions), std::end(positions));
	std::vector<glm::vec2> texCoordsVec(std::begin(texCoords), std::end(texCoords));
	std::vector<GLuint> indicesVec(std::begin(indices), std::end(indices));

	return new Mesh(positionsVec, texCoordsVec, indicesVec);
}

// The amount of repeated textures on the plane is texturesSquared^texturesSquared
Mesh* MeshCreator::CreatePlane(int texturesSquared)
{
	std::vector<glm::vec3> positions
	{
		glm::vec3(-1.0f, 0.0f,  1.0f),
		glm::vec3(1.0f, 0.0f,  1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, -1.0f),
	};

	std::vector<glm::vec2> texCoords
	{
		glm::vec2(0, 0),
		glm::vec2(texturesSquared, 0),
		glm::vec2(texturesSquared, texturesSquared),
		glm::vec2(0, texturesSquared),
	};

	std::vector<GLuint> indices
	{
		0, 1, 3,
		3, 1, 2
	};

	return new Mesh(positions, texCoords, indices);
}


Mesh* MeshCreator::CreateUntexturablePlane(int size)
{
	const int QUAD_SIZE = 1;

	// The total number of vertices needed to create a single triangle strip/row
	const float verticesPerRow = 2 * (size + 1);

	// When OpenGL sees this value in the index buffer, it knows
	// to create a new primitive, this is used at the end of each row
	// of the triangle strip
	const GLuint primitiveRestartIndex = 0xFFFF;
	
	// We can precompute these values
	// Add the size because a primitive restart index is inserted at the end of each row
	const int numPositions = (verticesPerRow / 2) * (verticesPerRow / 2);
	const int numNormals = numPositions;
	const int numIndices = verticesPerRow * size + size;
	
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;

	positions.reserve(numPositions);
	normals.reserve(numNormals);
	indices.reserve(numIndices);

	const float MAX_POSITIONS = positions.capacity();
	const float MAX_NORMALS = MAX_POSITIONS;
	const float MAX_INDICES = indices.capacity();

	// Keeps track of the total vertices
	int vertCount = 0;
	for(int z = 0; z < size; ++z)
	{
		for(int x = 0; x < size + 1; ++x)
		{
			float xPos = 0;
			float zPos = 0;

			// Calculate the vertex data for the top most vertex in the row
			if(z == 0)
			{				
				xPos = x * QUAD_SIZE;
				zPos = z * QUAD_SIZE;

				positions.push_back(glm::vec3(xPos, 0.0f, zPos));
				normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
				indices.push_back(vertCount++);
			}
			// To avoid vertex duplication, the bottom vertices in each row are
			// shared by the row below it
			else if(z == 1)
			{

				float difference = verticesPerRow - (x + 1);
				indices.push_back(vertCount - difference);
			}
			else
			{
				// After the second row, calculate the index of the shared vertex in the previous row.
				float difference = verticesPerRow / 2;
				indices.push_back(vertCount - difference);
			}

			// Calculate the vertex data for the bottom vertex in the row
			xPos = x * QUAD_SIZE;
			zPos = (z + 1) * QUAD_SIZE;

			positions.push_back(glm::vec3(xPos, 0.0f, zPos));
			normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			indices.push_back(vertCount++);
		}
		indices.push_back(primitiveRestartIndex);
	}

	assert(positions.size() == MAX_POSITIONS, "Number of positions must equal " + MAX_POSITIONS);
	assert(normals.size() == MAX_NORMALS, "Number of normals must equal " + MAX_NORMALS);
	assert(indices.size() == MAX_INDICES, "Number of indices must equal " + MAX_INDICES);

	return new Mesh(positions, normals, indices);
}
