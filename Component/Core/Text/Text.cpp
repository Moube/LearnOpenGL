#include "Text.h"
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "Asset/Asset.h"

Core::TextManager::TextManager()
{
    VAO = 0;
    VBO = 0;
    program = nullptr;
}

void Core::TextManager::Init()
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    program = new ShaderProgram("../../Text/ShaderSource/shader.vert", "../../Text/ShaderSource/shader.frag");
}

void Core::TextManager::RenderText(std::wstring text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color, std::string font)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::TextManager: Could not init FreeType Library" << std::endl;
        return;
    }
        
    FT_Face face;
    std::string fontPath = Asset::Path(font);
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
    {
        std::cout << "ERROR::TextManager: Failed to load font" << std::endl;
        return;
    }
        
    FT_Set_Pixel_Sizes(face, 0, 48);

    //必要的opengl选项设置
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    
    program->use();
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    program->setMat4("projection", projection);
    program->set3Float("textColor", color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::wstring::const_iterator iter;
    for (iter = text.begin(); iter != text.end(); iter++)
    {
        //从face中加载字符信息
        if (FT_Load_Char(face, *iter, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::TextManager: Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    //设置纹理选项
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character ch = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        //计算字符尺寸及位置
        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // 对每个字符更新VBO
        GLfloat vertices[6][4] = {
            {xpos,		ypos + h,	0.0, 0.0},
            {xpos,		ypos,		0.0, 1.0},
            {xpos + w,	ypos,		1.0, 1.0},

            {xpos,		ypos + h,	0.0, 0.0},
            {xpos + w,	ypos,		1.0, 1.0},
            {xpos + w,	ypos + h,	1.0, 0.0},
        };

        //在四边形上绘制字形纹理
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        //更新VBO内存的内容
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //绘制四边形
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //更新位置到下一个字形的原点, 单位是1/64像素
        x += (ch.Advance >> 6) * scale;	// 位偏移6个单位来获取单位为像素的值 
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //释放freetype库的资源
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

