#version 330 core
in vec3 FragPos;//片段世界坐标
in vec3 Normal;//法向量

out vec4 FragColor;

uniform vec3 objectColor;//物体自身颜色
uniform vec3 lightColor;//光源颜色
uniform vec3 lightPos;//光源位置
uniform vec3 viewPos;//观察者位置

void main()
{
    //环境光照
    float ambientStrength = 0.1;//环境光强度
    vec3 ambient = ambientStrength * lightColor;

    //漫反射光照
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);//从片段坐标指向光源坐标
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //镜面光照
    float specularStrength = 0.5;//镜面强度
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);//参数1需求的是【从光源指向片段的坐标】
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);//取32次幂, 高光的反光度(Shininess)
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}