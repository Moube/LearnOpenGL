#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Core
{
	class Shader
	{
	public:
		explicit Shader(std::string filePath);

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept = default;
		Shader& operator=(Shader&& other) noexcept = default;

		~Shader();

		constexpr unsigned int getID() const { return __id; }

	protected:
		unsigned int __id;
		std::string source;
	};
}