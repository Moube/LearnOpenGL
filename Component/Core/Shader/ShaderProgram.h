#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Object/GLObject.h"

namespace Core
{
	class ShaderProgram : public GLObject
	{
	public:
		ShaderProgram(std::string vertexShader, std::string fragmentShader);
		ShaderProgram(std::string vertexShader, std::string fragmentShader, std::string geometryShader);

		~ShaderProgram();

		void use() const;

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void set2Float(const std::string& name, float x, float y) const;
		void set3Float(const std::string& name, float x, float y, float z) const;
		void set4Float(const std::string& name, float x, float y, float z, float w) const;

		void setVec2(const std::string& name, glm::vec2 vec) const;
		void setVec3(const std::string& name, glm::vec3 vec) const;
		void setMat4(const std::string& name, const glm::mat4& mat4) const;

	private:
	};
}