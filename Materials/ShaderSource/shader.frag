#version 330 core
struct Material
{
    //与上一章使用float类型的ambientStrength与specularStrength不同
    //现在我们使用vec3可以控制材质的每一个颜色通道
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
    //上一章中的三种光照使用同一个lightColor
    //现在我们同样希望可以分别控制它们
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

in vec3 FragPos;//片段世界坐标
in vec3 Normal;//法向量

out vec4 FragColor;

uniform vec3 viewPos;//观察者位置

void main()
{
    //环境光
    vec3 ambient = light.ambient * material.ambient;

    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    //镜面反射
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec= pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}