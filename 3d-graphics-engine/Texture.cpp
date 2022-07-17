#include "Texture.h"
#include "Constants.h"
#include <SOIL2\SOIL2.h>
#include <algorithm>
#include <iostream>
#include <string>

GLuint LoadTexture(const std::string& filepath, GLfloat filter)
{
	GLuint textureID = SOIL_load_OGL_texture(filepath.c_str(), SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

	// Check for an error while loading
	if(textureID == 0)
	{
		std::cerr << "Failed to load texture '" << filepath << "' from file\n";
	}

	// Bind texture to set parameters
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set the texture filtering
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	// Checking for mipmap support
	if(filter == GL_NEAREST_MIPMAP_NEAREST ||
	    filter == GL_LINEAR_MIPMAP_NEAREST ||
	    filter == GL_NEAREST_MIPMAP_LINEAR ||
	    filter == GL_LINEAR_MIPMAP_LINEAR)
	{
		glGenerateMipmap(GL_TEXTURE_2D);

		// Get the maximum anisotropy samples the GPU supports
		GLfloat maxAnisotropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);

		//std::cout << "Maximum anisotropy supported is " << maxAnisotropy << " samples\n";

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY,
			std::clamp(maxAnisotropy, 0.0f, Constants::ANIOSTROPY));
	}
	
	return textureID;
}

/*
* Given a directory path, this function assumes all of the cubemap images are present,
* of the expected format (.jpg) and are named appropriately, e.g
* right	 - POSITIVE_X
* left	 - NEGATIVE_X
* top	 - POSITIVE_Y
* bottom - NEGATIVE_Y
* front	 - POSITIVE_Z
* back	 - NEGATIVE_Z
*/
GLuint LoadCubemap(const std::string& cubemapDir)
{
	// Create strings for each texture image for concatenation
	const std::string fullDirPath = Constants::TEXTURE_PATH + cubemapDir;
	std::string right  = fullDirPath + "/right.jpg";
	std::string left   = fullDirPath + "/left.jpg";
	std::string top    = fullDirPath + "/top.jpg";
	std::string bottom = fullDirPath + "/bottom.jpg";
	std::string front  = fullDirPath + "/front.jpg";
	std::string back   = fullDirPath + "/back.jpg";

	std::cout << "Attempting to load cubemap at '" << fullDirPath << "'\n";
	std::cout << "Outputting full image paths\n";
	std::cout << "'" << right << "'\n" <<
		"'" << left << "'\n" <<
		"'" << top << "'\n" <<
		"'" << bottom << "'\n" <<
		"'" << front << "'\n" <<
		"'" << back << "'\n";

	GLuint textureID = SOIL_load_OGL_cubemap(right.c_str(), left.c_str(),
		top.c_str(), bottom.c_str(),
		front.c_str(), back.c_str(),
		SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	// Check for error
	if(textureID == 0)
	{
		std::cerr << "Failed to load cubemap at directory '" << fullDirPath << "' from file!\n";
	}

	return textureID;
}

Texture::Texture()
{
	glGenTextures(1, &m_textureID);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetCompareMode(GLint compareMode)
{
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, compareMode);
	glBindTexture(GL_TEXTURE_2D, 0);
}