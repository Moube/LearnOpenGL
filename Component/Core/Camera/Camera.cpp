#include "Camera.h"
#include "Util/Math.h"
namespace Core
{
	Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
		SceneObject(position), WorldUp(up), Yaw(yaw), Pitch(pitch),
		Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		NeedUpdateVectors = false;
		MoveDirection = glm::vec3(0.0f);
		UpdateCameraVectors();
	}

	Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
		SceneObject(posX, posY, posZ), WorldUp(glm::vec3(upX, upY, upZ)), Yaw(yaw), Pitch(pitch),
		Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		NeedUpdateVectors = true;
		MoveDirection = glm::vec3(0.0f, 0.0f, 0.0f);
		UpdateCameraVectors();
	}

	glm::mat4 Camera::GetViewMatrix()
	{
		glm::mat4 lookAt = glm::lookAt(GetPosition(), GetPosition() + Front, Up);
		return lookAt;
	}

	glm::mat4 Camera::GetViewMatrixPractice()
	{
		//Chapter Camera��ϰ2.���Ŵ������Լ���LookAt��������������Ҫ�ֶ�����һ��������һ��ʼ���۵Ĺ۲��������ĺ���ʵ�����滻GLM��LookAt�������������Ƿ���һ���ع���
		glm::mat4 viewMatrix(1.0f);

		//˼·1:ʹ��pitch, yaw�ķ�����ת          
		// ʧ��                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
		//viewMatrix = glm::rotate(viewMatrix, -glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		//viewMatrix = glm::rotate(viewMatrix, -glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));

		//˼·2:�����(1,0,0)��ת��Front����ת����
		// ʧ��
		//glm::vec3 xAxis(1.0f, 0.0f, 0.0f);
		//float angle = acos(glm::dot(xAxis, Front));
		//glm::vec3 rotateAxis = glm::cross(Front, xAxis);
		//viewMatrix = glm::rotate(viewMatrix, -angle, rotateAxis);

		//---------------���������Ķ�������, ���Ե�....------------------

		//˼·3.����ʹ��Right, Up, Direction������ת����, ��Ҫ����ת��
		// �ɹ�!
		glm::mat4 ratote(
			glm::vec4(Right, 0.0f),
			glm::vec4(Up, 0.0f),
			glm::vec4(-Front, 0.0f),//��Ҫ������
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		glm::mat4 ratoteTranspose = glm::transpose(ratote);
		viewMatrix = viewMatrix * ratoteTranspose;
		viewMatrix = glm::translate(viewMatrix, -GetPosition());

		return viewMatrix;

		//˼·4. �ο���
		//��Camera::calculate_lookAt_matrix
	}

	glm::mat4 Camera::calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
	{
		// 1. Position = known
	// 2. Calculate cameraDirection
		glm::vec3 zaxis = glm::normalize(position - target);
		// 3. Get positive right axis vector
		glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
		// 4. Calculate camera up vector
		glm::vec3 yaxis = glm::cross(zaxis, xaxis);

		// Create translation and rotation matrix
		// In glm we access elements as mat[col][row] due to column-major layout
		glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
		translation[3][0] = -position.x; // Third column, first row
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0][0] = xaxis.x; // First column, first row
		rotation[1][0] = xaxis.y;
		rotation[2][0] = xaxis.z;
		rotation[0][1] = yaxis.x; // First column, second row
		rotation[1][1] = yaxis.y;
		rotation[2][1] = yaxis.z;
		rotation[0][2] = zaxis.x; // First column, third row
		rotation[1][2] = zaxis.y;
		rotation[2][2] = zaxis.z;

		// Return lookAt matrix as combination of translation and rotation matrix
		return rotation * translation; // Remember to read from right to left (first translation then rotation)
	}

	void Camera::Tick(float delta)
	{
		if (NeedUpdateVectors)
			UpdateCameraVectors();
		UpdatePosition(delta);
		//UpdatePositionLimitInXZ(delta);//��ϰ1.
	}

	void Camera::AddMovement(Movement direction, float value)
	{
		switch (direction)
		{
		case FORWARD:
			MoveDirection.x += value;
			break;
		case RIGHT:
			MoveDirection.z += value;
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
		Pitch = Math::Clamp(Pitch, -89.0f, 89.0f);
		NeedUpdateVectors = true;
	}

	void Camera::AddZoom(float value)
	{
		Zoom += value;
		Zoom = Math::Clamp(Zoom, 1.0f, 45.0f);
	}

	void Camera::SetMovespeed(float speed)
	{
		if (speed < 0.0f || speed == MovementSpeed)
			return;

		MovementSpeed = speed;
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
		if (glm::length(MoveDirection) <= 0.0f)
			return;
		
		//�ƶ�����
		MoveDirection = glm::normalize(MoveDirection);
		glm::mat3 moveMat(Front, Up, Right);
		glm::vec3 moveVec = moveMat * MoveDirection;

		//�ƶ�����
		float velocity = MovementSpeed * delta;
		moveVec = moveVec * velocity;

		SetPosition(GetPosition() + moveVec);

		MoveDirection = glm::vec3(0.0f);
	}

	void Camera::UpdatePositionLimitInXZ(float delta)
	{
		if (glm::length(MoveDirection) <= 0.0f)
			return;

		MoveDirection = glm::normalize(MoveDirection);
		glm::mat3 moveMat(Front, Up, Right);
		glm::vec3 moveVec = moveMat * MoveDirection;

		//����y���ƶ���
		moveVec.y = 0.0f;
		float velocity = MovementSpeed * delta;
		moveVec = glm::normalize(moveVec) * velocity;

		SetPosition(GetPosition() + moveVec);

		MoveDirection = glm::vec3(0.0f);
	}

}