#version 330 core
struct Material
{ 
    sampler2D diffuse;
    //sampler2D specular;
    float shininess;
};
uniform Material material;

struct Light
{
    vec3 position;
    //vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

out vec4 FragColor;

uniform vec3 viewPos;//观察者位置

void main()
{
    //纹理
    vec3 tex = vec3(texture(material.diffuse, fs_in.TexCoords));
    //vec3 specularTex = vec3(texture(material.specular, TexCoords));

    //环境光
    //vec3 ambient = light.ambient * tex;

    //漫反射
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * tex;

    //镜面反射 Blinn-Phong模型
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halaway = normalize(lightDir + viewDir);//从片段指向光源 + 从片段指向观察者
    float spec = pow(max(dot(halaway, norm), 0.0), material.shininess);
    //vec3 specular = light.specular * spec * specularTex;
    vec3 specular = light.specular * spec;

    //vec3 result = ambient + diffuse + specular;
    vec3 result = diffuse + specular;
    FragColor = vec4(result, 1.0);
}