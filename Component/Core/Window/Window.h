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
        void ProcessInput(float delta);

        void UpdateSize(int width, int height);
        void GetSize(int& width, int& height);
        int GetWidth() { return __width; };
        int GetHeight() { return __height; };

        void PreProcess(float delta);
        void PostProcess(float delta);

    public:
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
        int __width;
        int __height;

    private:
        Window() {};
        ~Window() {};
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&& other) noexcept = default;
        Window& operator=(Window&& other) noexcept = default;
        friend class Singleton<Window>;
    };
}

