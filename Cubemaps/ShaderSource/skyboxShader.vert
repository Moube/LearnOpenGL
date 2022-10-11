#version 330 core
layout (location = 0) in vec3 aPos; //位置坐标

uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

out vec3 TexCoords;//类型是vec3

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos,  1.0);
    gl_Position = pos.xyww;//欺骗深度缓冲, 让它以为天空盒深度永远为1
}