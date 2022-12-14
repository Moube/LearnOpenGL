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

GLFWwindow* window = nullptr;
unsigned int VAO, VBO, EBO;
unsigned int texture1, texture2;
void InitVertex3D();
void Draw();

//练习1.对GLM的projection函数中的FoV和aspect-ratio参数进行实验。看能否搞懂它们是如何影响透视平截头体的。
//fov影响frustum的开合角度，视野广度, 过高过低的数值会产生不真实感
//aspect影响投影结果2D屏幕空间的比率，与窗口比例尺寸不一致时会导致拉伸
float fov = 45.0f;
float screenRate = 1.0f;
//练习2.将观察矩阵在各个方向上进行位移，来看看场景是如何改变的。注意把观察矩阵当成摄像机对象。
glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
void ProcessInput();

int main()
{
	using namespace Core;
	Window* WindowInstance = Window::Instance();
	WindowInstance->Init();
	glEnable(GL_DEPTH_TEST);//开启深度测试

    window = WindowInstance->GetWindow();
	InitVertex3D();
	ShaderProgram program("../../CoordinateSystems/ShaderSource/shader.vert", "../../CoordinateSystems/ShaderSource/shader.frag");

	Texture2D::flipVertically();
	Texture2D texture1("container.jpg");
	Texture2D texture2("awesomeface.png");

	program.use();
	program.setInt("texture1", 0);
	program.setInt("texture2", 1);
	
	while (!glfwWindowShouldClose(window))
	{
		WindowInstance->PreProcess(0);
		ProcessInput();

		program.use();
		texture1.activate(0);
		texture2.activate(1);

		float time = (float)glfwGetTime();

		//model matrix
		//glm::mat4 model(1.0f);
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		//view matrix
		glm::mat4 view(1.0f);
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

		//练习2.将观察矩阵在各个方向上进行位移，来看看场景是如何改变的。注意把观察矩阵当成摄像机对象。
		view = glm::translate(view, -cameraPos);

		//perspective projection matrix
		float screenWidth = (float)WindowInstance->GetWidth();
		float screenHeight = (float)WindowInstance->GetHeight();
		glm::mat4 projection(1.0f);
		//projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);

		//练习1
		projection = glm::perspective(glm::radians(fov), screenRate * (screenWidth / screenHeight), 0.1f, 100.0f);
		
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

		for (unsigned int i = 0; i < 10; i++)
		{
			//model matrix
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			//float angle = 20.0f * i;
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//练习3.使用模型矩阵只让是3倍数的箱子旋转（以及第1个箱子），而让剩下的箱子保持静止。
			float angle = 20.0f * (i + 1);
			if (i % 3 == 0 || i == 1)
				model = glm::rotate(model, time * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			else
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			
			program.setMat4("model", model);
			Draw();
		}

		WindowInstance->SwapBuffer(0);
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

void ProcessInput()
{
	//或许应该做个封装
	float fovChangeSpeed = 0.1f;
	float screenChangeRate = 0.01f;
	float cameraMoveSpeed = 0.01f;
	int wKey = glfwGetKey(window, GLFW_KEY_W);
	int sKey = glfwGetKey(window, GLFW_KEY_S);
	int aKey = glfwGetKey(window, GLFW_KEY_A);
	int dKey = glfwGetKey(window, GLFW_KEY_D);

	int upKey = glfwGetKey(window, GLFW_KEY_UP);
	int downKey = glfwGetKey(window, GLFW_KEY_DOWN);
	int leftKey = glfwGetKey(window, GLFW_KEY_LEFT);
	int rightKey = glfwGetKey(window, GLFW_KEY_RIGHT);
	int riseKey = glfwGetKey(window, GLFW_KEY_PAGE_UP);
	int sinkKey = glfwGetKey(window, GLFW_KEY_PAGE_DOWN);

	if (wKey == GLFW_PRESS)
	{
		fov += fovChangeSpeed;
	}
	else if (sKey == GLFW_PRESS)
	{
		fov -= fovChangeSpeed;
	}
	else if (aKey == GLFW_PRESS)
	{
		screenRate += screenChangeRate;
	}
	else if (dKey == GLFW_PRESS)
	{
		screenRate -= screenChangeRate;
	}
	else if (upKey == GLFW_PRESS)
	{
		cameraPos.z -= cameraMoveSpeed;
	}
	else if (downKey == GLFW_PRESS)
	{
		cameraPos.z += cameraMoveSpeed;
	}
	else if (leftKey == GLFW_PRESS)
	{
		cameraPos.x -= cameraMoveSpeed;
	}
	else if (rightKey == GLFW_PRESS)
	{
		cameraPos.x += cameraMoveSpeed;
	}
	else if (riseKey == GLFW_PRESS)
	{
		cameraPos.y += cameraMoveSpeed;
	}
	else if (sinkKey == GLFW_PRESS)
	{
		cameraPos.y -= cameraMoveSpeed;
	}

	fov = fov < 0.0f ? 0.0f : fov;
	fov = fov > 180.0f ? 180.0f : fov;
	screenRate = screenRate < 0.0f ? 0.0f : screenRate;
	screenRate = screenRate > 10.0f ? 10.0f : screenRate;
}