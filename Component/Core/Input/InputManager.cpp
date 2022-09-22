#include "InputManager.h"
#include "Window/Window.h"

namespace Core
{
	void InputManager::BindInput(int key, InputExecute handler)
	{
		auto inputHandler = std::make_shared<InputHandler>();
		inputHandler->Bind(handler);
		inputHandlerMap.insert({ key, inputHandler });
	}

	void InputManager::ProcessInput()
	{
		GLFWwindow* window = Window::Instance()->GetWindow();
		for (InputIterator iter = inputHandlerMap.begin(); iter != inputHandlerMap.end(); iter++)
		{
			int key = iter->first;
			if (glfwGetKey(window, key) == GLFW_PRESS)
				iter->second->Execute();
		}
	}
}