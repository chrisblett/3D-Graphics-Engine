#include "pch.h"

#include "../3d-graphics-engine/Light.h"
#include "../3d-graphics-engine/PointLight.h"
#include "../3d-graphics-engine/DirectionalLight.h"
#include <glm\glm.hpp>

/*
* Light Testing
*/

TEST(Light, CheckValidColour)
{
	glm::vec3 redColour = glm::vec3(0.0f, 1.0f, 0.0f);

	bool result = Light::IsValidColour(redColour);

	EXPECT_TRUE(result);
}

TEST(Light, CheckValidColourBoundaryMin)
{
	glm::vec3 minColour = glm::vec3(0.0f, 0.0f, 0.0f);

	bool result = Light::IsValidColour(minColour);

	EXPECT_TRUE(result);
}

TEST(Light, CheckValidColourBoundaryMax)
{
	glm::vec3 maxColour = glm::vec3(1.0f, 1.0f, 1.0f);

	bool result = Light::IsValidColour(maxColour);

	EXPECT_TRUE(result);
}

TEST(Light, CheckInvalidColour)
{
	glm::vec3 invalidColour = glm::vec3(-1.0f, 0.0f, 0.2f);

	bool result = Light::IsValidColour(invalidColour);

	EXPECT_FALSE(result);
}

TEST(Light, DefaultCtorColour)
{
	Light* pLight = new PointLight;

	glm::vec3 colour = pLight->GetColour();

	EXPECT_FLOAT_EQ(colour.r, 0.0f);
	EXPECT_FLOAT_EQ(colour.g, 0.0f);
	EXPECT_FLOAT_EQ(colour.b, 0.0f);

	delete pLight;
}

TEST(Light, DefaultCtorIntensity)
{
	Light* pLight = new PointLight;

	float intensity = pLight->GetIntensity();

	EXPECT_FLOAT_EQ(intensity, 0.0f);

	delete pLight;
}

TEST(Light, ExplicitCtorColour)
{
	// Arrange
	glm::vec3 lightColour = glm::vec3(1.0f, 0.0f, 0.0f);

	// Act
	Light* pLight = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f),
		lightColour, 1.0f);

	// Assert
	glm::vec3 retColour = pLight->GetColour();

	EXPECT_FLOAT_EQ(retColour.r, lightColour.r);
	EXPECT_FLOAT_EQ(retColour.g, lightColour.g);
	EXPECT_FLOAT_EQ(retColour.b, lightColour.b);

	delete pLight;
}

TEST(Light, ExplicitCtorIntensity)
{
	// Arrange & Act
	Light* pLight = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);

	// Assert
	float returnedIntensity = pLight->GetIntensity();

	EXPECT_FLOAT_EQ(returnedIntensity, 0.3f);

	delete pLight;
}

TEST(Light, SetColour)
{
	// Arrange
	Light* pLight = new PointLight;

	// Act
	pLight->SetColour(glm::vec3(0.2f, 0.9f, 0.0f));

	// Assert
	glm::vec3 retColour = pLight->GetColour();

	EXPECT_FLOAT_EQ(retColour.x, 0.2f);
	EXPECT_FLOAT_EQ(retColour.y, 0.9f);
	EXPECT_FLOAT_EQ(retColour.z, 0.0f);

	delete pLight;
}

/*
* PointLight Testing
*/

TEST(PointLight, ExplicitCtorPosition)
{
	// Arrange & Act
	PointLight pointLight(glm::vec3(4.0f, -0.3f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

	// Assert
	glm::vec3 pos = pointLight.GetPosition();

	EXPECT_FLOAT_EQ(pos.x,  4.0f);
	EXPECT_FLOAT_EQ(pos.y, -0.3f);
	EXPECT_FLOAT_EQ(pos.z,  0.0f);
}

TEST(PointLight, SetPosition)
{
	// Arrange
	PointLight pointLight;

	// Act
	pointLight.SetPosition(glm::vec3(2.0f, 3.0f, -1.0f));

	// Assert
	glm::vec3 pos = pointLight.GetPosition();

	EXPECT_FLOAT_EQ(pos.x, 2.0f);
	EXPECT_FLOAT_EQ(pos.y, 3.0f);
	EXPECT_FLOAT_EQ(pos.z, -1.0f);
}

TEST(PointLight, SetPositionNegative)
{
	// Arrange
	PointLight pointLight;

	// Act
	pointLight.SetPosition(glm::vec3(-1.1f, -4.2f, -8.5f));

	// Assert
	glm::vec3 pos = pointLight.GetPosition();

	EXPECT_FLOAT_EQ(pos.x, -1.1f);
	EXPECT_FLOAT_EQ(pos.y, -4.2f);
	EXPECT_FLOAT_EQ(pos.z, -8.5f);
}

/*
* DirectionalLight Testing
*/

TEST(DirectionalLight, CtorDirection)
{
	DirectionalLight dirLight(glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

	glm::vec3 dir = dirLight.GetDirection();

	EXPECT_FLOAT_EQ(dir.x,  0.0f);
	EXPECT_FLOAT_EQ(dir.y, -1.0f);
	EXPECT_FLOAT_EQ(dir.z,  0.0f);
}

TEST(DirectionalLight, CheckDirectionIsNormalised)
{
	// Arrange
	glm::vec3 inputDir = glm::vec3(1.0f, -1.0f, 0.0f);
	DirectionalLight dirLight(inputDir, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	glm::vec3 inputDirNormalised = glm::normalize(inputDir);

	// Act
	glm::vec3 dirResult = dirLight.GetDirection();

	// Assert
	EXPECT_FLOAT_EQ(dirResult.x, inputDirNormalised.x);
	EXPECT_FLOAT_EQ(dirResult.y, inputDirNormalised.y);
	EXPECT_FLOAT_EQ(dirResult.z, inputDirNormalised.z);
}