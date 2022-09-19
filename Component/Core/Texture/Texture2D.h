#pragma once

#include <string>
#include <glad/glad.h>

namespace Core
{
	class Texture2D
	{
	public:
		Texture2D(std::string path, bool rgba = false);
		~Texture2D();

		void activate(int unit);
		unsigned int getID() const { return __id; }

		static void flipVertically();
	private:
		unsigned int __id;
		static const unsigned int unit0 = GL_TEXTURE0;
	};
}