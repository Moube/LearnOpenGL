#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>

#include "Singleton/Singleton.h"
#include "Shader/ShaderProgram.h"

namespace Core
{
    struct Character {
        GLuint TextureID;       // ��������ID
        glm::ivec2 Size;        // ���δ�С
        glm::ivec2 Bearing;     // �ӻ�׼��������/������ƫ��ֵ
        GLuint Advance;         // ԭ�������һ������ԭ��ľ���
    };

	class TextManager : public Singleton<TextManager>
	{
    public:
        void Init();

        void RenderText(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, std::string font);

    private:
        unsigned int VAO;
        unsigned int VBO;
        ShaderProgram* program;

    private:
        TextManager();
        ~TextManager() {};
        TextManager(const TextManager&) = delete;
        TextManager& operator=(const TextManager&) = delete;
        TextManager(TextManager&& other) noexcept = default;
        TextManager& operator=(TextManager&& other) noexcept = default;
        friend class Singleton<TextManager>;

	};
}