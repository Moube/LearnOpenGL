#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Singleton/Singleton.h"

namespace Core
{
    class Window : public Singleton<Window>
    {
    public:
        bool Init();

        GLFWwindow* GetWindow();

        GLFWwindow* window;

        void processInput();

    public:
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    private:
        Window();
        ~Window();
        friend class Singleton<Window>;
    };
}

