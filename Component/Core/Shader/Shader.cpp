#include "Shader.h"
#include "Util/Log.h"

namespace Core
{
	Shader::Shader(std::string filePath)
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
		if (getID() != 0)
			glDeleteShader(getID());
	}
}
