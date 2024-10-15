#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <map>

#include "Singleton/Singleton.h"

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

        Character getCharacter(GLchar c);

    private:
        std::map<GLchar, Character> CharacterMap;

    private:
        TextManager() {};
        ~TextManager() {};
        TextManager(const TextManager&) = delete;
        TextManager& operator=(const TextManager&) = delete;
        TextManager(TextManager&& other) noexcept = default;
        TextManager& operator=(TextManager&& other) noexcept = default;
        friend class Singleton<TextManager>;

	};
}