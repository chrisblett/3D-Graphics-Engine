#include "ShaderProgram.h"
#include "ShaderUtil.h"
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

ShaderProgram::ShaderProgram(const char* vertPath, const char* fragPath)
{
	m_programID = ShaderUtil::CreateShaderProgram(vertPath, fragPath);
}

void ShaderProgram::Bind() const
{
	glUseProgram(m_programID);
}

GLint ShaderProgram::GetUniformLocation(const std::string& uniformName) const
{
	return glGetUniformLocation(m_programID, uniformName.c_str());
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& mat4) const
{
	SetUniform(GetUniformLocation(name), mat4);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& vec3) const
{
	GLint loc = GetUniformLocation(name);
	//assert(loc != -1, "Uniform location value is -1");
	
	if(loc == -1)
	{	
		/*
		std::cout << "Warning: Uniform location value for uniform '" << name
			<< "' in program [" << m_programID << "] is -1\n";
		*/
	}
	SetUniform(loc, vec3);
}

void ShaderProgram::SetUniform(const std::string& name, float f) const
{
	SetUniform(GetUniformLocation(name), f);
}

void ShaderProgram::SetUniform(const std::string& name, int i) const
{
	SetUniform(GetUniformLocation(name), i);
}

void ShaderProgram::SetUniform(GLint uniformLocation, const glm::mat4& mat4) const
{
	glProgramUniformMatrix4fv(m_programID, uniformLocation, 1, GL_FALSE, glm::value_ptr(mat4));
}

void ShaderProgram::SetUniform(GLint uniformLocation, const glm::vec3& vec3) const
{
	glProgramUniform3fv(m_programID, uniformLocation, 1, glm::value_ptr(vec3));
}

void ShaderProgram::SetUniform(GLint uniformLocation, float f) const
{
	glProgramUniform1f(m_programID, uniformLocation, f);
}

void ShaderProgram::SetUniform(GLint uniformLocation, int i) const
{
	glProgramUniform1i(m_programID, uniformLocation, i);
}