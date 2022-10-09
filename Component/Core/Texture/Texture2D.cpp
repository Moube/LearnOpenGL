#include "Texture2D.h"
#include <stb_image.h>
#include "Util/Log.h"
#include "Asset/Asset.h"

Core::Texture2D::Texture2D(std::string path)
{
	std::string sourcePath = Asset::Path(path);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(sourcePath.c_str(), &width, &height, &nrChannels, 0);//路径, 宽度, 高度, 颜色通道数
	if (!data)
	{
		Core::Print("Failed to load texture");
		return;
	}

	//创建纹理对象
	glGenTextures(1, &__id);
	glBindTexture(GL_TEXTURE_2D, getID());//绑定纹理对象

	//生成纹理
	GLenum format;
	switch (nrChannels)
	{
	case 1:
		format = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		break;
	default:
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);//创建完纹理后释放图片资源
}

Core::Texture2D::~Texture2D()
{

}

void Core::Texture2D::activate(int unit)
{
	glActiveTexture(unit0 + unit);
	glBindTexture(GL_TEXTURE_2D, getID());
}

void Core::Texture2D::flipVertically()
{
	stbi_set_flip_vertically_on_load(true);
}
