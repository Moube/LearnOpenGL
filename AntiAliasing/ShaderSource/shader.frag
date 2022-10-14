#version 330 core
out vec4 FragColor;

//in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture1;
uniform vec3 viewPos;

void main()
{
    //vec4 texColor = texture(texture1, TexCoords);
    //FragColor = texColor;
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}