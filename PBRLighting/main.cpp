#pragma once

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Util/Math.h"
#include "Util/Log.h"
#include "Asset/Asset.h"
#include "Mesh/Model.h"
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
std::string projectName = "PBRLighting";
std::string shaderPath(std::string path);
float lastFrame = 0.0f;
float deltaTime = 0.0f;
float totalTime = 0.0f;
float lerp(float a, float b, float f);

void InitInput();
float lastX = 400, lastY = 300;//鼠标初始位置
bool firstMouse = true;
void mouse_callback(GLFWwindow* _window, double xpos, double ypos);
void scroll_callback(GLFWwindow* _window, double xoffset, double yoffset);

unsigned int sphereVAO, sphereVBO, sphereEBO;
void InitVertex();
void DrawSphere();

int main()
{
	Core::Window* Window = Core::Window::Instance();
	Input = Core::InputManager::Instance();
	Window->Init();
	window = Window->GetWindow();
	camera = new Core::Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	camera->SetMovespeed(25.f);

	InitInput();
	InitVertex();

	glEnable(GL_DEPTH_TEST);

	float screenWidth = (float)Window->GetWidth();
	float screenHeight = (float)Window->GetHeight();

	Core::ShaderProgram program(shaderPath("shader.vert"), shaderPath("shader.frag"));
	Core::ShaderProgram textureProgram(shaderPath("textureShader.vert"), shaderPath("textureShader.frag"));

	program.use();
	program.setVec3("albedo", glm::vec3(0.5f, 0.5f, 0.5f));
	program.setFloat("ao", 1.0f);

	textureProgram.use();
	textureProgram.setInt("normalMap", 0);
	textureProgram.setInt("albedoMap", 1);
	textureProgram.setInt("metallicMap", 2);
	textureProgram.setInt("roughnessMap", 3);
	textureProgram.setInt("aoMap", 4);

	Core::Texture2D normalTex("rusted_iron/normal.png");
	Core::Texture2D albedoTex("rusted_iron/albedo.png");
	Core::Texture2D metallicTex("rusted_iron/metallic.png");
	Core::Texture2D roughnessTex("rusted_iron/roughness.png");
	Core::Texture2D aoTex("rusted_iron/ao.png");
	
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Window->PreProcess(deltaTime);
		Input->ProcessInput();
		camera->Tick(deltaTime);

		glm::mat4 model(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), screenWidth / screenHeight, 0.1f, 100.0f);

		textureProgram.use();
		textureProgram.setMat4("view", view);
		textureProgram.setMat4("projection", projection);
		textureProgram.setVec3("camPos", camera->GetPosition());

		normalTex.activate(0);
		albedoTex.activate(1);
		metallicTex.activate(2);
		roughnessTex.activate(3);
		aoTex.activate(4);
		for (int row = 0; row < nrRows; row++)
		{
			//textureProgram.setFloat("metallic", (float)row / (float)nrRows);
			for (int col = 0; col < nrColumns; col++)
			{
				//限制粗糙度最小值为0.05, 直接光照下粗糙度为0的物体看起来有点奇怪
				//textureProgram.setFloat("roughness", Core::Math::Clamp((float)col / (float)nrColumns, 0.05f, 1.0f));
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(col - (nrColumns / 2)) * spacing,
					(row - (nrRows / 2)) * spacing,
					0.0f
				));
				textureProgram.setMat4("model", model);
				DrawSphere();
			}		
		}

		//使用同一个program来渲染灯光
		//脑袋有点没饶过弯来
		//疑惑:为什么光照信息和材质信息可以分来传？
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); i++)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];

			textureProgram.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			textureProgram.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			textureProgram.setMat4("model", model);

			//把此处的DrawSphere注释掉
			//仍然可以渲染出正确的画面
			//uniform的信息可以延后传入???
			DrawSphere();
		}

		Window->SwapBuffer(deltaTime);
	}

	glfwTerminate();

	return 0;
}

float lerp(float a, float b, float f)
{
	return a + f * ( b - a );
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

unsigned int indexCount;
void InitVertex()
{
	glGenVertexArrays(1, &sphereVAO);

	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uv;
	std::vector<unsigned int> indices;

	//生成顶点
	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359f;
	for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	{
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			float xSegment = float(x) / float(X_SEGMENTS);
			float ySegment = float(y) / float(Y_SEGMENTS);

			float xPos = std::sin(ySegment * PI) * std::cos(xSegment * 2.0f * PI);
			float zPos = std::sin(ySegment * PI) * std::sin(xSegment * 2.0f * PI);
			float yPos = std::cos(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
		}
	}

	//index
	bool oddRow = false;
	for (unsigned int y = 0; y <= Y_SEGMENTS; y++)
	{
		if (oddRow)
		{
			for (int x = X_SEGMENTS; x >= 0; x--)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y		  * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = 0; x <= X_SEGMENTS; x++)
			{
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	indexCount = static_cast<unsigned int>(indices.size());

	//
	std::vector<float> data;
	for (unsigned int i = 0; i < positions.size(); i++)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}

		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		unsigned int stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
	}
}

void DrawSphere()
{
	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}