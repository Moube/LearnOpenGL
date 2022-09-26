#version 330 core
struct Material
{ 
    //移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以我们不需要将它们分开储存
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

in vec3 FragPos;//片段世界坐标
in vec3 Normal;//法向量
in vec2 TexCoords;//纹理坐标

out vec4 FragColor;

uniform vec3 viewPos;//观察者位置

void main()
{
    //纹理
    vec3 tex = vec3(texture(material.diffuse, TexCoords));
    vec3 specularTex = vec3(texture(material.specular, TexCoords));

    //环境光
    vec3 ambient = light.ambient * tex;

    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * tex;

    //镜面反射
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec= pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specularTex;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}