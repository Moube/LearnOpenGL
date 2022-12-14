#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "Window/Window.h"
#include "Shader/ShaderProgram.h"
#include "Util/Log.h"
#include "Texture/Texture2D.h"

unsigned int VAO, VBO, EBO;
unsigned int texture1, texture2;
void InitVertex();
void Draw();
unsigned int CreateTexture(const char* path);

int main()
{
	using namespace Core;
	//using namespace glm;
    Window::Instance()->Init();
    GLFWwindow* window = Window::Instance()->GetWindow();
	InitVertex();
	ShaderProgram program("../../Transformations/ShaderSource/shader.vert", "../../Transformations/ShaderSource/shader.frag");

	/* hello transform
	vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	mat4 trans(1.0f);
	trans = translate(trans, vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;
	*/

	//加载图片
	Texture2D::flipVertically();
	Texture2D texture1("container.jpg");
	Texture2D texture2("awesomeface.png");

	//纹理单元
	program.use();
	program.setInt("texture1", 0);
	program.setInt("texture2", 1);
	
	while (!glfwWindowShouldClose(window))
	{
		Window::Instance()->ProcessInput(0);
		Window::Instance()->PreProcess(0);
		
		program.use();

		//绑定纹理
		texture1.activate(0);
		texture2.activate(1);

		float time = (float)glfwGetTime();

		//旋转
		glm::mat4 trans(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, time, glm::vec3(0.0f, 0.0f, 1.0f));
		
		//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));//旋转90度
		
		//练习1.使用应用在箱子上的最后一个变换，尝试将其改变为先旋转，后位移。看看发生了什么，试着想想为什么会发生这样的事情
		//矩阵乘法不满足交换律, trans的位移、缩放、旋转的作用顺序对最终结果有影响
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		program.setMat4("transform", trans);
		Draw();

		//练习2.尝试再次调用glDrawElements画出第二个箱子，只使用变换将其摆放在不同的位置。让这个箱子被摆放在窗口的左上角，并且会不断的缩放（而不是旋转）。（sin函数在这里会很有用，不过注意使用sin函数时应用负值会导致物体被翻转）
		trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scale = sin(time);
		scale = scale > 0 ? scale : -scale;
		trans = glm::scale(trans, glm::vec3(scale, scale, 0.0f));
		program.setMat4("transform", trans);
		Draw();

		Window::Instance()->SwapBuffer(0);
	}

	glfwTerminate();

    return 0;
}

void InitVertex()
{
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3,
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//颜色属性
	//			  位置属性1, 3个float				步长,				偏移
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	//纹理属性
	//			  位置属性2, 2个float				步长,				偏移
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}