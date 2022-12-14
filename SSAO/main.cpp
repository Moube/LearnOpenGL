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
std::string projectName = "SSAO";
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

unsigned int cubeVAO, cubeVBO;
unsigned int quadVAO, quadVBO;
void InitVertex();
void DrawCube();
void DrawQuad();

unsigned int gBuffer;
unsigned int gPositionDepth, gNormal, gAlbedoSpec;
unsigned int depthRBO;
void InitDeferredShading();

int main()
{
	Core::Window* Window = Core::Window::Instance();
	Input = Core::InputManager::Instance();
	Window->Init();
	window = Window->GetWindow();
	camera = new Core::Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	InitInput();
	InitVertex();
	InitDeferredShading();

	glEnable(GL_DEPTH_TEST);

	float screenWidth = (float)Window->GetWidth();
	float screenHeight = (float)Window->GetHeight();

	Core::ShaderProgram geometryPass(shaderPath("gBufferShader.vert"), shaderPath("gBufferShader.frag"));
	Core::ShaderProgram lightingPass(shaderPath("quadShader.vert"), shaderPath("lightingShader.frag"));
	Core::ShaderProgram ssaoShader(shaderPath("quadShader.vert"), shaderPath("ssaoShader.frag"));
	Core::ShaderProgram ssaoBlurShader(shaderPath("quadShader.vert"), shaderPath("ssaoBlurShader.frag"));

	lightingPass.use();
	lightingPass.setInt("gPositionDepth", 0);
	lightingPass.setInt("gNormal", 1);
	lightingPass.setInt("gAlbedoSpec", 2);
	lightingPass.setInt("ssao", 3);

	ssaoShader.use();
	ssaoShader.setInt("gPositionDepth", 0);
	ssaoShader.setInt("gNormal", 1);
	ssaoShader.setInt("texNoise", 2);

	ssaoBlurShader.use();
	ssaoBlurShader.setInt("ssaoInput", 0);

	Core::Model nanosuit("nanosuit/nanosuit.obj");

	std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
	std::default_random_engine generator;

	//SSAO Normal-oriented Hemisphere
	std::vector<glm::vec3> ssaoKernel;
	for (unsigned int i = 0; i < 64; ++i)
	{
		//xy在[-1, 1], z在[0, 1], 取样核心便形成了半球形
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);

		sample = glm::normalize(sample);
		sample *= randomFloats(generator);

		//用插值的方式让采样点更靠近原点
		float scale = float(i) / 64.0;
		scale = lerp(0.1f, 1.0f, scale * scale);//二次方, 加速曲线
		sample *= scale;

		ssaoKernel.push_back(sample);
	}

	//Noise Kernel
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; ++i)
	{
		//在tangent space, 围绕z轴旋转
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f
		);
		ssaoNoise.push_back(noise);
	}

	unsigned int noiseTexture;
	glGenTextures(1, &noiseTexture);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	//SSAO bufer
	unsigned int ssaoFBO;
	unsigned int ssaoColor;
	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	glGenTextures(1, &ssaoColor);
	glBindTexture(GL_TEXTURE_2D, ssaoColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, screenWidth, screenHeight, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColor, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Core::Print("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//SSAO Blur
	unsigned int ssaoBlurFBO;
	unsigned int ssaoBlurColor;
	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);

	glGenTextures(1, &ssaoBlurColor);
	glBindTexture(GL_TEXTURE_2D, ssaoBlurColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, screenWidth, screenHeight, 0, GL_RED, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBlurColor, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Core::Print("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//Window->PreProcess(deltaTime);
		Input->ProcessInput();
		camera->Tick(deltaTime);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera->GetZoom()), screenWidth / screenHeight, 0.1f, 100.0f);

		//--------------------G-Buffer-----------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			geometryPass.use();
			geometryPass.setMat4("view", view);
			geometryPass.setMat4("projection", projection);

			//floor
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0, -1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
			geometryPass.setMat4("model", model);
			DrawCube();

			//nanosuit
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));//教程示例代码没加radians
			model = glm::scale(model, glm::vec3(0.5f));
			geometryPass.setMat4("model", model);
			nanosuit.Draw(geometryPass);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//--------------------G-Buffer End--------------

		//-----------------SSAO----------------------
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		{
			glClear(GL_COLOR_BUFFER_BIT);
			ssaoShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPositionDepth);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNormal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, noiseTexture);
			ssaoShader.setMat4("projection", projection);
			for (int i = 0; i < ssaoKernel.size(); i++)
			{
				ssaoShader.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
			}
			DrawQuad();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//-----------------SSAO End----------------------

		
		//-----------------SSAO Blur----------------------
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		{
			glClear(GL_COLOR_BUFFER_BIT);
			ssaoBlurShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ssaoColor);
			DrawQuad();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//-----------------SSAO Blur End----------------------
		

		//--------------------Lighting----------------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			lightingPass.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gPositionDepth);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNormal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, ssaoBlurColor);
			//glBindTexture(GL_TEXTURE_2D, ssaoColor);
			glDisable(GL_DEPTH_TEST);

			glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
			glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

			glm::vec3 lightPosView = glm::vec3(view * glm::vec4(lightPos, 1.0));
			//lightingPass.setVec3("light.Position", lightPos);
			lightingPass.setVec3("light.Position", lightPosView);//观察空间
			lightingPass.setVec3("light.Color", lightColor);

			const float constant = 1.0f;
			const float linear = 0.7f;
			const float quadratic = 1.8f;
			lightingPass.setFloat("light.Linear", linear);
			lightingPass.setFloat("light.Quadratic", quadratic);
			//lightingPass.setVec3("viewPos", camera->GetPosition());
			DrawQuad();
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

float quadVertices[] = {
	// positions        // texture Coords
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

float vertices[] = {
	// Back face
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//quad
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

void DrawCube()
{
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void DrawQuad()
{
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void InitDeferredShading()
{
	Core::Window* Window = Core::Window::Instance();
	float screenWidth = (float)Window->GetWidth();
	float screenHeight = (float)Window->GetHeight();
	//----------------G-buffer---------------------------

//g-buffer
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//位置
	glGenTextures(1, &gPositionDepth);
	glBindTexture(GL_TEXTURE_2D, gPositionDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);//改成vec4, 存储深度信息
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPositionDepth, 0);

	//法线
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	//颜色+镜面
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	//显示提醒OpenGL要输出的颜色附件
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	//深度组件
	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Core::Print("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}