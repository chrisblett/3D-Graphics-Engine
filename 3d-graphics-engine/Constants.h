#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

namespace Constants
{
	inline constexpr int DEFAULT_WINDOW_WIDTH = 1280;
	inline constexpr int DEFAULT_WINDOW_HEIGHT = 720;

	inline constexpr float DEFAULT_MOUSE_SENSITIVITY = 0.1f;
	
	// The field of view in degrees
	inline constexpr float FIELD_OF_VIEW = 60.0f;
		   
	inline constexpr float NEAR_PLANE = 0.01f;
	inline constexpr float FAR_PLANE = 100.0f;

	// The maximum number of light sources supported
	inline constexpr int MAX_LIGHTS = 4;

	// The number of MSAA samples
	inline constexpr int MSAA_SAMPLES = 4;

	// The number of anisotropic samples per pixel
	inline constexpr float ANIOSTROPY = 8.0f;

	const std::string RESOURCE_PATH = "../res/";
	const std::string MODEL_PATH    = RESOURCE_PATH + "models/";
	const std::string TEXTURE_PATH  = RESOURCE_PATH + "textures/";
	const std::string SHADER_PATH   = "shaders/";

	const std::string DEFAULT_SHADER_NAME_TEXTURE     = "phong";
	const std::string DEFAULT_SHADER_NAME_TEXTURELESS = "phong-notexture";
}

#endif