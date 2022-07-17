#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include <string>

class Texture;

class TextureManager
{
	std::map<std::string, Texture> m_textures;

public:
	void CreateTexture(const std::string& filename);
	Texture* GetTexture(const std::string& name);
};

#endif