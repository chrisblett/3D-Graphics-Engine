#include "Mesh.h"
#include "Texture.h"
#include "TexturedModel.h"
#include <GL\glew.h>
#include <cassert>

TexturedModel::TexturedModel(Mesh* pMesh, Texture* pTexture)
	: Model{ pMesh }
	, m_pTexture{ pTexture }
{
	assert(m_pTexture && "Texture is NULL");
}

void TexturedModel::Render()
{
	// Assign a texture unit first before binding
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, m_pTexture->GetTextureID());
	m_pMesh->Render(GL_TRIANGLES);
}