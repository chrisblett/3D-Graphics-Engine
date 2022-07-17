#include "DirectionalLight.h"
#include "ShaderProgram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

int DirectionalLight::s_count = 0;

namespace
{
	// The unsigned value for each length of the cuboid that forms the orthographic viewing volume.
	constexpr float LIGHT_MATRIX_DIRECTIONAL_SIZE = 7.0f;

	// The near and far plane values for the directional light's projection matrix
	constexpr float LIGHT_MATRIX_DIRECTIONAL_NEAR_PLANE = 0.1f;
	constexpr float LIGHT_MATRIX_DIRECTIONAL_FAR_PLANE = 12.0f;

	constexpr float DIRECTIONAL_SHADOW_BIAS = 0.001f;

	// How much to scale along the direction vector that will become the 'eye' position
	// for the directional light's view matrix
	constexpr float LIGHT_MATRIX_DIRECTIONAL_POSITIONAL_OFFSET = 6.0f;
}

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity)
	: Light{ color, intensity }
	, m_direction{ glm::normalize(direction) }
{
	// The zero vector is not allowed
	assert(glm::length(direction) > 0.0f && "The zero vector is not a valid direction vector");
	
	m_id = s_count++;
	std::cout << "DirectionalLight [" << m_id << "] created\n";

	// Create data necessary for allowing shadow mapping
	m_shadowData = CreateShadowData(m_direction);
}

DirectionalLight::~DirectionalLight() 
{ 
	std::cout << "DirectionalLight [" << m_id << "] destroyed\n"; 
}

void DirectionalLight::SetUniforms(ShaderProgram* pShader, int shadowIndex)
{
	std::string idStr = std::to_string(m_id);

	pShader->SetUniform("directionalLights[" + idStr + "].light.shadowIndex", shadowIndex);
	pShader->SetUniform("directionalLights[" + idStr + "].light.ambient",  GetAmbient());
	pShader->SetUniform("directionalLights[" + idStr + "].light.diffuse",  GetDiffuse());
	pShader->SetUniform("directionalLights[" + idStr + "].light.specular", GetSpecular());
	pShader->SetUniform("directionalLights[" + idStr + "].direction", m_direction);
}


void DirectionalLight::SetDirection(const glm::vec3& direction)
{
	// Ignore the zero vector if given
	if(glm::length(direction) > 0.0f)
	{
		// Normalise the vector before assigning
		m_direction = glm::normalize(direction);

		// Update this light's view matrix
		m_shadowData->UpdateLightViewMatrix(CreateViewMatrix(m_direction));
	}
}

glm::mat4 CreateViewMatrix(const glm::vec3& dir)
{
	assert(glm::length(dir) > 0.0f &&
		"The zero vector is not a valid direction vector!");

	// Position the view somewhere along the light direction vector
	glm::vec3 pos = -dir * LIGHT_MATRIX_DIRECTIONAL_POSITIONAL_OFFSET;

	// Set target position
	glm::vec3 target = pos + dir;

	// Need to calculate a suitable up vector
	glm::vec3 lightUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// If the light direction and the up vector are colinear,
	// the camera matrix cannot be created, so we need to adjust
	// the up vector in these cases.
	if(dir == glm::vec3(0.0f, -1.0f, 0.0f))
	{
		lightUp = glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if(dir == glm::vec3(0.0f, 1.0f, 0.0f))
	{
		lightUp = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	return glm::lookAt(pos, target, lightUp);
}

ShadowData* DirectionalLight::CreateShadowData(const glm::vec3& dir)
{
	// Create an orthographic projection matrix since directional lights
	// aim to simulate parallel light rays which originate infinitely far away

	// Set the left, right, bottom and top values of the cuboid
	float l = -LIGHT_MATRIX_DIRECTIONAL_SIZE;
	float r = LIGHT_MATRIX_DIRECTIONAL_SIZE;
	float b = -LIGHT_MATRIX_DIRECTIONAL_SIZE;
	float t = LIGHT_MATRIX_DIRECTIONAL_SIZE;

	glm::mat4 lightProjectionMatrix = glm::ortho(l, r, b, t, LIGHT_MATRIX_DIRECTIONAL_NEAR_PLANE,
		LIGHT_MATRIX_DIRECTIONAL_FAR_PLANE);

	// Combine the projection matrix and the view matrix to create the 'light matrix'
	return new ShadowData(lightProjectionMatrix, CreateViewMatrix(dir),
		LIGHT_MATRIX_DIRECTIONAL_NEAR_PLANE, LIGHT_MATRIX_DIRECTIONAL_FAR_PLANE,
		DIRECTIONAL_SHADOW_BIAS, true);
}