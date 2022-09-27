#version 330 core
//材质
struct Material
{ 
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
uniform Material material;

//点光源
struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHT 4
uniform PointLight pointLights[NR_POINT_LIGHT];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//定向光
struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

//聚光
struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec3 FragPos;//片段世界坐标
in vec3 Normal;//法向量
in vec2 TexCoords;//纹理坐标

uniform vec3 viewPos;//观察者位置

out vec4 FragColor;

void main()
{
    vec3 result = vec3(0.0);
    vec3 viewDir = normalize(viewPos - FragPos);//从片段指向相机

    result += CalcDirLight(dirLight, Normal, viewDir);
    for(int i = 0; i < NR_POINT_LIGHT; i++)
        result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir);
    result += CalcSpotLight(spotLight, Normal, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcLight(vec3 amb, vec3 dif, vec3 spe, vec3 normal, vec3 lightDir, vec3 viewDir)
{
    normal = normalize(normal);
    lightDir = normalize(lightDir);
    viewDir = normalize(viewDir);

    vec3 ambient = amb * vec3(texture(material.diffuse, TexCoords));

    float diff = max(dot(lightDir, normal), 0.0) ;
    vec3 diffuse = dif * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = spe * spec * vec3(texture(material.specular, TexCoords));

    return ambient + diffuse + specular;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(normal);
    vec3 lightDir = -normalize(light.direction);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDirection = normalize(viewDir);
    float spec = pow(max(dot(reflectDir, viewDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec* vec3(texture(material.specular, TexCoords));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);//从片段指向光源
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDirection = normalize(viewDir);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //点光源衰减
    float dist = length(fragPos - light.position);
    float attenuation = 1.0 / 
    (
        light.constant + 
        light.linear * dist +
        light.quadratic * dist * dist
    );

    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);//从片段指向光源
    
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDirection = normalize(viewDir);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(reflectDir, viewDirection), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    //聚光及柔化边缘
    vec3 direction = normalize(light.direction);
    float theta = dot(-lightDir, direction);
    float epsilon = light.cutOff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    return ambient + (diffuse + specular) * intensity;
}