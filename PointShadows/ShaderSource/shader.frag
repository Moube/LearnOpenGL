#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube shadowMap;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform float far_plane;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

float ShadowCalculation(vec3 fragPos)
{
    //
    vec3 fragToLight = fragPos - lightPos;
    //float closestDepth = texture(shadowMap, fragToLight).r;

    //从[0,1]映射到[0, far_plane]
    //closestDepth *= far_plane;

    //当前深度
    float currentDepth = length(fragToLight);

    //阴影偏移
    //float bais = 0.05;
    //float shadow = currentDepth - bais > closestDepth ? 1 : 0;

    //PCF采样
    /*
    float shadow = 0.0;
    float bais = 0.05;
    float samples = 4.0;
    float offset = 0.1;
    for(float x = -offset; x <= offset; x += offset / (samples * 0.5))
    {
        for(float y = -offset; y <= offset; y += offset / (samples * 0.5))
        {
            for(float z = -offset; z <= offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(shadowMap, fragToLight + vec3(x,y,z)).r;
                closestDepth *= far_plane;
                shadow += currentDepth - bais > closestDepth ? 1.0 : 0.0;
            }
        }
    }
    shadow /= (samples * samples * samples);
    */

    //优化过的采样
    float shadow = 0.0;
    float bais = 0.15;
    int samples = 20;
    float viewDistance = length(viewPos - fragPos);
    //float diskRadius = 0.05;
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;
    for(int i = 0; i < samples; i++)
    {
        float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;
        shadow += currentDepth - bais > closestDepth ? 1.0 : 0.0;
    }
    shadow /= samples;

    return shadow;
}

void main()
{
    vec3 tex = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);

    //环境光
    vec3 ambient = 0.3 * tex;

    //漫反射
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = lightColor * diff;

    //镜面反射 Blinn-Phong模型
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halaway = normalize(lightDir + viewDir);//从片段指向光源 + 从片段指向观察者
    float spec = pow(max(dot(halaway, normal), 0.0), 64);
    vec3 specular = lightColor * spec;

    //计算阴影
    float shadow = ShadowCalculation(fs_in.FragPos);
    //float closestDepth = ShadowCalculation(fs_in.FragPos);
    //vec3 result = (ambient +  diffuse + specular) * tex;
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * tex;
    //vec3 result = vec3(closestDepth / far_plane);

    FragColor = vec4(result, 1.0);
}