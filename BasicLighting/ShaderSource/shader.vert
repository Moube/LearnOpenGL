#version 330 core
layout (location = 0) in vec3 aPos; //位置坐标
layout (location = 1) in vec3 aNormal; //法向量

uniform mat4 model;//模型矩阵
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵
//uniform vec3 lightPos;//光源位置 练习3.

out vec3 FragPos;
out vec3 Normal;
//out vec3 LightPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.xyz,  1.0);
    FragPos = vec3(model * vec4(aPos.xyz,  1.0));
    Normal = aNormal;

    //法线矩阵
    //1.求逆在着色器中的性能消耗较大, 尽量避免在着色器中求逆
    //2.当模型矩阵带有不等比缩放时才需要法线矩阵
    //Normal = mat3(transpose(inverse(model))) * aNormal;

    //练习3.在观察空间（而不是世界空间）中计算冯氏光照
    //FragPos = vec3(view * model * vec4(aPos.xyz,  1.0));
    //Normal = vec3(view * vec4(aNormal.xyz, 1.0));
    //Normal = mat3(transpose(inverse(view))) * aNormal;
    //Normal = vec3(transpose(view) * vec4(aNormal.xyz, 1.0));
    //Normal = vec3(inverse(view) * vec4(aNormal.xyz, 1.0));
    //没有实现出来, 参考答案
    //Normal = mat3(transpose(inverse(view * model))) * aNormal;
    //LightPos = vec3(view * vec4(lightPos, 1.0)); // Transform world-space light position to view-space light position
}