#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoord;
    WorldPos = vec3(model * vec4(aPosition, 1.0));
    Normal = mat3(model) * aNormal;

    gl_Position = projection * view * vec4(WorldPos, 1.0);
}
