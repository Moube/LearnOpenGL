#version 330 core
out vec4 FragColor;

//in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

//uniform sampler2D texture1;
uniform samplerCube skybox;
uniform vec3 viewPos;

void main()
{
    vec3 viewDir = normalize(FragPos - viewPos);//从片段指向相机
    vec3 norm = normalize(Normal);
    vec3 reflectDir = reflect(viewDir, norm);//反射, 从片段指向skybox
    float ratio = 1.0 / 1.52; //从空气射入玻璃
    vec3 refractDir = refract(viewDir, norm, ratio);//折射

    //vec4 texColor = texture(texture1, TexCoords);
    //vec4 texColor = vec4(texture(skybox, reflectDir).rgb, 1.0);
    vec4 texColor = vec4(texture(skybox, refractDir).rgb, 1.0);
    FragColor = texColor;
}