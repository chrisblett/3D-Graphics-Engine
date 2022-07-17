#include "ShaderUtil.h"
#include <fstream>
#include <iostream>
#include <string>

GLuint ShaderUtil::CreateShaderProgram(const char* vertFilename,
	const char* fragFilename)
{
	// Retrieve the shader source from the file
	std::string vertShaderStr = LoadShader(vertFilename);
	std::string fragShaderStr = LoadShader(fragFilename);

	// Create the shader objects
	GLuint vertShaderID = CreateShader(vertFilename, vertShaderStr, GL_VERTEX_SHADER);
	GLuint fragShaderID = CreateShader(vertFilename, fragShaderStr, GL_FRAGMENT_SHADER);

	GLuint shaderProgramID = CreateProgram(vertShaderID, fragShaderID);

	// These shaders are no longer needed after they have been
	// linked into the program, therefore they can be deleted. 
	glDetachShader(shaderProgramID, vertShaderID);
	glDetachShader(shaderProgramID, fragShaderID);

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	return shaderProgramID;
}

std::string ShaderUtil::LoadShader(const char* filename)
{
	std::string source;

	// Allow input to be read from the stream
	std::ifstream stream(filename, std::ios::in);

	if(stream)
	{
		while(!stream.eof())
		{
			// Read each line of the file
			std::string line;
			std::getline(stream, line);

			// Append each line to the source string
			source.append(line + '\n');
		}
		stream.close();
	}
	else
	{
		std::cerr << "Failed to load shader '" << filename << "' from file!\n";
	}

	return source;
}

GLuint ShaderUtil::CreateShader(const char* filename, const std::string& shaderStr, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);

	// Check for an error during the shader object creation
	if(shaderID == 0 || shaderID == GL_INVALID_ENUM)
	{
		std::cerr << "Failed to create a shader object for '"
			<< filename << "'\n";
		return 0;
	}

	// Convert to a C-string in order for OpenGL to use
	const char* shaderSource = shaderStr.c_str();

	// Set the source for the shader object
	glShaderSource(shaderID, 1, &shaderSource, nullptr);

	glCompileShader(shaderID);

	GLint compileStatus;

	// Retrieve the compile status of the shader object
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
	if(compileStatus != GL_TRUE)
	{
		std::cerr << "Shader '" << filename << "' compilation failed\n";
		PrintLog(shaderID, false);
	}

	return shaderID;
}

GLuint ShaderUtil::CreateProgram(GLuint vertShaderID, GLuint fragShaderID)
{
	GLuint programID = glCreateProgram();

	if(programID == 0)
	{
		std::cerr << "Error occurred during program object creation!\n";
		return 0;
	}

	// Attach the shaders to the program
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);

	glLinkProgram(programID);

	// Check whether the program linked successfully
	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	if(linkStatus != GL_TRUE)
	{
		std::cerr << "Program failed to link\n";
		PrintLog(programID, true);

		return 0;
	}

	return programID;
}

void ShaderUtil::PrintLog(GLuint objectID, bool isProgram)
{
	// Stores the length of the info log
	GLint infoLogLength;

	// Retrieve the log length
	if(isProgram)
	{
		glGetProgramiv(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
	}
	else
	{
		glGetShaderiv(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
	}
	
	// Allocate a buffer to fit the log
	GLchar* logBuffer = new GLchar[infoLogLength];

	// Retrieve the info log
	if(isProgram)
	{
		glGetProgramInfoLog(objectID, infoLogLength, nullptr, logBuffer);
	}
	else
	{
		glGetShaderInfoLog(objectID, infoLogLength, nullptr, logBuffer);
	}
	
	std::cout << logBuffer << '\n';

	delete[] logBuffer;
}