#include "Material.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include <GL\glew.h>

Material::Material()
	: m_diffuseMap(nullptr)
{
}

Material::Material(Texture* diffuseMap)
	: m_diffuseMap(diffuseMap)
{
}

void Material::SetDiffuseMap(Texture* pDiffuseMap)
{
	m_diffuseMap = pDiffuseMap;
	m_hasTexture = true;
}

void Material::SetShininess(float shininess)
{
	m_shininess = shininess;
}

void Material::Bind(ShaderProgram* pShader) const
{
	if(HasTexture())
	{
		// Bind the diffuse map
		glActiveTexture(GL_TEXTURE0);
		m_diffuseMap->Bind();
	}

	pShader->SetUniform("material.ambient", m_ambient);
	pShader->SetUniform("material.diffuse", m_diffuse);
	pShader->SetUniform("material.specular", m_specular);
	pShader->SetUniform("material.shininess", m_shininess);
}
