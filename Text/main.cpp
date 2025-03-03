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
#include "Input/InputManager.h"
#include "Camera/Camera.h"
#include "Text/Text.h"

GLFWwindow* window = nullptr;
Core::TextManager* _TextManager = nullptr;

int main()
{
	using namespace Core;
	Window* WindowInstance = Window::Instance();
	_TextManager = TextManager::Instance();
	WindowInstance->Init(800, 600);
	window = WindowInstance->GetWindow();
	_TextManager->Init();

	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		WindowInstance->PreProcess(deltaTime);

		_TextManager->RenderText(L"(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), "fonts/arial.ttf");
		_TextManager->RenderText(L"This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), "fonts/arial.ttf");
		_TextManager->RenderText(L"ttc file", 300.0f, 300.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), "fonts/msyh.ttc");
		_TextManager->RenderText(L"尝试渲染中文", 300.0f, 250.0f, 1.0f, glm::vec3(0.9, 0.1f, 0.1f), "fonts/msyh.ttc");
	
		WindowInstance->SwapBuffer(deltaTime);
	}

	glfwTerminate();

	return 0;
}

