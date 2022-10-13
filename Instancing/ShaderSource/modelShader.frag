#version 330 core
//材质
struct Material
{
    sampler2D texture_ambient1;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_reflect1;
};
uniform Material material;

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    vec3 diff = vec3(texture(material.texture_diffuse1, TexCoords));
    FragColor = vec4(diff, 1.0);
}