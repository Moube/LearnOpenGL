#include "CubeMap.h"
#include <stb_image.h>
#include "Util/Log.h"
#include "Asset/Asset.h"

namespace Core
{
	CubeMap::CubeMap(std::string directory)
	{

	}

	CubeMap::CubeMap(std::vector<std::string> faces)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, getID());

		int width, height, nrChannels;
		for (int i = 0; i < faces.size(); i++)
		{
			std::string sourcePath = Asset::Path(faces[i]);
			unsigned char* data = stbi_load(sourcePath.c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else
			{
				Print("Cubemap texture failed to load at path: ", faces[i]);
			}
			stbi_image_free(data);
		}

		//纹理过滤
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//纹理环绕
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	}

	void CubeMap::activate(int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, getID());
	}
}


