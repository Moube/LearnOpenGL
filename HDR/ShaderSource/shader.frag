#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light
{
    vec3 Position;
    vec3 Color;
};

uniform Light lights[16];
uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

void main()
{
    vec3 fragPos = fs_in.FragPos;
    vec3 viewDir = normalize(viewPos - fragPos);     
    vec3 normal = normalize(fs_in.Normal);
    vec2 texCoords = fs_in.TexCoords;
    //color
    vec3 color = texture(diffuseTexture, texCoords).rgb;
    //环境光
    vec3 ambient = vec3(0.0) * color;
    //diffuse
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 16; i++)
    {
        Light light = lights[i];
        vec3 lightDir = normalize(light.Position - fragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = light.Color * diff * color;
        float dist = length(fragPos - light.Position);
        vec3 result = diffuse / (dist * dist);
        lighting += result;
    }

    vec3 result = ambient + lighting;
    FragColor = vec4(result, 1.0);
}