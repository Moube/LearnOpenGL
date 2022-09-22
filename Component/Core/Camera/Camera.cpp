#include "Camera.h"
#include "Util/Math.h"
namespace Core
{
	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
		Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch),
		Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		NeedUpdateVectors = false;
		MovementDirection = glm::vec3(0.0f);
		UpdateCameraVectors();
	}

	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
		Position(glm::vec3(posX, posY, posZ)), WorldUp(glm::vec3(upX, upY, upZ)), Yaw(yaw), Pitch(pitch),
		Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		NeedUpdateVectors = true;
		MovementDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		UpdateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void Camera::Tick(float delta)
	{
		if (NeedUpdateVectors)
			UpdateCameraVectors();
		UpdatePosition(delta);
	}

	void Camera::AddMovement(Movement direction, float value)
	{
		switch (direction)
		{
		case FORWARD:
			MovementDirection.x += value;
			break;
		case RIGHT:
			MovementDirection.z += value;
		default:
			break;
		}
	}

	void Camera::AddYaw(float value)
	{
		Yaw += value * MouseSensitivity;
		NeedUpdateVectors = true;
	}

	void Camera::AddPitch(float value)
	{
		Pitch += value * MouseSensitivity;
		Yaw = Math::Clamp(Yaw, -89.0f, 89.0f);
		NeedUpdateVectors = true;
	}

	void Camera::AddZoom(float value)
	{
		Zoom += value;
		Zoom = Math::Clamp(Zoom, 1.0f, 45.0f);
	}

	void Camera::UpdateCameraVectors()
	{
		glm::vec3 front;
		front.y = sin(glm::radians(Pitch));
		front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
		front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
		Front = glm::normalize(front);
		//
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
		NeedUpdateVectors = false;
	}

	void Camera::UpdatePosition(float delta)
	{
		if (glm::length(MovementDirection) <= 0.0f)
			return;
			
		MovementDirection = glm::normalize(MovementDirection);
		float velocity = MovementSpeed * delta;
		glm::vec3 moveVec = MovementDirection * velocity;
		glm::mat3 moveMat(Front, Up, Right);
		moveVec = moveMat * moveVec;
		Position += moveVec;

		MovementDirection = glm::vec3(0.0f);
	}

}