#version 330 core
//材质
struct Material
{
    sampler2D texture_ambient1;
    sampler2D texture_ambient2;
    sampler2D texture_ambient3; 
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
    sampler2D texture_reflect1;
    sampler2D texture_reflect2;
    sampler2D texture_reflect3;
};
uniform Material material;

in vec3 FragPos;//片段世界坐标
in vec3 Normal;//法向量
in vec2 TexCoords;//纹理坐标

uniform vec3 viewPos;//观察者位置
uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(FragPos - viewPos);

    //reflect
    vec3 reflectDir = reflect(viewDir, norm);
    vec3 ref = vec3(texture(skybox, reflectDir).rgb);
    //在c++欺骗了assimp, 将refl贴图加载成amb贴图
    vec3 refTex = vec3(texture(material.texture_ambient1, TexCoords));
    vec3 refl = ref * refTex;

    //diffuse
    vec3 diff = vec3(texture(material.texture_diffuse1, TexCoords));

    FragColor = vec4(refl + diff, 1.0);
}