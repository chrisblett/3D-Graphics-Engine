#ifndef CAMERA_H
#define CAMERA_H

#include "Constants.h"
#include <glm\glm.hpp>

enum MoveDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class Camera
{
	static constexpr glm::vec3 WORLD_UP{ 0.0f, 1.0f, 0.0f };
	static constexpr glm::vec3 DEFAULT_VIEW_DIRECTION{ 0.0f, 0.0f, -1.0f };
	static constexpr glm::vec3 DEFAULT_CAMERA_POSITION{ 0.0f, 0.0f, 0.0f };
	static constexpr float PITCH_LIMIT{ 89.0f };

	mutable glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	glm::vec3 m_position;
	glm::vec3 m_vel;

	glm::vec3 m_viewDirection;
	glm::vec3 m_right;
	glm::vec3 m_up;

	float m_mouseSensitivity;
	float m_pitch;
	float m_yaw;

	void CalculateCameraVectors();
	
public:
	Camera(int windowWidth, int windowHeight, const glm::vec3& position = DEFAULT_CAMERA_POSITION);

	void UpdateOrientation(float newX, float newY);
	void CalculateVelocity(MoveDirection moveDir, float dt);
	void UpdatePosition(float dt);

	void CreateProjectionMatrix(int width, int height);

	const glm::mat4& GetViewMatrix() const;
	const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }

	const glm::vec3& GetPosition() const { return m_position; }
	const glm::vec3& GetVelocity() const { return m_vel;      }

	const glm::vec3& GetForwardVector() const { return m_viewDirection; }
	const glm::vec3& GetRightVector()   const { return m_right;         }
	const glm::vec3& GetUpVector()      const { return m_up;            }
}; 

#endif