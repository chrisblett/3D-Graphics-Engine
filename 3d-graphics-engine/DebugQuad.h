#ifndef DEBUGQUAD_H
#define DEBUGQUAD_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>

class ShaderProgram;

class DebugQuad
{
	ShaderProgram* m_shader;

	GLuint m_VAO;
	GLuint m_VBO;

	// The position and width in NDC space
	glm::vec2 m_pos;
	float m_width;
	float m_height;

	constexpr static float DEFAULT_WIDTH  = 0.25f;
	constexpr static float DEFAULT_HEIGHT = 0.25f;

	// Right of the viewport
	constexpr static float DEFAULT_POS_X = 1.0f - DEFAULT_WIDTH;
	// Top of the viewport						  
	constexpr static float DEFAULT_POS_Y = 1.0f - DEFAULT_HEIGHT;
public:
	DebugQuad();

	void Bind() const;
	void Render(GLsizei windowWidth, GLsizei windowHeight) const;
	void Initialise(ShaderProgram* shader);

	ShaderProgram* GetShader() const { return m_shader; }
};

#endif
