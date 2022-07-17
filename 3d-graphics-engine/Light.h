#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

class ShaderProgram;

class ShadowData
{
	struct LightMatrix
	{
		glm::mat4 projMatrix;
		glm::mat4 viewMatrix;
		float nearPlane;
		float farPlane;
		bool isOrthographic;
	} m_lightMatrix;

	float m_shadowBias;

public:
	ShadowData(const glm::mat4& projMatrix, const glm::mat4& viewMatrix,
		float nearPlane, float farPlane, float shadowBias, bool isOrtho)
		: m_lightMatrix{ projMatrix, viewMatrix, nearPlane, farPlane, isOrtho }
		, m_shadowBias(shadowBias)
	{
	}

	// It is necessary to recalculate the new light matrix everytime it is needed
	// since changes may have been made to the light's view matrix.
	glm::mat4 GetLightMatrix() const 
	{ 
		return m_lightMatrix.projMatrix * m_lightMatrix.viewMatrix;
	}

	void UpdateLightViewMatrix(const glm::mat4& viewMatrix) { m_lightMatrix.viewMatrix = viewMatrix; }

	float GetNearPlane()  const { return m_lightMatrix.nearPlane; }
	float GetFarPlane()   const { return m_lightMatrix.farPlane; }
	float GetShadowBias() const { return m_shadowBias; }
	// Returns true if the light matrix projection is orthographic
	bool IsOrthographic() const { return m_lightMatrix.isOrthographic; }
};

class Light
{
	constexpr static float AMBIENT_FACTOR{ 0.1f };
protected:
	ShadowData* m_shadowData;

	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;

	float m_intensity;

	int m_id;
public:
	// TODO: Set default initializers
	Light()
		: m_shadowData(nullptr)
		, m_id(0)
	{
		SetColour({ 0.0f, 0.0f, 0.0f });
		SetIntensity(0.0f);
	}

	Light(const glm::vec3& color, float intensity = 1.0f)
		: m_shadowData(nullptr)
		, m_id(0)
	{
		SetColour(color);
		SetIntensity(intensity);
	}

	virtual ~Light()
	{
		if(m_shadowData)
		{
			delete m_shadowData;
		}
	}

	// Set a shaders uniform values with the light attributes.
	// 'shadowIndex' is the index of the light's shadow map, in the 
	// shadow map array. Currently the index of the light and its 
	// shadow map are the same.
	virtual void SetUniforms(ShaderProgram* pShader, int shadowIndex) = 0;

	static bool IsValidColour(const glm::vec3& colour)
	{
		return (colour.r >= 0.0f && colour.r <= 1.0f) &&
			   (colour.g >= 0.0f && colour.g <= 1.0f) &&
			   (colour.b >= 0.0f && colour.b <= 1.0f);
	}

	void SetColour(const glm::vec3& colour)
	{
		// Colour cannot be negative, check each component
		if (!IsValidColour(colour))
		{			
			assert(false && "Colour values cannot be negative!");
		}
		
		m_ambient  = AMBIENT_FACTOR * colour;
		m_diffuse  = colour;
		m_specular = colour;
	}

	void SetIntensity(float intensity)
	{
		assert(intensity >= 0.0f && intensity <= 1.0f &&
			"Light intensity must be between 0 and 1!");

		m_intensity = std::clamp(intensity, 0.0f, 1.0f);
	}

	// Accessors
	ShadowData* GetShadowData() const { return m_shadowData; }

	float GetIntensity() const { return m_intensity; }

	// Returns the colour of the light without accounting for intensity
	// (Basically, the colour provided in the ctor)
	glm::vec3 GetColour() const { return m_diffuse; }

	glm::vec3 GetAmbient()  const { return m_ambient  * m_intensity; }
	glm::vec3 GetDiffuse()  const { return m_diffuse  * m_intensity; }
	glm::vec3 GetSpecular() const { return m_specular * m_intensity; }
};

#endif
