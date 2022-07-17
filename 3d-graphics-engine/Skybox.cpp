#include "Skybox.h"
#include "Mesh.h"
#include "MeshCreator.h"
#include "Texture.h"
#include "ShaderProgram.h"

Skybox::Skybox(const std::string& cubemapDir, const Mesh& cube, ShaderProgram& shader)
	: m_mesh(cube)
	, m_shaderProgram(shader)
	, m_cubemapTextureID(LoadCubemap(cubemapDir))
{
}

void Skybox::PreRender(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
{
	m_shaderProgram.Bind();

	m_shaderProgram.SetUniform("projection", projectionMatrix);
	m_shaderProgram.SetUniform("view", viewMatrix);
}

void Skybox::Render() const
{
	// Since we want to see the inside faces of the cube change the winding order
	// temporarily when drawing the skybox
	glFrontFace(GL_CW);

	// Disable writing to the depth buffer since we want the skybox to appear
	// behind everything else
	glDepthMask(GL_FALSE);

	m_mesh.Render();

	// Revert changes in order to draw everything else
	glFrontFace(GL_CCW);
	glDepthMask(GL_TRUE);
}