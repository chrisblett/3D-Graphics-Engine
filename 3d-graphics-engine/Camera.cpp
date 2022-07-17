#include "Camera.h"
#include "Constants.h"
#include "Window.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include <algorithm>

Camera::Camera(int windowWidth, int windowHeight, const glm::vec3& position)
	: m_position(position)
	, m_vel(0.0f, 0.0f, 0.0f)
	, m_viewDirection{DEFAULT_VIEW_DIRECTION}
	, m_up{WORLD_UP}
	, m_right{glm::vec3()}
	, m_projectionMatrix{glm::mat4()}
	, m_viewMatrix{glm::mat4()}
	, m_mouseSensitivity(Constants::DEFAULT_MOUSE_SENSITIVITY)
{
	std::cout << "Creating camera\n";

	// Set pitch and yaw values according to the view direction
	m_pitch = 0.0f;

	// Since the camera faces the negative z axis by default in OpenGL, 
	// the camera must be rotated to face this direction
	m_yaw = -90.0f;

	CreateProjectionMatrix(windowWidth, windowHeight);

	// Force the camera to update when constructed so that the necessary vectors can be
	// calculated
	UpdateOrientation(0.0f, 0.0f);

	std::cout << "Camera is facing: (" << m_viewDirection.x << ", " << m_viewDirection.y << ", "
		<< m_viewDirection.z << ")\n";
}

void Camera::CalculateCameraVectors()
{
	m_viewDirection.x = cosf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));
	m_viewDirection.y = sinf(glm::radians(m_pitch));
	m_viewDirection.z = sinf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch));

	// Construct the right vector using the camera view vector in order 
	// to move relative to the view direction
	m_right = glm::normalize(glm::cross(m_viewDirection, WORLD_UP));
	m_up    = glm::normalize(glm::cross(m_right, m_viewDirection));
}

void Camera::CreateProjectionMatrix(int width, int height)
{
	// If the window has been minimized, the width and height will be 0.
	// Set height to 1 to avoid division by zero error.
	if(height == 0) height = 1;
	float aspectRatio = static_cast<float>(width) / height;

	m_projectionMatrix = glm::perspective(glm::radians(Constants::FIELD_OF_VIEW),
		aspectRatio, Constants::NEAR_PLANE, Constants::FAR_PLANE);
}

void Camera::UpdateOrientation(float newX, float newY)
{
	// Negate the incoming y value since the screen y and world y coordinate axes are flipped
	m_pitch += (-newY * m_mouseSensitivity);
	m_yaw += (newX * m_mouseSensitivity);

	// If the range exceeds the limit, the view vector and the world up will be colinear, meaning
	// the cross product calculations used later would produce the zero vector.
	m_pitch = std::clamp(m_pitch, -PITCH_LIMIT, PITCH_LIMIT);

	CalculateCameraVectors();
}

void Camera::CalculateVelocity(MoveDirection moveDir, float dt)
{
	switch(moveDir)
	{
		case FORWARD:
			m_vel += m_viewDirection;
			break;

		case BACKWARD:
			m_vel -= m_viewDirection;
			break;

		case LEFT:
			m_vel += -m_right;
			break;

		case RIGHT:
			m_vel += m_right;
			break;

		case UP:
			m_vel += m_up;
			break;

		case DOWN:
			m_vel += -m_up;
			break;

		default:
			break;
	}
}

void Camera::UpdatePosition(float dt)
{
	if(glm::length(m_vel) > 0.0f)
	{
		m_vel = glm::normalize(m_vel);
		//std::cout << glm::length(m_vel) << std::endl;

		m_position += m_vel * dt;
	}

	m_vel = { 0, 0, 0 };
}

const glm::mat4& Camera::GetViewMatrix() const
{
	// Recalculate and return a reference to the view matrix
	m_viewMatrix = glm::lookAt(m_position, m_position + m_viewDirection, m_up);
	return m_viewMatrix;
}
