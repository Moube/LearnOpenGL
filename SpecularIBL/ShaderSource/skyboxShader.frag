#version 330 core
out vec4 FragColor;

in vec3 localPos;

uniform samplerCube environmentMap;

void main()
{
    //vec3 color = texture(environmentMap, localPos).rgb;
    vec3 color = textureLod(environmentMap, localPos, 0.0).rgb;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}
