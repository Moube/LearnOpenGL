#version 330 core
out vec4 FragColor;

in vec3 TexCoords;//vec3类型

uniform samplerCube skybox;//samplerCube类型

void main()
{
    FragColor = texture(skybox, TexCoords);
}