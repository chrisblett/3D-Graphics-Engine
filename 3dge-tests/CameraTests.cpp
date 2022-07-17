#include "pch.h"

#include "../3d-graphics-engine/Camera.h"
#include <glm\glm.hpp>

#define EPSILON 0.000001f

TEST(Camera, CtorDefaultPosition)
{
	// Arrange
	Camera cam(1, 1);
	
	// Act
	glm::vec3 pos = cam.GetPosition();

	// Assert
	EXPECT_FLOAT_EQ(pos.x, 0.0f);
	EXPECT_FLOAT_EQ(pos.y, 0.0f);
	EXPECT_FLOAT_EQ(pos.z, 0.0f);
}

TEST(Camera, CtorPosition)
{
	// Arrange
	Camera cam(1, 1, glm::vec3(2.0f, 0.5f, 1.0f));

	// Act
	glm::vec3 pos = cam.GetPosition();

	// Assert
	EXPECT_FLOAT_EQ(pos.x, 2.0f);
	EXPECT_FLOAT_EQ(pos.y, 0.5f);
	EXPECT_FLOAT_EQ(pos.z, 1.0f);
}

TEST(Camera, CtorForwardVector)
{
	// Arrange
	Camera cam(1, 1);

	// Act
	glm::vec3 forward = cam.GetForwardVector();

	// Assert
	EXPECT_NEAR(forward.x,  0.0f, EPSILON);
	EXPECT_NEAR(forward.y,  0.0f, EPSILON);
	EXPECT_NEAR(forward.z, -1.0f, EPSILON);
}


TEST(Camera, CtorRightVector)
{
	// Arrange
	Camera cam(1, 1);

	// Act
	glm::vec3 right = cam.GetRightVector();

	// Assert
	EXPECT_NEAR(right.x, 1.0f, EPSILON);
	EXPECT_NEAR(right.y, 0.0f, EPSILON);
	EXPECT_NEAR(right.z, 0.0f, EPSILON);
}

TEST(Camera, CtorUpVector)
{
	// Arrange
	Camera cam(1, 1);

	// Act
	glm::vec3 up = cam.GetUpVector();

	// Assert
	EXPECT_NEAR(up.x, 0.0f, EPSILON);
	EXPECT_NEAR(up.y, 1.0f, EPSILON);
	EXPECT_NEAR(up.z, 0.0f, EPSILON);
}

TEST(Camera, NewPositionWhenMovingForwardFor1Second)
{
	Camera cam(1, 1);

	// Advance for 1 second
	float deltaTime = 1.0f;
	cam.CalculateVelocity(FORWARD, 1.0f);
	cam.UpdatePosition(1.0f);

	// Assert
	glm::vec3 newPos = cam.GetPosition();

	EXPECT_NEAR(newPos.x,  0.0f, EPSILON);
	EXPECT_NEAR(newPos.y,  0.0f, EPSILON);
	EXPECT_NEAR(newPos.z, -1.0f, EPSILON);
}

TEST(Camera, NewPositionWhenMovingRightFor1Second)
{
	Camera cam(1, 1);

	// Advance for 1 second
	float deltaTime = 1.0f;
	cam.CalculateVelocity(RIGHT, deltaTime);
	cam.UpdatePosition(1.0f);

	// Assert
	glm::vec3 newPos = cam.GetPosition();

	EXPECT_NEAR(newPos.x, 1.0f, EPSILON);
	EXPECT_NEAR(newPos.y, 0.0f, EPSILON);
	EXPECT_NEAR(newPos.z, 0.0f, EPSILON);
}

TEST(Camera, NewPositionWhenMovingUpFor1Second)
{
	Camera cam(1, 1);

	// Advance for 1 second
	float deltaTime = 1.0f;
	cam.CalculateVelocity(UP, deltaTime);
	cam.UpdatePosition(1.0f); 

	// Assert
	glm::vec3 newPos = cam.GetPosition();

	EXPECT_NEAR(newPos.x, 0.0f, EPSILON);
	EXPECT_NEAR(newPos.y, 1.0f, EPSILON);
	EXPECT_NEAR(newPos.z, 0.0f, EPSILON);
}

TEST(Camera, AccumulateVelocityForwardAndRight)
{
	// Arrange
	Camera cam(1, 1);

	// Expected velocity
	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 expectedVel = forward + right;

	// Advance for 1 second
	float deltaTime = 1.0f;

	cam.CalculateVelocity(FORWARD, deltaTime);
	cam.CalculateVelocity(RIGHT, deltaTime);

	// Assert
	glm::vec3 accumulatedVel = cam.GetVelocity();

	EXPECT_NEAR(accumulatedVel.x, expectedVel.x, EPSILON);
	EXPECT_NEAR(accumulatedVel.y, expectedVel.y, EPSILON);
	EXPECT_NEAR(accumulatedVel.z, expectedVel.z, EPSILON);
}

TEST(Camera, AccumulateVelocityUpAndRight)
{
	// Arrange
	Camera cam(1, 1);

	// Expected velocity
	glm::vec3 up    = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 expectedVel = up + right;

	// Advance for 1 second
	float deltaTime = 1.0f;

	// Act
	cam.CalculateVelocity(UP, deltaTime);
	cam.CalculateVelocity(RIGHT, deltaTime);

	// Assert
	glm::vec3 accumulatedVel = cam.GetVelocity();

	EXPECT_NEAR(accumulatedVel.x, expectedVel.x, EPSILON);
	EXPECT_NEAR(accumulatedVel.y, expectedVel.y, EPSILON);
	EXPECT_NEAR(accumulatedVel.z, expectedVel.z, EPSILON);
}

TEST(Camera, VelocityIsResetAfterMovingForwardFor1SecondAndUpdatingPosition)
{
	// Arrange
	Camera cam(1, 1);
	float deltaTime = 1.0f;

	// Act
	cam.CalculateVelocity(FORWARD, deltaTime);
	cam.UpdatePosition(deltaTime);

	// Assert
	glm::vec3 vel = cam.GetVelocity();

	EXPECT_FLOAT_EQ(vel.x, 0.0f);
	EXPECT_FLOAT_EQ(vel.y, 0.0f);
	EXPECT_FLOAT_EQ(vel.z, 0.0f);
}