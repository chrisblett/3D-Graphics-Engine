#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <iostream>

class Mesh;
class ShaderProgram;

class Skybox
{
	const Mesh& m_mesh;
	ShaderProgram& m_shaderProgram;
	GLuint m_cubemapTextureID;

public:
	Skybox(const std::string& cubemapDir, const Mesh& cube, ShaderProgram& shader);
	
	const Mesh& GetMesh() { return m_mesh; }
	ShaderProgram& GetShader() { return m_shaderProgram; }

	void PreRender(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
	void Render() const;
};

#endif