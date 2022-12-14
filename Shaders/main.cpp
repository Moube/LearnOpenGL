#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include  <iostream>
#include "Window/Window.h"
#include "Shader/ShaderProgram.h"

void InitVertex();
void DrawVertex();

int main()
{
	using namespace Core;
    Window::Instance()->Init();
    GLFWwindow* window = Window::Instance()->GetWindow();

	InitVertex();

	//绝对路径
	//Shader firstShader("F:/MyProject/LearnOpenGL/Shaders/ShaderSource/shader.vert", "F:/MyProject/LearnOpenGL/Shaders/ShaderSource/shader.frag");
	//相对路径
	//vcxproj文件所在路径为起点
	ShaderProgram program("../../Shaders/ShaderSource/shader.vert", "../../Shaders/ShaderSource/shader.frag");
	
	while (!glfwWindowShouldClose(window))
	{
		Window::Instance()->ProcessInput(0);
		Window::Instance()->PreProcess(0);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.use();

		float timeValue = (float)glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		program.set4Float("uniformColor", 0.0f, greenValue, 0.0f, 0.0f);

		program.setFloat("offset", 0.5f);

		DrawVertex();

		Window::Instance()->SwapBuffer(0);
	}

	glfwTerminate();

    return 0;
}

unsigned int VAO, VBO, EBO;
void InitVertex()
{
	float vertices[] =
	{
		//位置				//颜色
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] =
	{
		0, 1, 2,
	};

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//位置属性
	//			  位置属性0, 3个float				步长,				偏移
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//颜色属性
	//			  位置属性1, 3个float				步长,				偏移
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void DrawVertex()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}