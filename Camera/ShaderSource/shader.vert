#version 330 core
layout (location = 0) in vec3 aPos; //位置坐标
layout (location = 1) in vec2 aTexCoord;//纹理坐标

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.xyz,  1.0);
    texCoord = aTexCoord;
}