#include "ShaderProgram.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "Util/Log.h"
#include <glad/glad.h>

ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader)
{
	VertexShader vertex{ vertexShader };
	FragmentShader Fragment{ fragmentShader };

	__id = glCreateProgram();
	glAttachShader(__id, vertex.getID());
	glAttachShader(__id, Fragment.getID());
	glLinkProgram(__id);

	int success;
	char log[512];
	glGetProgramiv(__id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(__id, 512, NULL, log);
		Core::Print("ERROR::SHADER::PROGRAM::LINKING_FAILED\n", log);
	}
}

ShaderProgram::~ShaderProgram()
{
	if (__id != 0)
	{
		glDeleteProgram(__id);
	}
}

void ShaderProgram::use() const
{
	glUseProgram(__id);
}

void ShaderProgram::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(__id, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(__id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(__id, name.c_str()), value);
}

void ShaderProgram::set4Float(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(__id, name.c_str()), x, y, z, w);
}
