#pragma once
#include "Light.h"
#include "Object/SceneObject.h"

namespace Core
{
    class PointLight : public SceneObject, public Light
    {
    public:
        PointLight(glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 _ambient = glm::vec3(1.0f),
            glm::vec3 _diffuse = glm::vec3(1.0f),
            glm::vec3 _specular = glm::vec3(1.0f));
       
    private:
        PointLight(const PointLight&) = delete;
        PointLight& operator=(const PointLight&) = delete;
        PointLight(PointLight&& other) noexcept = default;
        PointLight& operator=(PointLight&& other) noexcept = default;
    };
}

