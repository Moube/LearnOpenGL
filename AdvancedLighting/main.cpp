#pragma once

#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Util/Math.h"
#include "Asset/Asset.h"
#include "Light/PointLight.h"
#include "Window/Window.h"
#include "Camera/Camera.h"
#include "Material/Material.h"
#include "Texture/Texture2D.h"
#include "Input/InputManager.h"
#include "Shader/ShaderProgram.h"


GLFWwindow* window = nullptr;
Core::Camera* camera = nullptr;
Core::InputManager* Input = nullptr;
std::string projectName = "AdvancedLighting";
std::string shaderPath(std::string path);
float lastFrame = 0.0f;
float deltaTime = 0.0f;
float totalTime = 0.0f;

void InitInput();
float lastX = 400, lastY = 300;//鼠标初始位置
bool firstMouse = true;
void mouse_callback(GLFWwindow* _window, double xpos, double ypos);
void scroll_callback(GLFWwindow* _window, double xoffset, double yoffset);

unsigned int planeVAO, planeVBO;
void InitVertex();
void DrawPlane();

int main()
{
	Core::Window* Window = Core::Window::Instance();
	Input = Core::InputManager::Instance();
	Window->Init();
	window = Window->GetWindow();
	camera = new Core::Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	InitInput();
	InitVertex();

	Core::ShaderProgram program(shaderPath("shader.vert"), shaderPath("shader.frag"));

	Core::Material material(
		std::string("wood.png"),
		std::string("wood.png"),//不使用spec贴图
		32.0f);

	Core::PointLight light(
		glm::vec3(1.2f, 1.0f, 2.0f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.3f)
	);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Window->PreProcess(deltaTime);
		Input->ProcessInput();
		camera->Tick(deltaTime);

		//-------------------Cube-----------------------
		program.use();
		
		glm::mat4 view = camera->GetViewMatrix();
		float screenWidth = (float)Window->GetWidth();
		float screenHeight = (float)Window->GetHeight();
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(camera->GetZoom()), screenWidth / screenHeight, 0.1f, 100.0f);
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0));

		program.setMat4("view", view);
		program.setMat4("projection", projection);
		program.setMat4("model", model);
		program.setVec3("viewPos", camera->GetPosition());

		program.setInt("material.diffuse", 0);
		//program.setInt("material.specular", 1);
		material.GetDiffuseMap()->activate(0);
		//material.GetSpecularMap()->activate(1);
		program.setFloat("material.shininess", material.GetShininess());

		float rotateSpeed = 50.0f;
		glm::mat4 rotateMat(1.0f);
		rotateMat = glm::rotate(rotateMat, glm::radians(rotateSpeed * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));//light实例声明在循环外 使用currentFrame作为参数
		glm::vec4 newPos = rotateMat * glm::vec4(light.GetPosition(), 0.0f);
		light.SetPosition(newPos);

		//program.setVec3("light.ambient", light.GetAmbient());
		program.setVec3("light.diffuse", light.GetDiffuse());
		program.setVec3("light.specular", light.GetSpecular());
		program.setVec3("light.position", light.GetPosition());

		DrawPlane();

		Window->SwapBuffer(deltaTime);
	}

	glfwTerminate();

	return 0;
}

std::string shaderPath(std::string path)
{
	std::string rootPath{ "../../"};
	return rootPath + projectName + "/ShaderSource/" + path;
}

void InitInput()
{
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Input->BindInput(GLFW_KEY_W, [&] { camera->AddMovement(Core::Movement::FORWARD, 1); });
	Input->BindInput(GLFW_KEY_S, [&] { camera->AddMovement(Core::Movement::FORWARD, -1); });
	Input->BindInput(GLFW_KEY_D, [&] { camera->AddMovement(Core::Movement::RIGHT, 1); });
	Input->BindInput(GLFW_KEY_A, [&] { camera->AddMovement(Core::Movement::RIGHT, -1); });
	Input->BindInput(GLFW_KEY_ESCAPE, [&] { glfwSetWindowShouldClose(window, true); });
}

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
}

void scroll_callback(GLFWwindow* _window, double xoffset, double yoffset)
{
	camera->AddZoom((float)-yoffset);
}


float planeVertices[] = {
	// positions            // normals         // texcoords
	 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
	-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

	 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
	-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
	 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
};

void InitVertex()
{
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}

void DrawPlane()
{
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
