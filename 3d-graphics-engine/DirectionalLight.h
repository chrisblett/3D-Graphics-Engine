#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"
#include <glm/glm.hpp>

glm::mat4 CreateViewMatrix(const glm::vec3& dir);

class DirectionalLight : public Light
{
	static int s_count;
	glm::vec3 m_direction;

public:
	DirectionalLight(const glm::vec3& direction, const glm::vec3& color, float intensity);
	~DirectionalLight();

	virtual void SetUniforms(ShaderProgram* pShader, int shadowIndex) override;

	void SetDirection(const glm::vec3& direction);
	const glm::vec3& GetDirection() const { return m_direction; }

private:
	static ShadowData* CreateShadowData(const glm::vec3& dir);
};

#endif
