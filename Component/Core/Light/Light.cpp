#include "Light.h"
#include "Util/Math.h"
namespace Core
{

	Light::Light(glm::vec3 position, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular):
		SceneObject(position), ambient(_ambient), diffuse(_diffuse), specular(_specular)
	{

	}

}