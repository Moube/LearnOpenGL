#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <memory>
#include "Singleton/Singleton.h"
#include "InputHandler.h"

namespace Core
{
    class InputManager : public Singleton<InputManager>
    {
    public:
        void BindInput(int key, InputExecute handler);

        void ProcessInput();
    private:
        using InputHandlerMap = std::map<int, std::shared_ptr<InputHandler>>;
        using InputIterator = InputHandlerMap::iterator;
        InputHandlerMap inputHandlerMap;

    private:
        InputManager() {};
        ~InputManager() {};
        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager(InputManager&& other) noexcept = default;
        InputManager& operator=(InputManager&& other) noexcept = default;
        friend class Singleton<InputManager>;
    };
}

