#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include "Object/GLObject.h"

namespace Core
{
	class CubeMap : public GLObject
	{
	public:
		CubeMap(std::string directory);
		CubeMap(std::vector<std::string> faces);
		
		void activate(int unit);
	};
}