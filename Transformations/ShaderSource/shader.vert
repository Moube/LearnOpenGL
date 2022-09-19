#version 330 core
layout (location = 0) in vec3 aPos; //位置坐标的属性位置为0
layout (location = 1) in vec3 aColor;//颜色变量的属性位置为1
layout (location = 2) in vec2 aTexCoord;//纹理坐标的属性位置为2

out vec3 ourColor;
out vec2 texCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos.xyz,  1.0);
    ourColor = aColor;
    texCoord = aTexCoord;
}