#pragma once

#include <string>
#include <glad/glad.h>
#include "Object/GLObject.h"

namespace Core
{
	class Texture2D : public GLObject
	{
	public:
		Texture2D(std::string path);
		~Texture2D();

		void activate(int unit);
		static void flipVertically();
	private:
		static const unsigned int unit0 = GL_TEXTURE0;
	};
}