#ifndef TEXTUREDMODEL_H
#define TEXTUREDMODEL_H

#include "Model.h"

class Mesh;

class TexturedModel : public Model
{
	Texture* m_pTexture;

public:
	TexturedModel(Mesh* pMesh, Texture* pTexture);
	void Render() override;
};

#endif
