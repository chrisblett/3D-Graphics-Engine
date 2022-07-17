#include "Entity.h"
#include "Material.h"
#include "Mesh.h"
#include "Model.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

Entity::Entity(Mesh* pMesh)
	: m_pMesh{ pMesh }
	, m_pShader{nullptr}
	, m_position{ 0.0f, 0.0f, 0.0f }
	, m_rotation{0.0, 0.0f, 0.0f}
	, m_scale{ 1.0f, 1.0f, 1.0f }
{
}

void Entity::Bind(ShaderProgram* pShader) const
{
	m_material.Bind(pShader);
}

void Entity::Render() const
{
	// Render the model
	m_pMesh->Render();
}

glm::mat4 Entity::CalculateModelMatrix() const
{
	// Create translation matrix
	glm::mat4 T = glm::translate(glm::mat4(1.0f), m_position);

	// Create rotation matrices for each axis
	glm::mat4 RX = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 RY = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 RZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Combine rotation matrices
	glm::mat4 R = RX * RY * RZ;

	return T * R * glm::scale(glm::mat4(1.0f), m_scale);
}
