#include "Material.h"

namespace Core
{
	Material::Material(glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, float _shininess) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess)
	{
		diffuseMap = nullptr;
		specularMap = nullptr;
	}


	Material::Material(std::string diffusePath, std::string specularPath, float _shininess)
		: shininess(_shininess)
	{
		diffuseMap = std::make_shared<Texture2D>(diffusePath);
		specularMap = std::make_shared<Texture2D>(specularPath);
		ambient = glm::vec3(0.0f);
		diffuse = glm::vec3(0.0f);
		specular = glm::vec3(0.0f);
	}

}

