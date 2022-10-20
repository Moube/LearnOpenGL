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
uniform sampler2D depthMap;
uniform float height_scale;
uniform bool parallax;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
    //TangentPos
    vec3 lightPos = fs_in.TangentLightPos;
    vec3 viewPos = fs_in.TangentViewPos;
    vec3 fragPos = fs_in.TangentFragPos;

    //dir
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 lightDir = normalize(lightPos - fragPos);

    //parallax
    vec2 texCoords = fs_in.TexCoords;
    if(parallax)
    {
        texCoords = ParallaxMapping(fs_in.TexCoords, viewDir);
        if(texCoords.x > 1.0 || texCoords.x < 0.0 || texCoords.y > 1.0 || texCoords.y < 0.0)
            discard;
    }
        
    //normal
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    //color
    vec3 color = texture(diffuseTexture, texCoords).rgb;
    vec3 diffuseLight = vec3(1.0);
    vec3 specularLight = vec3(0.2);
    vec3 ambientLight = vec3(0.1);
    
    
    //环境光
    vec3 ambient = ambientLight * color;

    //漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * color;

    //镜面反射 Blinn-Phong模型
    vec3 halaway = normalize(lightDir + viewDir);//从片段指向光源 + 从片段指向观察者
    float spec = pow(max(dot(halaway, normal), 0.0), 32);
    vec3 specular = specularLight * spec;

    vec3 result = ambient + diffuse + specular;
    //vec3 result = ambient;
    FragColor = vec4(result, 1.0);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    //steep parallax mapping
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / 10;
    float currentLayerDepth = 0.0;

    vec2 p = viewDir.xy * height_scale;
    vec2 deltaTexCoords = p / numLayers;//每一层采样位移距离

    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    //parallax occlusion mapping
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    float prevDepthMapValue = texture(depthMap, prevTexCoords).r;
    float prevLayerDepth = currentLayerDepth - layerDepth;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = prevDepthMapValue - prevLayerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    //float weight = afterDepth / (afterDepth + beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
    //return currentTexCoords;

    //parallax mapping
    //float height = texture(depthMap, texCoords).r;
    //vec2 p = viewDir.xy / viewDir.z * (height * height_scale);

    //↓教程里steep parallax示例代码用这个返回值, 是错误的
    //return texCoords - currentTexCoords;
    
}