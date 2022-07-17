#include "DebugQuad.h"
#include "ShaderProgram.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

DebugQuad::DebugQuad()
	: m_shader(nullptr)
	, m_pos(DEFAULT_POS_X, DEFAULT_POS_Y)
	, m_width(DEFAULT_WIDTH)
	, m_height(DEFAULT_HEIGHT)
{
	GLfloat quadVerts[]
	{
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts, GL_STATIC_DRAW);

	// Positional and texture coordinate data are interleaved
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}

void DebugQuad::Render(GLsizei windowWidth, GLsizei windowHeight) const
{
	glBindVertexArray(m_VAO);

	glViewport(0, 0, windowWidth, windowHeight);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
}

void DebugQuad::Bind() const
{
	m_shader->Bind();
}

void DebugQuad::Initialise(ShaderProgram* shader)
{
	m_shader = shader;
	m_shader->SetUniform("scale", glm::scale(glm::mat4(1.0f), glm::vec3(m_width, m_height, 0.0f)));
	m_shader->SetUniform("translation", glm::translate(glm::mat4(1.0f), glm::vec3(m_pos.x, m_pos.y, 0.0f)));
}