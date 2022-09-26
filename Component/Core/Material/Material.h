#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Texture/Texture2D.h"

namespace Core
{
	class Material
	{
	public:
		Material(glm::vec3 _ambient = glm::vec3(1.0f),
			glm::vec3 _diffuse = glm::vec3(1.0f),
			glm::vec3 _specular = glm::vec3(1.0f),
			float _shininess = 16);

		Material(std::string diffusePath,
			std::string specularPath,
			float _shininess);

	public:
		glm::vec3 GetAmbient() { return ambient; }
		glm::vec3 GetDiffuse() { return diffuse; }
		glm::vec3 GetSpecular() { return specular; }
		float GetShininess() { return shininess; }

		void SetAmbient(glm::vec3 _ambient) { ambient = _ambient; }
		void SetDiffuse(glm::vec3 _diffuse) { diffuse = _diffuse; }
		void SetSpecular(glm::vec3 _specular) { specular = _specular; }
		void SetShininess(float _shininess) { shininess = _shininess; }

	private:
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;

	public:
		Texture2D* GetDiffuseMap() { return diffuseMap.get(); }
		Texture2D* GetSpecularMap() { return specularMap.get(); }

	private:
		std::shared_ptr<Texture2D> diffuseMap;
		std::shared_ptr<Texture2D> specularMap;

	private:
		Material(const Material&) = delete;
		Material& operator=(const Material&) = delete;
		Material(Material&& other) noexcept = default;
		Material& operator=(Material&& other) noexcept = default;
	};
}