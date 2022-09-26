#include "PointLight.h"
#include "Util/Math.h"
namespace Core
{

	PointLight::PointLight(glm::vec3 position, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular):
		SceneObject(position), Light(_ambient, _diffuse, _specular)
	{

	}

}