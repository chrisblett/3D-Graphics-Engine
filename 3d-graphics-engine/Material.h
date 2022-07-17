#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm\glm.hpp>

class Texture;
class ShaderProgram;

class Material
{
	Texture* m_diffuseMap;
	
	glm::vec3 m_ambient{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_diffuse{ 1.0f, 1.0f, 1.0f };
	glm::vec3 m_specular{ 1.0f, 1.0f, 1.0f };
	float m_shininess{ 32.0f };

	bool m_hasTexture;

public:
	Material();
	Material(Texture* diffuseMap);
	void SetAmbient(const glm::vec3& ambient)   { m_ambient = ambient; }
	void SetDiffuse(const glm::vec3& diffuse)   { m_diffuse = diffuse; }
	void SetSpecular(const glm::vec3& specular) { m_specular = specular; }
	void SetShininess(float shininess);

	void SetDiffuseMap(Texture* pDiffuseMap);

	void Bind(ShaderProgram* pShader) const;

	bool HasTexture() const { return m_hasTexture; }
};


#endif
