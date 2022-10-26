#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D ssao;

struct Light
{
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
};
uniform Light light;
uniform vec3 viewPos;

void main()
{
    //从gbuffer中获得数据
    vec3 FragPos = texture(gPositionDepth, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    //float Specular = texture(gAlbedoSpec, TexCoords).a;
    float AmbientOcclusion = texture(ssao, TexCoords).r;

    vec3 ambient = vec3(1.0f) * AmbientOcclusion;
    //ambient = vec3(1.0f);
    vec3 lighting = Albedo * ambient;
    //vec3 viewDir = normalize(viewPos - FragPos);
    vec3 viewDir = normalize(-FragPos);

    float dist = length(light.Position - FragPos);
    vec3 lightDir = normalize(light.Position - FragPos);
    float diff = max(dot(lightDir, Normal), 0.0);
    vec3 diffuse = diff * Albedo * light.Color;

    vec3 halfwarDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(Normal, halfwarDir),0.0), 8.0);
    //vec3 specular = spec * Specular * light.Color;
    vec3 specular = spec * light.Color;

    float attenuation = 1.0 / (1.0 + light.Linear * dist + light.Quadratic * dist * dist);
    diffuse *= attenuation;
    specular *= attenuation;
    lighting += diffuse + specular;

    FragColor = vec4(lighting, 1.0);
}