#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <cmath>
#include "Window/Window.h"
#include "Shader/ShaderProgram.h"
#include "Util/Log.h"
#include "Texture/Texture2D.h"
#include "Input/InputManager.h"
#include "Camera/Camera.h"

GLFWwindow* window = nullptr;
Core::Camera* camera = nullptr;
Core::InputManager* _InputManager = nullptr;
unsigned int VAO, VBO, EBO;
unsigned int texture1, texture2;
void InitVertex3D();
void InitInput();
void Draw();

//glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
//glm::vec3 cameraFront(0.0f, 0.0f, -1.f);
//glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

//glm::vec3 direction(0.0f);
//float row = 0.0f;
//float pitch = 0.0f;
//float yaw = -90.0f;
//void ProcessInput(float delta);

float lastX = 400, lastY = 300;//鼠标初始位置
bool firstMouse = true;
void mouse_callback(GLFWwindow* _window, double xpos, double ypos);

//float fov = 45.0f;
void scroll_callback(GLFWwindow* _window, double xoffset, double yoffset);

int main()
{
	using namespace Core;
	Window* WindowInstance = Window::Instance();
	_InputManager = InputManager::Instance();
	WindowInstance->Init();
	window = WindowInstance->GetWindow();
	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	InitInput();
	InitVertex3D();
	ShaderProgram program("../../Camera/ShaderSource/shader.vert", "../../Camera/ShaderSource/shader.frag");

	Texture2D::flipVertically();
	Texture2D texture1("container.jpg");
	Texture2D texture2("awesomeface.png");

	program.use();
	program.setInt("texture1", 0);
	program.setInt("texture2", 1);

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		WindowInstance->PreProcess(deltaTime);
		//ProcessInput(deltaTime);
		_InputManager->ProcessInput();
		camera->Tick(deltaTime);

		program.use();
		texture1.activate(0);
		texture2.activate(1);

		//direction.y = sin(glm::radians(pitch));
		//direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		//direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		// view matrix from look at 
		//direction
		//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);//方向向量(Direction Vector)并不是最好的名字，因为它实际上指向从它到目标向量的相反方向
		//right
		//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		//up
		//glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

		//look at
		//glm::mat4 view(1.0f);
		//view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 view = camera->GetViewMatrixPractice();//练习2.

		//perspective projection matrix
		float screenWidth = (float)WindowInstance->GetWidth();
		float screenHeight = (float)WindowInstance->GetHeight();
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(camera->GetZoom()), screenWidth / screenHeight, 0.1f, 100.0f);

		//vertex shader uniform
		program.setMat4("view", view);
		program.setMat4("projection", projection);

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		//相机绕0,0,0做在xz平面上做圆周运动
		//float radius = 10.0f;
		//float camX = sin(time) * radius;
		//float camZ = cos(time) * radius;
		//cameraPos = glm::vec3(camX, 0.0f, camZ);

		for (unsigned int i = 0; i < 10; i++)
		{
			//model matrix
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			program.setMat4("model", model);
			Draw();
		}

		WindowInstance->SwapBuffer(deltaTime);
	}

	glfwTerminate();

	return 0;
}

void InitVertex3D()
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//没有使用EBO

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//位置属性
	//			  位置属性0, 3个float				步长,				偏移
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//纹理属性
	//			  位置属性1, 2个float				步长,				偏移
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void InitInput()
{
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	_InputManager->BindInput(GLFW_KEY_W, [&] { camera->AddMovement(Core::Movement::FORWARD, 1); });
	_InputManager->BindInput(GLFW_KEY_S, [&] { camera->AddMovement(Core::Movement::FORWARD, -1); });
	_InputManager->BindInput(GLFW_KEY_D, [&] { camera->AddMovement(Core::Movement::RIGHT, 1); });
	_InputManager->BindInput(GLFW_KEY_A, [&] { camera->AddMovement(Core::Movement::RIGHT, -1); });
	_InputManager->BindInput(GLFW_KEY_ESCAPE, [&] { glfwSetWindowShouldClose(window, true); });
}
/*
void ProcessInput(float delta)
{
	float cameraSpeed = 2.5f * delta;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
*/


void mouse_callback(GLFWwindow* _window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffest = (float)(xpos - lastX);
	//glfwSetCursorPosCallback返回给mouse_callback函数的 (x,y) 是鼠标相对于窗口左上角的位置，所以需要将 (ypos - lastY) 取反
	float yoffset = (float)(lastY - ypos);
	lastX = (float)xpos;
	lastY = (float)ypos;

	camera->AddYaw(xoffest);
	camera->AddPitch(yoffset);

	/*
	float sensitivity = 0.05f;
	xoffest *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffest;
	pitch += yoffset;
	pitch = pitch > 89.0f ? 89.0f : pitch;
	pitch = pitch < -89.0f ? -89.0f : pitch;

	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(cameraFront);
	*/
}


void scroll_callback(GLFWwindow* _window, double xoffset, double yoffset)
{
	camera->AddZoom((float)-yoffset);
	/*
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= (float)yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
	*/
}
