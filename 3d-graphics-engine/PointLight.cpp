#include "PointLight.h"
#include "ShaderProgram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

int PointLight::s_count = 0;

namespace
{
	// The field of view for the point light's projection matrix
	constexpr float LIGHT_MATRIX_POINT_FOV = 60.0f;

	// The near and far plane values for the point light's projection matrix
	constexpr float LIGHT_MATRIX_POINT_NEAR_PLANE = 0.1f;
	constexpr float LIGHT_MATRIX_POINT_FAR_PLANE = 10.0f;

	constexpr float POINT_SHADOW_BIAS = 0.000075f;
}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float intensity,
	Attenuation attenuation)
	: Light{ color, intensity }
	, m_position{ position }
	, m_attenuation{ attenuation }
{
	m_id = s_count++;
	std::cout << "PointLight [" << m_id << "] created\n";
}

PointLight::~PointLight()
{
	std::cout << "PointLight [" << m_id << "] destroyed\n";
}

void PointLight::SetUniforms(ShaderProgram* pShader, int shadowIndex)
{
	std::string idStr = std::to_string(m_id);

	pShader->SetUniform("pointLights[" + idStr + "].light.shadowIndex", shadowIndex);

	pShader->SetUniform("pointLights[" + idStr + "].light.ambient",  GetAmbient());
	pShader->SetUniform("pointLights[" + idStr + "].light.diffuse",  GetDiffuse());
	pShader->SetUniform("pointLights[" + idStr + "].light.specular", GetSpecular());
	pShader->SetUniform("pointLights[" + idStr + "].position", m_position);

	pShader->SetUniform("pointLights[" + idStr + "].attenuation.quadratic", m_attenuation.quadratic);
	pShader->SetUniform("pointLights[" + idStr + "].attenuation.linear",    m_attenuation.linear);
	pShader->SetUniform("pointLights[" + idStr + "].attenuation.constant",  m_attenuation.constant);
}

void PointLight::CreateShadowData(float aspectRatio)
{
	// Look at the origin from its position
	glm::mat4 lightViewMatrix = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Create the point light's projection matrix
	glm::mat4 lightProjectionMatrix
		= glm::perspective(glm::radians(LIGHT_MATRIX_POINT_FOV), aspectRatio, LIGHT_MATRIX_POINT_NEAR_PLANE,
			LIGHT_MATRIX_POINT_FAR_PLANE);

	m_shadowData = new ShadowData(lightProjectionMatrix, lightViewMatrix,
		LIGHT_MATRIX_POINT_NEAR_PLANE, LIGHT_MATRIX_POINT_FAR_PLANE, POINT_SHADOW_BIAS
	, false);
}

void PointLight::SetPosition(const glm::vec3& position)
{
	m_position = position;

	// Create a new view matrix with the new position
	glm::mat4 lightViewMatrix = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Update this light's view matrix
	if(m_shadowData)
	{
		m_shadowData->UpdateLightViewMatrix(lightViewMatrix);
	}
}