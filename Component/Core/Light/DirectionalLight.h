#pragma once
#include "Light.h"

namespace Core
{
    class DirectionalLight : public Light
    {
    public:
        DirectionalLight(glm::vec3 _direction = glm::vec3(0.0f),
            glm::vec3 _ambient = glm::vec3(1.0f),
            glm::vec3 _diffuse = glm::vec3(1.0f),
            glm::vec3 _specular = glm::vec3(1.0f));

    public:
        glm::vec3 GetDirection() { return direction; }

    public:
        glm::vec3 direction;
       
    private:
        DirectionalLight(const DirectionalLight&) = delete;
        DirectionalLight& operator=(const DirectionalLight&) = delete;
        DirectionalLight(DirectionalLight&& other) noexcept = default;
        DirectionalLight& operator=(DirectionalLight&& other) noexcept = default;
    };
}

