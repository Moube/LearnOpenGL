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
#include "Window/Window.h"
#include "Camera/Camera.h"
#include "Light/PointLight.h"
#include "Material/Material.h"
#include "Texture/Texture2D.h"
#include "Input/InputManager.h"
#include "Shader/ShaderProgram.h"
#include "Light/DirectionalLight.h"

GLFWwindow* window = nullptr;
Core::Camera* camera = nullptr;
Core::InputManager* Input = nullptr;
std::string projectName = "LightCasters";
std::string shaderPath(std::string path);
float lastFrame = 0.0f;
float deltaTime = 0.0f;
float totalTime = 0.0f;
unsigned int VAO, VBO, EBO;
unsigned int texture1, texture2;
void InitVertex3D();
void InitInput();
void Draw();

float lastX = 400, lastY = 300;//鼠标初始位置
bool firstMouse = true;
void mouse_callback(GLFWwindow* _window, double xpos, double ypos);
void scroll_callback(GLFWwindow* _window, double xoffset, double yoffset);

unsigned int lightVAO;
void InitLight();
void DrawLight();
glm::vec3 cubePos(0.0f);

int main()
{
	Core::Window* Window = Core::Window::Instance();
	Input = Core::InputManager::Instance();
	Window->Init();
	window = Window->GetWindow();
	camera = new Core::Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	InitInput();
	InitVertex3D();
	InitLight();

	Core::ShaderProgram program(shaderPath("shader.vert"), shaderPath("shader.frag"));
	Core::ShaderProgram lightProgram(shaderPath("shader.vert"), shaderPath("lightShader.frag"));

	Core::Material material(
		std::string("container2.png"),
		std::string("container2_specular.png"),
		32.0f);

	material.CreateEmissionMap("matrix.jpg");

	//点光源
	Core::PointLight pointLight(
		glm::vec3(1.2f, 1.0f, 2.0f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	pointLight.SetAttenuation(1.0f, 0.09f, 0.032f);

	//定向光
	Core::DirectionalLight directionLight(
		glm::vec3(-0.2f, -1.0f, -0.3f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
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
		
		//材质
		program.setInt("material.diffuse", 0);
		program.setInt("material.specular", 1);
		program.setInt("material.emission", 2);
		material.GetDiffuseMap()->activate(0);
		material.GetSpecularMap()->activate(1);
		material.GetEmissionMap()->activate(2);
		program.setFloat("material.shininess", material.GetShininess());

		//光源
		//float rotateSpeed = 50.0f;
		//glm::mat4 rotateMat(1.0f);
		//rotateMat = glm::rotate(rotateMat, glm::radians(rotateSpeed * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::vec4 newPos = rotateMat * glm::vec4(pointLight.GetPosition(), 0.0f);
		//pointLight.SetPosition(newPos);

		//点光
		program.setVec3("pointLight.ambient", pointLight.GetAmbient());
		program.setVec3("pointLight.diffuse", pointLight.GetDiffuse());
		program.setVec3("pointLight.specular", pointLight.GetSpecular());
		//program.setVec3("pointLight.position", pointLight.GetPosition());
		program.setFloat("pointLight.constant", pointLight.GetConstant());
		program.setFloat("pointLight.linear", pointLight.GetLinear());
		program.setFloat("pointLight.quadratic", pointLight.GetQuadratic());
		
		//聚光
		program.setVec3("pointLight.position", camera->GetPosition());
		program.setVec3("pointLight.direction", camera->GetFront());
		program.setFloat("pointLight.cutOff", cos(glm::radians(12.5f)));
		program.setFloat("pointLight.outerCutoff", cos(glm::radians(17.5f)));

		//定向光
		//program.setVec3("directionalLight.ambient", directionLight.GetAmbient());
		//program.setVec3("directionalLight.diffuse", directionLight.GetDiffuse());
		//program.setVec3("directionalLight.specular", directionLight.GetSpecular());
		//program.setVec3("directionalLight.direction", directionLight.GetDirection());

		glm::mat4 view = camera->GetViewMatrix();
		float screenWidth = (float)Window->GetWidth();
		float screenHeight = (float)Window->GetHeight();
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(camera->GetZoom()), screenWidth / screenHeight, 0.1f, 100.0f);

		program.setMat4("view", view);
		program.setMat4("projection", projection);
		program.setVec3("viewPos", camera->GetPosition());

		//glm::mat4 model(1.0f);
		//model = glm::translate(model, cubePos);
		//program.setMat4("model", model);

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

		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			program.setMat4("model", model);
			Draw();
		}

		//Draw();

		//----------------光源cube---------------------
		/*
		lightProgram.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pointLight.GetPosition());
		model = glm::scale(model, glm::vec3(0.2f));
		lightProgram.setMat4("model", model);
		lightProgram.setMat4("view", view);
		lightProgram.setMat4("projection", projection);
		lightProgram.setVec3("lightColor", pointLight.GetDiffuse());
		DrawLight();
		*/

		Window->SwapBuffer(deltaTime);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}

std::string shaderPath(std::string path)
{
	std::string rootPath{ "../../"};
	return rootPath + projectName + "/ShaderSource/" + path;
}

float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

void InitVertex3D()
{
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//位置属性
	//			  位置属性0, 3个float				步长,				偏移
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//法向量
	//			  位置属性1, 3个float				步长,				偏移
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//纹理坐标
	//			  位置属性1, 3个float				步长,				偏移
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
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

void InitLight()
{
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void DrawLight()
{
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
