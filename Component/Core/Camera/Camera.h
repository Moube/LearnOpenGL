#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Object/SceneObject.h"
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

    //旋转理应好好封装一下, 暂不处理
    class Camera : public SceneObject
    {
    public:
        // constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        // constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    public:
        //获得观察矩阵
        glm::mat4 GetViewMatrix();
        glm::mat4 GetViewMatrixPractice();
        static glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);

        //移动的代码不用它的 自己写
        void Tick(float delta);

        //移动
        void AddMovement(Movement direction, float value);

        //旋转
        void AddYaw(float value);
        void AddPitch(float value);

        //fov
        void AddZoom(float value);
        float GetZoom() { return Zoom; }

        glm::vec3 GetFront() { return Front; }

        //speed
        void SetMovespeed(float speed);

	private:
		// camera Attributes
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
        //Chapter Camera练习1.看看你是否能够修改摄像机类，使得其能够变成一个真正的FPS摄像机（也就是说不能够随意飞行）；你只能够呆在xz平面上
        void UpdatePositionLimitInXZ(float delta);

    private:
        Camera(const Camera&) = delete;
        Camera& operator=(const Camera&) = delete;
        Camera(Camera&& other) noexcept = default;
        Camera& operator=(Camera&& other) noexcept = default;
    };
}

