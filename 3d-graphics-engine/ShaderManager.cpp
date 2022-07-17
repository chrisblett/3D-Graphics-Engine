#include "ShaderManager.h"
#include "ShaderProgram.h"
#include <cassert>
#include <iostream>
#include <string>

ShaderManager::~ShaderManager()
{
	std::cout << "ShaderManager destructor called\n";

	// Delete all shader programs
	auto it = m_shaders.begin();
	while(it != m_shaders.end())
	{
		GLuint id = it->second.m_programID;
		glDeleteProgram(id);

		std::cout << "Deleting shader program [" << id << "]\n";

		++it;
	}
}

void ShaderManager::RegisterShader(const std::string& vertPath, const std::string& fragPath, const char* name)
{
	m_shaders.emplace(name, ShaderProgram(vertPath.c_str(), fragPath.c_str()));
}

ShaderProgram* ShaderManager::GetShaderByName(const std::string& name)
{
	ShaderProgram* pShaderProgram = nullptr;

	auto it = m_shaders.find(name);

	if(it != m_shaders.end())
	{
		pShaderProgram = &(it->second);
	}
	else
	{
		assert(false && "Could not find shader");
	}

	return pShaderProgram;
}