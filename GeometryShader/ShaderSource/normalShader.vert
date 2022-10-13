#version 330 core
layout (location = 0) in vec3 aPos; //位置坐标
layout (location = 1) in vec3 aNormal; //法向量

uniform mat4 model;//模型矩阵
uniform mat4 view;//观察矩阵
//uniform mat4 projection;//投影矩阵

out VS_OUT
{
    vec3 normal;
} vs_out;

void main()
{
    //gl_Position = projection * view * model * vec4(aPos.xyz,  1.0);
    gl_Position = view * model * vec4(aPos.xyz,  1.0);
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}