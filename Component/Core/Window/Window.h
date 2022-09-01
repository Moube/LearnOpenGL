#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Core/Singleton/Singleton.h"

namespace Component
{
    class Window : public Singleton<Window>
    {
    public:
        bool Init();

        GLFWwindow* GetWindow();

        GLFWwindow* window;

    public:
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

    private:
        Window();
        ~Window();
    };
}

