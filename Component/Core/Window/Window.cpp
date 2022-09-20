#include "Window.h"
#include <iostream>

namespace Core
{

	bool Window::Init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

		return true;
	}

	GLFWwindow* Window::GetWindow()
	{
		return window;
	}

	void Window::processInput()
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void Window::updateSize(int width, int height)
	{
		__width = width;
		__height = height;
	}

	void Window::getSize(int& width, int& height)
	{
		width = __width;
		height = __height;
	}

	void Window::PreProcess()
	{
		processInput();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Çå³ýÉî¶È»º´æ
	}

	void Window::PostProcess()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		Window::Instance()->updateSize(width, height);
	}
}