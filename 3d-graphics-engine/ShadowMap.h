#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <GL\glew.h>
#include "Texture.h"

class ShadowMap
{
public:
	static constexpr GLsizei DEFAULT_WIDTH  = 1024;
	static constexpr GLsizei DEFAULT_HEIGHT = 1024;

	// Stores the initial slot number to start binding shadow maps from
	static constexpr int  START_TEXTURE_SLOT = 1;

private:
	Texture* m_texture;

	GLsizei m_width;
	GLsizei m_height;
	GLuint m_shadowFrameBufferID;

public:
	ShadowMap(GLsizei width = DEFAULT_WIDTH, GLsizei height = DEFAULT_HEIGHT);
	~ShadowMap();

	// Bind for writing
	void Write() const;

	// Bind the shadow map texture for reading
	void Read(GLenum texture) const;

	void SetTextureCompareMode(GLint compareMode) const;

private:
	void Init();
};

#endif
