#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;//练习1.

void main()
{
    FragColor = vec4(lightColor, 1.0);
}