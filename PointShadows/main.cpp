#pragma once

#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
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
std::string projectName = "PointShadows";
std::string shaderPath(std::string path);
float lastFrame = 0.0f;
float deltaTime = 0.0f;
float totalTime = 0.0f;

void InitInput();
float lastX = 400, lastY = 300;//鼠标初始位置
bool firstMouse = true;
void mouse_callback(GLFWwindow* _window, double xpos, double ypos);
void scroll_callback(GLFWwindow* _window, double xoffset, double yoffset);

unsigned int cubeVAO, cubeVBO;
void InitVertex();
void DrawCube();
void DrawScene(const Core::ShaderProgram& program);

int main()
{
	Core::Window* Window = Core::Window::Instance();
	Input = Core::InputManager::Instance();
	Window->Init();
	window = Window->GetWindow();
	camera = new Core::Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	InitInput();
	InitVertex();

	glEnable(GL_DEPTH_TEST);

	unsigned int depthMapFBO, depthCubeMap;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glGenFramebuffers(1, &depthMapFBO);

	//深度cube map
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	for (GLuint i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	//GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);//超出深度贴图范围的部分均取1.0的深度值
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//绑定
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glDrawBuffer(GL_NONE);//告诉openGL该缓冲不需要颜色组件
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Core::ShaderProgram sceneProgram(shaderPath("shader.vert"), shaderPath("shader.frag"));
	Core::ShaderProgram shadowProgram(shaderPath("shadowShader.vert"), shaderPath("shadowShader.frag"), shaderPath("shadowShader.geom"));

	float screenWidth = (float)Window->GetWidth();
	float screenHeight = (float)Window->GetHeight();

	Core::Texture2D woodTexture("wood.png");

	sceneProgram.use();
	sceneProgram.setInt("diffuseTexture", 0);
	sceneProgram.setInt("shadowMap", 1);

	glm::vec3 lightPos(0.0f);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//Window->PreProcess(deltaTime);
		Input->ProcessInput();
		camera->Tick(deltaTime);

		//---------------DepthMap--------------------

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//计算方向光的光源空间矩阵
		glm::vec3 up;
		glm::vec3 front;
		glm::mat4 lightProjection(1.0f);
		
		float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
		float near_plane = 1.0f, far_plane = 25.0f;
		lightProjection = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);

		std::vector<glm::mat4> lightTransforms;
		auto createLightTransform = [&](const glm::vec3& front, const glm::vec3& up)
		{
			lightTransforms.push_back(lightProjection * glm::lookAt(lightPos, lightPos + front, up));
		};

		//这个顺序很重要！！！
		//前--------------------------
		up = glm::vec3(0.0, -1.0, 0.0f);
		front = glm::vec3(1.0f, 0.0f, 0.0f);
		createLightTransform(front, up);
		//后
		front = glm::vec3(-1.0f, 0.0f, 0.0f);
		createLightTransform(front, up);

		//上----------------------------
		up = glm::vec3(0.0, 0.0, 1.0f);
		front = glm::vec3(0.0f, 1.0f, 0.0f);
		createLightTransform(front, up);
		//下
		up = glm::vec3(0.0, 0.0, -1.0f);
		front = glm::vec3(0.0f, -1.0f, 0.0f);
		createLightTransform(front, up);

		//右---------------------------
		up = glm::vec3(0.0, -1.0, 0.0f);
		front = glm::vec3(0.0f, 0.0f, 1.0f);
		createLightTransform(front, up);
		//左
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		createLightTransform(front, up);
		
		shadowProgram.use();
		for (int i = 0; i < lightTransforms.size(); i++)
		{
			std::stringstream ss;
			ss << "shadowMatrices[";
			ss << i;
			ss << "]";
			shadowProgram.setMat4(ss.str(), lightTransforms[i]);
		}
		shadowProgram.setVec3("lightPos", lightPos);
		shadowProgram.setFloat("far_plane", far_plane);

		//开始绘制depth map
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			DrawScene(shadowProgram);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		//---------------DepthMap End--------------------

		//--------------------DrawScene----------------------

		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneProgram.use();

		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(camera->GetZoom()), screenWidth / screenHeight, 0.1f, 100.0f);

		sceneProgram.setMat4("view", view);
		sceneProgram.setMat4("projection", projection);
		sceneProgram.setVec3("viewPos", camera->GetPosition());
		sceneProgram.setVec3("lightPos", lightPos);
		sceneProgram.setFloat("far_plane", far_plane);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture.getID());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		DrawScene(sceneProgram);

		//--------------------DrawScene End------------------

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

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
	// Front face
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
	// Left face
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
	// Right face
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
	// Bottom face
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
	// Top face
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left         
};

void InitVertex()
{
	//cube
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}

void DrawCube()
{
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void DrawScene(const Core::ShaderProgram& program)
{
	// big cube
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(10.0));
	program.setMat4("model", model);
	program.setBool("reverse_normals", 1);
	DrawCube();

	program.setBool("reverse_normals", 0);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
	program.setMat4("model", model);
	DrawCube();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
	model = glm::scale(model, glm::vec3(1.5));
	program.setMat4("model", model);
	DrawCube();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
	program.setMat4("model", model);
	DrawCube();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
	program.setMat4("model", model);
	DrawCube();

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(1.5));
	program.setMat4("model", model);
	DrawCube();
}