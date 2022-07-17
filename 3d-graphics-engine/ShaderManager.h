#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "ShaderProgram.h"

#include <map>
#include <string>

class ShaderManager
{
	std::map<std::string, ShaderProgram> m_shaders;

public:
	~ShaderManager();

	void RegisterShader(const std::string& vertPath, const std::string& fragPath, const char* name);
	ShaderProgram* GetShaderByName(const std::string& name);
};

#endif
