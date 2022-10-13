#include "ShaderProgram.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "GeometryShader.h"
#include "Util/Log.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Core
{
	ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader)
	{
		VertexShader vertex{ vertexShader };
		FragmentShader fragment{ fragmentShader };

		id = glCreateProgram();
		glAttachShader(getID(), vertex.getID());
		glAttachShader(getID(), fragment.getID());
		glLinkProgram(getID());

		int success;
		char log[512];
		glGetProgramiv(getID(), GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(getID(), 512, NULL, log);
			Core::Print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n", log);
		}
	}

	ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader, std::string geometryShader)
	{
		VertexShader vertex{ vertexShader };
		FragmentShader fragment{ fragmentShader };
		GeometryShader geometry{ geometryShader };

		id = glCreateProgram();
		glAttachShader(getID(), vertex.getID());
		glAttachShader(getID(), fragment.getID());
		glAttachShader(getID(), geometry.getID());
		glLinkProgram(getID());

		int success;
		char log[512];
		glGetProgramiv(getID(), GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(getID(), 512, NULL, log);
			Core::Print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n", log);
		}
	}

	ShaderProgram::~ShaderProgram()
	{
		if (getID() != 0)
		{
			glDeleteProgram(getID());
		}
	}

	void ShaderProgram::use() const
	{
		glUseProgram(getID());
	}

	void ShaderProgram::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(getID(), name.c_str()), (int)value);
	}

	void ShaderProgram::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(getID(), name.c_str()), value);
	}

	void ShaderProgram::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(getID(), name.c_str()), value);
	}

	void ShaderProgram::set2Float(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(getID(), name.c_str()), x, y);
	}

	void ShaderProgram::set3Float(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(getID(), name.c_str()), x, y, z);
	}

	void ShaderProgram::set4Float(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(getID(), name.c_str()), x, y, z, w);
	}

	void ShaderProgram::setVec3(const std::string& name, glm::vec3 vec) const
	{
		set3Float(name, vec.x, vec.y, vec.z);
	}

	void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat4) const
	{
		glUniformMatrix4fv(glGetUniformLocation(getID(), name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
	}
}
