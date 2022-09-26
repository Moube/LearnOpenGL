#version 330 core
layout (location = 0) in vec3 aPos; //位置坐标
layout (location = 1) in vec3 aNormal; //法向量
layout (location = 2) in vec2 aTexCoords; //纹理坐标

uniform mat4 model;//模型矩阵
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.xyz,  1.0);
    FragPos = vec3(model * vec4(aPos.xyz,  1.0));
    Normal = aNormal;
    //Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
}