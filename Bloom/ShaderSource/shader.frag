#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

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

uniform Light lights[4];
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
    for(int i = 0; i < 4; i++)
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

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}