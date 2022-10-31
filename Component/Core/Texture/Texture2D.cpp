#include "Texture2D.h"
#include <stb_image.h>
#include "Util/Log.h"
#include "Asset/Asset.h"

Core::Texture2D::Texture2D(std::string path)
{
	sourcePath = Asset::Path(path);
	if (getExtension(sourcePath) == ".hdr")
	{
		loadHDR();
	}
	else
	{
		loadNormal();
	}
	
}

Core::Texture2D::~Texture2D()
{

}

void Core::Texture2D::loadNormal()
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(sourcePath.c_str(), &width, &height, &nrChannels, 0);//·��, ���, �߶�, ��ɫͨ����
	if (!data)
	{
		Core::Print("Failed to load texture");
		return;
	}

	//�����������
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, getID());//���������

	//��������
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
	stbi_image_free(data);//������������ͷ�ͼƬ��Դ
}

void Core::Texture2D::loadHDR()
{
	int width, height, nrChannels;
	float* data = stbi_loadf(sourcePath.c_str(), &width, &height, &nrChannels, 0);//·��, ���, �߶�, ��ɫͨ����
	if (!data)
	{
		Core::Print("Failed to load texture");
		return;
	}

	//�����������
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, getID());//���������

	//��������
	GLenum format = GL_RGB16F;
	GLenum innerFormat = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, innerFormat, GL_FLOAT, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);//������������ͷ�ͼƬ��Դ
}

void Core::Texture2D::activate(int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, getID());
}

void Core::Texture2D::flipVertically()
{
	stbi_set_flip_vertically_on_load(true);
}

std::string Core::Texture2D::getExtension(std::string s)
{
	int offset =  s.find_last_of(".");
	std::string extension = s.substr(offset);
	return extension;
}
