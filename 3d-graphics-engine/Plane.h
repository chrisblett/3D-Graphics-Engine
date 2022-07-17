#ifndef PLANE_H
#define PLANE_H

#include "MeshCreator.h"
#include <glm\glm.hpp>

class Mesh;
class ShaderProgram;

class Plane
{
	Mesh* m_mesh;
	glm::vec3 m_position;
	int m_size;

public:
	Plane(int size, const glm::vec3& position = glm::vec3(0, 0, 0));

	void Render();

	int GetSize() const { return m_size; }
	const glm::vec3& GetPosition() const { return m_position; }
};

#endif