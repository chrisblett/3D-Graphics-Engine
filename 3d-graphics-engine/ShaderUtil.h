#ifndef SHADER_UTIL_H
#define SHADER_UTIL_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <string>
#include <vector>

class ShaderUtil
{
	static std::string LoadShader(const char* filename);
	static GLuint CreateShader(const char* filename, const std::string& shaderStr,
		GLenum shaderType);
	static GLuint CreateProgram(GLuint vertShaderID, GLuint fragShaderID);
	static void PrintLog(GLuint objectID, bool isProgram);

public:
	static GLuint CreateShaderProgram(const char* vertFilename, const char* fragFilename);
};

#endif