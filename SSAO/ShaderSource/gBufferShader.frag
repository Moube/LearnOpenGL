#version 330 core
layout (location = 0) out vec4 gPositionDepth;//vec3 → vec4
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

const float NEAR = 0.1f;
const float FAR = 50.0f;
float LinearizeDepth(float depth)
{
    float z = depth * 2 - 1.0;//[0,1]→[-1,1]
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
}

void main()
{
    gPositionDepth.xyz = fs_in.FragPos;
    gPositionDepth.w = LinearizeDepth(gl_FragCoord.z);
    gNormal = normalize(fs_in.Normal);
    gAlbedoSpec.rgb = vec3(0.95);
    gAlbedoSpec.a = 0.0;
}