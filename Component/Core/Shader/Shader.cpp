#include "Shader.h"
#include "Util/Log.h"

Shader::Shader(std::string filePath)
	: __id(0)
{
	std::ifstream fs{};
	fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		fs.open(filePath);
		std::stringstream ss{};
		ss << fs.rdbuf();
		fs.close();
		source = ss.str();
	}
	catch (std::ifstream::failure e)
	{
		Core::Print("Error::Shader::FileNotSuccessfullyRead");
	}
}

Shader::~Shader()
{
	if (__id != 0)
		glDeleteShader(__id);
}
