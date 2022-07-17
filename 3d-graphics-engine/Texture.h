#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>
#include <string>

// Use mipmaps by default and trilinear filtering (GL_LINEAR_MIPMAP_LINEAR)
GLuint LoadTexture(const std::string& filepath, GLfloat filter = GL_LINEAR_MIPMAP_LINEAR);
GLuint LoadCubemap(const std::string& cubeMapDir);

class Texture
{
	GLuint m_textureID;
public:
	Texture();
	Texture(GLuint textureID) : m_textureID{ textureID } {};

	void Bind();
	void Unbind();
	void SetCompareMode(GLint compareMode);

	GLuint GetTextureID() { return m_textureID; }
};

#endif