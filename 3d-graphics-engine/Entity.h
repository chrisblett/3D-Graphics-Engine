#ifndef ENTITY_H
#define ENTITY_H

#include "Material.h"
#include <glm\glm.hpp>
#include <string>

class Mesh;
class Renderer;
class ShaderProgram;

class Entity
{
	Mesh* m_pMesh;
	ShaderProgram* m_pShader;
	Material m_material;
	
	glm::vec3 m_position;
	
	// Represents rotation about each axis in degrees, not a vector
	struct Rotation
	{
		float x;
		float y;
		float z;
	} m_rotation;

	glm::vec3 m_scale;

public:
	Entity(Mesh* pMesh);

	void Bind(ShaderProgram* pShader) const;

	void Render() const;
	glm::mat4 CalculateModelMatrix() const;

	const glm::vec3& GetPosition() const { return m_position; }
	glm::vec3 GetRotation() const { return { m_rotation.x, m_rotation.y, m_rotation.z }; }
	const glm::vec3& GetScale()    const { return m_scale;    }
	
	void SetPosition(const glm::vec3& pos) { m_position = pos; }
	void SetRotation(float angleX, float angleY, float angleZ)
	{
		m_rotation.x = angleX;
		m_rotation.y = angleY;
		m_rotation.z = angleZ;
	}

	// Only allow for uniform scaling since the engine doesn't
	// support non-uniform scaling
	void SetScale(float factor)  
	{
		m_scale.x = factor;
		m_scale.y = factor;
		m_scale.z = factor;
	}

	void SetShader(ShaderProgram* pShader) { m_pShader = pShader; }
	Material* GetMaterial() { return &m_material; }
	ShaderProgram* GetShader() const { return m_pShader; }
};

#endif
