#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Object/GLObject.h"

namespace Core
{
	class Shader : public GLObject
	{
	public:
		explicit Shader(std::string filePath);

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept = default;
		Shader& operator=(Shader&& other) noexcept = default;

		~Shader();

	protected:
		std::string source;
	};
}