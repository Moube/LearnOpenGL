#include "DirectionalLight.h"
#include "Util/Math.h"
namespace Core
{

	DirectionalLight::DirectionalLight(glm::vec3 _direction, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular):
		direction(_direction), Light(_ambient, _diffuse, _specular)
	{

	}

}