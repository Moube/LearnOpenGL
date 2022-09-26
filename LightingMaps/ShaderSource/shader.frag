#version 330 core
struct Material
{ 
    //移除了环境光材质颜色向量，因为环境光颜色在几乎所有情况下都等于漫反射颜色，所以我们不需要将它们分开储存
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;//练习4.
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
    //练习2.尝试在片段着色器中反转镜面光贴图的颜色值，让木头显示镜面高光而钢制边缘不反光（由于钢制边缘中有一些裂缝，边缘仍会显示一些镜面高光，虽然强度会小很多）
    //material.specular = (1 - material.specular);练习2. X 错误!
    vec3 specularTex = vec3(texture(material.specular, TexCoords));
    //vec3 specularTex = vec3(1.0) - vec3(texture(material.specular, TexCoords));//练习2.看了参考答案
    vec3 emissionTex = vec3(texture(material.emission, TexCoords));

    //环境光
    vec3 ambient = light.ambient * tex;

    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * tex;
    //vec3 diffuse = light.diffuse * diff * tex * emissionTex;//练习4. X 错误!

    //镜面反射
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec= pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specularTex;

    //放射光

    //vec3 result = ambient + diffuse + specular;
    vec3 result = ambient + diffuse + specular + emissionTex;//练习4.
    FragColor = vec4(result, 1.0);
}