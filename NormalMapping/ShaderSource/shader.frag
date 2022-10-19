#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;
uniform bool normalMapping;

void main()
{
    //normal
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    //color
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 diffuseLight = vec3(1.0);
    vec3 specularLight = vec3(0.2);
    vec3 ambientLight = vec3(0.1);
    //TangentPos
    vec3 lightPos = fs_in.TangentLightPos;
    vec3 viewPos = fs_in.TangentViewPos;
    vec3 fragPos = fs_in.TangentFragPos;
    
    //环境光
    vec3 ambient = ambientLight * color;

    //漫反射
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * color;

    //镜面反射 Blinn-Phong模型
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halaway = normalize(lightDir + viewDir);//从片段指向光源 + 从片段指向观察者
    float spec = pow(max(dot(halaway, normal), 0.0), 32);
    vec3 specular = specularLight * spec;

    vec3 result = ambient + diffuse + specular;
    //vec3 result = ambient;
    FragColor = vec4(result, 1.0);
}