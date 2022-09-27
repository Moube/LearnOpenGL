#version 330 core
struct Material
{ 
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    vec3 direction;
    float cutOff;
    float outerCutoff;
};
uniform PointLight pointLight;

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight directionalLight;

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
    vec3 emissionTex = vec3(texture(material.emission, TexCoords));

    //点光源
    float dist = length(pointLight.position - FragPos);
    float attenuation = 1.0 / 
    (pointLight.constant + 
    pointLight.linear * dist +
    pointLight.quadratic * dist * dist);

    vec3 ambient = pointLight.ambient * tex;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * diff * tex;

    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec= pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = pointLight.specular * spec * specularTex;
    
    //vec3 result = (ambient + diffuse + specular) * attenuation;
    //vec3 result = ambient + diffuse + specular;

    //聚光
    vec3 result;
    float theta = dot(lightDir, -normalize(pointLight.direction));
    float epsilon = (pointLight.cutOff - pointLight.outerCutoff);
    float intensity = (theta - pointLight.outerCutoff) / epsilon;
    intensity = clamp(intensity, 0.0, 1.0);

    result = ambient + (diffuse + specular) * intensity;

    /*
    //定向光
    vec3 ambient = directionalLight.ambient * tex;

    vec3 norm = normalize(Normal);
    vec3 lightDir = -normalize(directionalLight.direction);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = directionalLight.diffuse * diff * tex;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDir),0.0), material.shininess);
    vec3 specular = directionalLight.specular * spec * specularTex;
    */

    
    //vec3 result = ambient + diffuse + specular;
    //vec3 result = ambient + diffuse + specular + emissionTex;
    
    FragColor = vec4(result, 1.0);

    
}