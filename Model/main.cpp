#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Util/Math.h"
#include "Mesh/Model.h"
#include "Light/Light.h"
#include "Asset/Asset.h"
#include "Window/Window.h"
#include "Camera/Camera.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#include "Input/InputManager.h"
#include "Shader/ShaderProgram.h"
#include "Light/DirectionalLight.h"

GLFWwindow* window = nullptr;
Core::Camera* camera = nullptr;
Core::InputManager* Input = nullptr;
std::string projectName = "Model";
std::string shaderPath(std::string path);
float lastFrame = 0.0f;
float deltaTime = 0.0f;
float totalTime = 0.0f;
void InitInput();

bool firstMouse = true;
float lastX = 400, lastY = 300;//鼠标初始位置
void mouse_callback(GLFWwindow* _window, double xpos, double ypos);
void scroll_callback(GLFWwindow* _window, double xoffset, double yoffset);

glm::vec3 cubePos(0.0f);

int main()
{
	Core::Window* Window = Core::Window::Instance();
	Input = Core::InputManager::Instance();
	Window->Init();
	window = Window->GetWindow();
	camera = new Core::Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	InitInput();

	Core::ShaderProgram program(shaderPath("shader.vert"), shaderPath("shader.frag"));

	Core::Light metaPointLight(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	Core::DirectionalLight directionLight(
		glm::vec3(-0.2f, -1.0f, -0.3f),
		glm::vec3(1.0f, 1.0f, 1.0),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	Core::Light metaSpotLight(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f));

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)};

	std::vector<Core::PointLight> pointLights;
	for (int i = 0; i < 4; i++)
	{
		Core::PointLight pointLight(pointLightPositions[i], metaPointLight);
		pointLight.SetAttenuation(1.0f, 0.09f, 0.032f);
		pointLights.push_back(pointLight);
	}

	Core::Model myModel("nanosuit/nanosuit.obj");

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
		
		std::string backfix = "].";
		for (int i = 0; i < pointLights.size(); i++)
		{
			std::string prefix = "pointLights[";
			prefix.append(std::to_string(i));
			prefix.append(backfix);
			program.setVec3(prefix + "ambient", pointLights[i].GetAmbient());
			program.setVec3(prefix + "diffuse", pointLights[i].GetDiffuse());
			program.setVec3(prefix + "specular", pointLights[i].GetSpecular());
			program.setVec3(prefix + "position", pointLights[i].GetPosition());
			program.setFloat(prefix + "constant", pointLights[i].GetConstant());
			program.setFloat(prefix + "linear", pointLights[i].GetLinear());
			program.setFloat(prefix + "quadratic", pointLights[i].GetQuadratic());
		}
		
		//聚光
		Core::SpotLight spotLight(camera->GetPosition(), camera->GetFront(), 12.5f, 17.5f, metaSpotLight);
		program.setVec3("spotLight.ambient", spotLight.GetAmbient());
		program.setVec3("spotLight.diffuse", spotLight.GetDiffuse());
		program.setVec3("spotLight.specular", spotLight.GetSpecular());
		program.setVec3("spotLight.position", spotLight.GetPosition());
		program.setVec3("spotLight.direction", spotLight.GetDirection());
		program.setFloat("spotLight.cutOff", cos(glm::radians(spotLight.GetCutoff())));
		program.setFloat("spotLight.outerCutoff", cos(glm::radians(spotLight.GetOuterCutoff())));

		//定向光
		program.setVec3("dirLight.ambient", directionLight.GetAmbient());
		program.setVec3("dirLight.diffuse", directionLight.GetDiffuse());
		program.setVec3("dirLight.specular", directionLight.GetSpecular());
		program.setVec3("dirLight.direction", directionLight.GetDirection());

		glm::mat4 view = camera->GetViewMatrix();
		float screenWidth = (float)Window->GetWidth();
		float screenHeight = (float)Window->GetHeight();
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(camera->GetZoom()), screenWidth / screenHeight, 0.1f, 100.0f);

		program.setMat4("view", view);
		program.setMat4("projection", projection);
		program.setVec3("viewPos", camera->GetPosition());

		glm::mat4 model(1.0f);
		model = glm::translate(model, cubePos);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		program.setMat4("model", model);

		//踩坑:
		//缺了shininess属性, 贴图计算直接全部黑掉
		program.setFloat("material.shininess", 32);
		myModel.Draw(program);

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