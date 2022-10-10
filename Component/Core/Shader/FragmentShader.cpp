#include "FragmentShader.h"
#include "Util/Log.h"

namespace Core
{
	FragmentShader::FragmentShader(std::string filePath)
		: Shader(filePath)
	{
		id = glCreateShader(GL_FRAGMENT_SHADER);
		const char* sourceStr = source.c_str();
		glShaderSource(getID(), 1, &sourceStr, NULL);
		glCompileShader(getID());

		int success;
		char log[512];
		glGetShaderiv(getID(), GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(getID(), 512, NULL, log);
			Core::Print("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", log);
		}
	}

}