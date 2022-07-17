#include "Mesh.h"
#include "Plane.h"
#include "ShaderProgram.h"

Plane::Plane(int size, const glm::vec3& position)
	: m_size{ size }
	, m_mesh{ (MeshCreator::CreateUntexturablePlane(size)) }
{
	// Centre the plane relative to the position given
	float moveAmount = -m_size / 2.0f;
	m_position.x = position.x + moveAmount;
	m_position.y = position.y;
	m_position.z = position.z + moveAmount;
}

void Plane::Render()
{
	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);

	m_mesh->Render(GL_TRIANGLE_STRIP);

	glDisable(GL_PRIMITIVE_RESTART);
}