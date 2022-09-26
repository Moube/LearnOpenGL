#include "FragmentShader.h"
#include "Util/Log.h"

namespace Core
{
	FragmentShader::FragmentShader(std::string filePath)
		: Shader(filePath)
	{
		__id = glCreateShader(GL_FRAGMENT_SHADER);
		const char* sourceStr = source.c_str();
		glShaderSource(__id, 1, &sourceStr, NULL);
		glCompileShader(__id);

		int success;
		char log[512];
		glGetShaderiv(__id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(__id, 512, NULL, log);
			Core::Print("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", log);
		}
	}

}