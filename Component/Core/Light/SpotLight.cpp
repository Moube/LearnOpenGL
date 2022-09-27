#include "SpotLight.h"

namespace Core
{

	SpotLight::SpotLight(glm::vec3 position,
		glm::vec3 _direction,
		float _cutoff,
		float _outerCutoff,
		glm::vec3 _ambient,
		glm::vec3 _diffuse,
		glm::vec3 _specular) :
		SceneObject(position), Light(_ambient, _diffuse, _specular),
		direction(_direction), cutoff(_cutoff), outerCutoff(_outerCutoff)
	{
		
	}

	SpotLight::SpotLight(glm::vec3 position, glm::vec3 _direction, float _cutoff, float _outerCutoff, Core::Light light) :
		SceneObject(position), Light(light.GetAmbient(), light.GetDiffuse(), light.GetSpecular()),
		direction(_direction), cutoff(_cutoff), outerCutoff(_outerCutoff)
	{

	}

}