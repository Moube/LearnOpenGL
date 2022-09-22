#pragma once

#include <functional>
//#include "Singleton/Singleton.h"

namespace Core
{
    using InputExecute = std::function<void()>;
    class InputHandler
    {
    public:
        InputHandler(){}
        InputExecute handler;
    private:
        void Execute();
        void Bind(InputExecute _handler);
        
    private:
        InputHandler(const InputHandler&) = delete;
        InputHandler& operator=(const InputHandler&) = delete;
        InputHandler(InputHandler&& other) noexcept = default;
        InputHandler& operator=(InputHandler&& other) noexcept = default;
        friend class InputManager;
    };
}

