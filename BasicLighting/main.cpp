#pragma once

#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "Util/Log.h"
#include "Util/Math.h"
#include "Window/Window.h"
#include "Camera/Camera.h"
#include "Texture/Texture2D.h"
#include "Input/InputManager.h"
#include "Shader/ShaderProgram.h"

GLFWwindow* window = nullptr;
Core::Camera* camera = nullptr;
Core::InputManager* Input = nullptr;
std::string projectName = "BasicLighting";
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
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 cubePos(0.0f);

//练习2.尝试使用不同的环境光、漫反射和镜面强度，观察它们怎么是影响光照效果的。同样，尝试实验一下镜面光照的反光度因子。尝试理解为什么某一个值能够有着某一种视觉输出。
float ambientStrength = 0.1f;
float specularStrength = 0.5f;
float ambientChangeSpeed = 1.0f;
float specularChangeSpeed = 1.0f;

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
	//练习4.
	//ShaderProgram program(shaderPath("shaderPractice4.vert"), shaderPath("shaderPractice4.frag"));
	Core::ShaderProgram lightProgram(shaderPath("shader.vert"), shaderPath("lightShader.frag"));

	float yaw = 0.0f;//练习1
	float radius = 2.0f;//练习1

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		totalTime += deltaTime;
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
		model = glm::translate(model, cubePos);

		program.setMat4("view", view);
		program.setMat4("projection", projection);
		program.setMat4("model", model);
		program.set3Float("objectColor", 1.0f, 0.5f, 0.31f);
		program.set3Float("lightColor", 1.0f, 1.0f, 1.0f);
		program.setVec3("lightPos", lightPos);
		program.setVec3("viewPos", camera->GetPosition());

		program.setFloat("_ambientStrength", ambientStrength);
		program.setFloat("_specularStrength", specularStrength);

		Draw();

		//----------------光源---------------------

		//练习1.目前，我们的光源是静止的，你可以尝试使用sin或cos函数让光源在场景中来回移动。观察光照随时间的改变能让你更容易理解冯氏光照模型
		//让光源旋转 思路1 成功
		//float rotateSpeed = 50.0f;
		//glm::mat4 rotateMat(1.0f);
		//rotateMat = glm::rotate(rotateMat, glm::radians(rotateSpeed * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::vec4 newPos = rotateMat * glm::vec4(lightPos, 0.0f);
		//lightPos = newPos;

		//让光源旋转 官方思路2 成功
		//float rotateSpeed = 50.0f;
		//yaw += rotateSpeed * deltaTime;
		//lightPos = glm::vec3(radius * cos(glm::radians(yaw)), 1.0f, radius * sin(glm::radians(yaw)));

		//练习2.
		float moveSpeed = 1.0f;
		float maxRadius = 5.0f;
		radius = maxRadius * sin(totalTime * moveSpeed);
		lightPos = glm::vec3(radius * cos(glm::radians(yaw)), 1.0f, radius * sin(glm::radians(yaw)));

		lightProgram.use();
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightProgram.setMat4("model", model);
		lightProgram.setMat4("view", view);
		lightProgram.setMat4("projection", projection);

		DrawLight();

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
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

void InitVertex3D()
{
	//没有使用EBO

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//位置属性
	//			  位置属性0, 3个float				步长,				偏移
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//法向量
	//			  位置属性0, 3个float				步长,				偏移
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

void Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
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

	//练习2.
	
	Input->BindInput(GLFW_KEY_U, [&] {
		ambientStrength += ambientChangeSpeed * deltaTime;
		Core::Math::Clamp(ambientStrength, 0.0f, 3.0f);
	});
	Input->BindInput(GLFW_KEY_I, [&] {
		ambientStrength -= ambientChangeSpeed * deltaTime;
		Core::Math::Clamp(ambientStrength, 0.0f, 3.0f);
	});
	Input->BindInput(GLFW_KEY_J, [&] {
		specularStrength += specularChangeSpeed * deltaTime;
		Core::Math::Clamp(specularStrength, 0.0f, 3.0f);
		});
	Input->BindInput(GLFW_KEY_K, [&] {
		specularStrength -= specularChangeSpeed * deltaTime;
		Core::Math::Clamp(specularStrength, 0.0f, 3.0f);
	});
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

void InitLight()
{
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

void DrawLight()
{
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}
