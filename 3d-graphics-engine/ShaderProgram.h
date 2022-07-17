#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>

class ShaderProgram
{	
	// This class should not implement a destructor that involves a call
	// to glDeleteProgram() due to the chance of copies of an instance being made
	// It is currently the responsibility of the ShaderManager class to manage this.
	friend class ShaderManager;

	GLuint m_programID;

	void SetUniform(GLint uniformLocation, const glm::mat4& mat4) const;
	void SetUniform(GLint uniformLocation, const glm::vec3& vec3) const;
	void SetUniform(GLint uniformLocation, float f) const;
	void SetUniform(GLint uniformLocation, int i) const;

	GLint GetUniformLocation(const std::string& uniformName) const;
public:
	ShaderProgram(const char* vertPath, const char* fragPath);

	void Bind() const;

	// Returns the uniform value, assuming the uniform is of type int
	int GetUniformiValue(const char* uniformName)
	{
		int result = 0;
		glGetUniformiv(m_programID, GetUniformLocation(uniformName), &result);
		return result;
	}

	void SetUniform(const std::string& name, const glm::mat4& mat4) const;
	void SetUniform(const std::string& name, const glm::vec3& vec3) const;
	void SetUniform(const std::string& name, float f) const;
	void SetUniform(const std::string& name, int i) const;
};

#endif