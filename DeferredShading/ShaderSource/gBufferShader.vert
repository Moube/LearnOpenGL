#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal; 
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    vec4 wolrdPos = model * vec4(position, 1.0f);
    gl_Position = projection * view * wolrdPos;
    vs_out.FragPos = wolrdPos.xyz;
    vs_out.TexCoords = texCoords;
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vs_out.Normal = normalMatrix * normal;
}