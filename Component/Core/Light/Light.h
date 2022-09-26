#pragma once
#include "glm/glm.hpp"

namespace Core
{
    class Light
    {
    public:
        Light(glm::vec3 _ambient = glm::vec3(1.0f),
            glm::vec3 _diffuse = glm::vec3(1.0f),
            glm::vec3 _specular = glm::vec3(1.0f));

    public:
        glm::vec3 GetAmbient() { return ambient; }
        glm::vec3 GetDiffuse() { return diffuse; }
        glm::vec3 GetSpecular() { return specular; }

        void SetAmbient(glm::vec3 _ambient) { ambient = _ambient; }
        void SetDiffuse(glm::vec3 _diffuse) { diffuse = _diffuse; }
        void SetSpecular(glm::vec3 _specular) { specular = _specular; }

    private:
		glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        
    private:
        Light(const Light&) = delete;
        Light& operator=(const Light&) = delete;
        Light(Light&& other) noexcept = default;
        Light& operator=(Light&& other) noexcept = default;
    };
}

