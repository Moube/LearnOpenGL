#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include "Singleton/Singleton.h"
//https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

namespace Core
{
    // Default camera values
    const float YAW = -90.0F;
    const float PITCH = 0.0f;
    const float SPEED = 2.5F;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;

    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum Movement
    {
        FORWARD,
        RIGHT,
    };

    //֮����Է�װһ��SceneNode�ĸ���
    //�ٰѿռ���Ϣ �ƶ���װ�������
    class Camera
    {
    public:
        // constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        // constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    public:
        //��ù۲����
        glm::mat4 GetViewMatrix();
        glm::mat4 GetViewMatrixPractice();
        static glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);

        //�ƶ��Ĵ��벻������ �Լ�д
        void Tick(float delta);

        //�ƶ�
        void AddMovement(Movement direction, float value);
        glm::vec3 GetPosition() { return Position; }

        //��ת
        void AddYaw(float value);
        void AddPitch(float value);

        //fov
        void AddZoom(float value);
        float GetZoom() { return Zoom; }

	private:
		// camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		//euler Angles
		float Yaw;
		float Pitch;

		//camera options
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

    private:
        void UpdateCameraVectors();
        void UpdatePosition(float delta);
        bool NeedUpdateVectors;
        glm::vec3 MoveDirection;

    private:
        //Chapter Camera��ϰ1.�������Ƿ��ܹ��޸�������࣬ʹ�����ܹ����һ��������FPS�������Ҳ����˵���ܹ�������У�����ֻ�ܹ�����xzƽ����
        void UpdatePositionLimitInXZ(float delta);

    private:
        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera(Camera&& other) noexcept = default;
        Camera& operator=(Camera&& other) noexcept = default;
    };
}
