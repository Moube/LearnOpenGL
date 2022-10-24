#include "Window.h"
#include <iostream>

namespace Core
{

	bool Window::Init()
	{
		glfwInit();
		//glfwWindowHint(GLFW_SAMPLES, 4);//4倍多采样
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		__width = 800;
		__height = 600;

		window = glfwCreateWindow(__width, __height, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return false;
		}

		glfwSetFramebufferSizeCallback(window, Window::FramebufferSizeCallback);

		glEnable(GL_DEPTH_TEST);//开启深度测试
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏并捕获光标

		backColor = glm::vec3(0.1f, 0.1f, 0.1f);

		return true;
	}

	GLFWwindow* Window::GetWindow()
	{
		return window;
	}

	void Window::ProcessInput(float delta)
	{
		
	}

	void Window::UpdateSize(int width, int height)
	{
		__width = width;
		__height = height;
	}

	void Window::GetSize(int& width, int& height)
	{
		width = __width;
		height = __height;
	}

	void Window::PreProcess(float delta)
	{
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(backColor.x, backColor.y, backColor.z, 1.0f);//从光照开始使用该背景缓冲
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //清除Buffer
	}

	void Window::SwapBuffer(float delta)
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void Window::SetBack(glm::vec3 color)
	{
		backColor = color;
	}

	void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		Window::Instance()->UpdateSize(width, height);
	}
}