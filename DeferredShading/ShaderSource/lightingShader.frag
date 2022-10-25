#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light
{
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
    float Radius;
};

const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{
    //从gbuffer中获得数据
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    vec3 ambient = vec3(0.1f); //写死ambient
    vec3 lighting = Albedo * ambient;
    vec3 viewDir = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; i++)
    {
        Light light = lights[i];
        float dist = length(light.Position - FragPos);
        if(dist > light.Radius)//这不是体积光的正确使用方式, 仅作为学习思路的展示
            continue;//由于GPU的特性, 着色器会运行if语句的所有分支

        vec3 lightDir = normalize(light.Position - FragPos);
        float diff = max(dot(lightDir, Normal), 0.0);
        vec3 diffuse = diff * Albedo * light.Color;

        vec3 halfwarDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(Normal, halfwarDir),0.0), 16.0);
        vec3 specular = spec * Specular * light.Color;

        
        float attenuation = 1.0 / (1.0 + light.Linear * dist + light.Quadratic * dist * dist);
        diffuse *= attenuation;
        specular *= attenuation;

        lighting += diffuse + specular;
    }

    FragColor = vec4(lighting, 1.0);
}