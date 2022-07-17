#include "TextureManager.h"
#include "Texture.h"
#include "Constants.h"
#include <cassert>
#include <iostream>
#include <string>

void TextureManager::CreateTexture(const std::string& filename)
{
	printf("Creating texture '%s'\n", filename.c_str());
	auto it = m_textures.find(filename);
	if(it != m_textures.end())
	{
		std::cerr << "A texture already exists with the name '" << filename << "'\n";
		assert(false);
	}
	else
	{
		// If a texture with the specified name is not in the map,
		// create one and add it to the map
		m_textures.insert({ filename, Texture(LoadTexture(Constants::TEXTURE_PATH + filename)) });
	}
}

Texture* TextureManager::GetTexture(const std::string& name)
{
	Texture* pTexture = nullptr;

	auto it = m_textures.find(name);
	if(it != m_textures.end())
	{
		pTexture = &(*it).second;
	}
	else
	{
		std::cerr << "Failed to retrieve texture with name '" << name << "'\n";
	}

	return pTexture;
}