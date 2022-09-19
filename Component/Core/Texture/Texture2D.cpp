#include "Texture2D.h"
#include <stb_image.h>
#include "Util/Log.h"

Core::Texture2D::Texture2D(std::string path, bool rgba)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);//·��, ���, �߶�, ��ɫͨ����
	if (!data)
	{
		Core::Print("Failed to load texture");
		return;
	}

	//�����������
	glGenTextures(1, &__id);
	glBindTexture(GL_TEXTURE_2D, getID());//���������

	//��������
	if (rgba)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);//������������ͷ�ͼƬ��Դ
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
