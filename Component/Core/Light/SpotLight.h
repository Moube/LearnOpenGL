#pragma once
#include "Light.h"
#include "Object/SceneObject.h"

namespace Core
{
    class SpotLight : public SceneObject, public Light
    {
    public:
		SpotLight(glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 _direction = glm::vec3(1.0f),
            float _cutoff = 12.5f,
            float _outerCutoff = 17.5f,
			glm::vec3 _ambient = glm::vec3(1.0f),
			glm::vec3 _diffuse = glm::vec3(1.0f),
			glm::vec3 _specular = glm::vec3(1.0f));

		SpotLight(glm::vec3 position,
			glm::vec3 _direction,
			float _cutoff,
			float _outerCutoff,
			Core::Light light);
    public:
        glm::vec3 GetDirection() { return direction; }
        float GetCutoff() { return cutoff; }
        float GetOuterCutoff() { return outerCutoff; }
        
    private:
        glm::vec3 direction;
        float cutoff;
        float outerCutoff;

    private:
        //SpotLight(const SpotLight&) = delete;
        //SpotLight& operator=(const SpotLight&) = delete;
        //SpotLight(SpotLight&& other) noexcept = default;
        //SpotLight& operator=(SpotLight&& other) noexcept = default;
    };
}

