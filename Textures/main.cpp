#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
//#include <algorithm>
#include "Window/Window.h"
#include "Shader/ShaderProgram.h"
#include "Util/Log.h"

unsigned int VAO, VBO, EBO;
unsigned int texture1, texture2;
float mixRate = 0.5f;//练习4
void InitVertex();
void InitVertex2();//练习2.尝试用不同的纹理环绕方式，设定一个从0.0f到2.0f范围内的（而不是原来的0.0f到1.0f）纹理坐标。试试看能不能在箱子的角落放置4个笑脸
void InitVertex3();//练习3.尝试在矩形上只显示纹理图像的中间一部分，修改纹理坐标，达到能看见单个的像素的效果。尝试使用GL_NEAREST的纹理过滤方式让像素显示得更清晰
void ProcessInput(GLFWwindow* window);//练习4.使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度，使用上和下键来改变箱子或笑脸的可见度
void Draw();
unsigned int CreateTexture(const char* path);

int main()
{
	using namespace Core;
    Window::Instance()->Init();
    GLFWwindow* window = Window::Instance()->GetWindow();
	InitVertex();
	//InitVertex2();
	//InitVertex3();
	ShaderProgram program("../../Textures/ShaderSource/shader.vert", "../../Textures/ShaderSource/shader.frag");

	//创建纹理对象
	unsigned int texture1, texture2;

	//加载图片
	stbi_set_flip_vertically_on_load(true);//翻转y轴
	int width, height, nrChannels;
	unsigned char* data = stbi_load("../../Asset/container.jpg", &width, &height, &nrChannels, 0);//路径, 宽度, 高度, 颜色通道数
	if (!data)
	{
		Core::Print("Failed to load texture");
		return 0;
	}

	//创建纹理对象
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);//绑定纹理对象

	//练习3
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//生成纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//生成纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);//创建完纹理后释放图片资源


	//第二张图
	data = stbi_load("../../Asset/awesomeface.png", &width, &height, &nrChannels, 0);
	if (!data)
	{
		Core::Print("Failed to load texture");
		return 0;
	}
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//踩坑 : 这张png有alpha通道, 参数需填GL_RGBA, 不然会创建错误的纹理
	// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);


	//---------------------------------------------------------------------------------

	//glTexParamete函数簇
	//纹理环绕方式
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//纹理过滤
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	//邻近过滤, 取最接近纹理坐标的像素
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//线性过滤, 基于周围像素计算差值
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//多级渐远纹理多虑

	//纹理边缘颜色
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//绑定纹理单元
	program.use();
	program.setInt("texture1", 0);//此处的texture1对应的是glsl中的uniform变量, 而非c++中的texture1
	program.setInt("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		//Window::Instance()->processInput();
		ProcessInput(window);//练习4
		Window::Instance()->PreProcess(0);

		program.use();
		program.setFloat("mixRate", mixRate);//练习4

		//绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

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
		1, 2, 3
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

unsigned int CreateTexture(const char* path)
{
	//加载图片
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);//路径, 宽度, 高度, 颜色通道数
	if (!data)
	{
		Core::Print("Failed to load texture");
		return 0;
	}

	//创建纹理对象
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//生成纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);//生成纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);//创建完纹理后释放图片资源

	return texture;
}

void InitVertex2()
{
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----   - 纹理坐标 -	 - 练习2 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   2.0f, 2.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   2.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f, 2.0f,   // 左上
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//颜色属性
	//			  位置属性1, 3个float				步长,				偏移
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//纹理属性
	//			  位置属性2, 2个float				步长,				偏移
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//练习2属性
	//			  位置属性3, 2个float				步长,				偏移
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void InitVertex3()
{
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.055f, 0.055f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.055f, 0.045f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.045f, 0.045f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.045f, 0.055f    // 左上
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//纹理属性
	//			  位置属性2, 2个float				步长,				偏移
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//练习4
void ProcessInput(GLFWwindow* window)
{
	float rate = 0.01f;
	int upKey = glfwGetKey(window, GLFW_KEY_UP);
	int downKey = glfwGetKey(window, GLFW_KEY_DOWN);
	if (upKey == GLFW_PRESS)
	{
		mixRate += rate;
	}
	else if (downKey == GLFW_PRESS)
	{
		mixRate -= rate;
	}

	//mixRate = std::clamp(mixRate, 0.0f, 1.0f);
	mixRate = mixRate < 0.0f ? 0.0f : mixRate;
	mixRate = mixRate > 1.0f ? 1.0f : mixRate;
	
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, true);
}