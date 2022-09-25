//练习4.尝试实现一个Gouraud着色（而不是冯氏着色）。如果你做对了话，立方体的光照应该会看起来有些奇怪，尝试推理为什么它会看起来这么奇怪
#version 330 core
layout (location = 0) in vec3 aPos; //位置坐标
layout (location = 1) in vec3 aNormal; //法向量

uniform mat4 model;//模型矩阵
uniform mat4 view;//观察矩阵
uniform mat4 projection;//投影矩阵

uniform vec3 objectColor;//物体自身颜色
uniform vec3 lightColor;//光源颜色
uniform vec3 lightPos;//光源位置
uniform vec3 viewPos;//观察者位置

uniform float _ambientStrength;
uniform float _specularStrength;

out vec4 resultColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.xyz,  1.0);
    vec3 FragPos = vec3(model * vec4(aPos.xyz,  1.0));
    vec3 Normal = aNormal;

     //环境光照
    vec3 ambient = _ambientStrength * lightColor;//练习2

    //漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);//从片段坐标指向光源坐标 练习3.需注释掉
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //镜面光照
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//参数1需求的是【从光源指向片段的坐标】
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);//取32次幂, 高光的反光度(Shininess)
    vec3 specular = _specularStrength * spec * lightColor;//练习2

    vec3 result = (ambient + diffuse + specular) * objectColor;

    resultColor = vec4(result, 1.0);
}