#include "PointLight.h"
#include "Util/Math.h"
namespace Core
{

	PointLight::PointLight(glm::vec3 position, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular):
		SceneObject(position), Light(_ambient, _diffuse, _specular)
	{
		constant = 1.0f;
		linear = 0.0f;
		quadratic = 0.0f;
	}

	void PointLight::SetAttenuation(float _constant, float _linear, float _quadratic)
	{
		constant = _constant;
		linear = _linear;
		quadratic = _quadratic;
	}

}