#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"
#include <glm/glm.hpp>

class ShaderProgram;

class PointLight : public Light
{
public:
	struct Attenuation
	{
		float quadratic;
		float linear;
		float constant;
	} m_attenuation;

	static constexpr Attenuation DEFAULT_ATTENUATION{ 0.07f, 0.14f, 1.0f};

private:
	static int s_count;
	glm::vec3 m_position;

public:
	PointLight() = default;
	PointLight(const glm::vec3& position, const glm::vec3& color, float intensity,
		Attenuation attenuation = DEFAULT_ATTENUATION);

	~PointLight();

	virtual void SetUniforms(ShaderProgram* pShader, int shadowIndex) override;

	void CreateShadowData(float aspectRatio);
	void SetPosition(const glm::vec3& position);
	void SetDiffuse(const glm::vec3& diffuse)   { m_diffuse = diffuse; }
	void SetSpecular(const glm::vec3& specular) { m_specular = specular; }

	const glm::vec3&   GetPosition()    const { return m_position; }
	const Attenuation& GetAttenuation() const { return m_attenuation; }
};

#endif
