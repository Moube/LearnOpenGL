#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

const float PI = 3.14159265359;

uniform vec3 camPos;
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

float DistributionGGX(vec3 N, vec3 H, float _roughness);
float GeometrySchlickGGX(float NdotV, float _roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float _roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float _roughness);

void main()
{
    //---------------
    vec3 N = normalize(Normal);//法线
    vec3 V = normalize(camPos - WorldPos);//视线
    vec3 F0 = vec3(0.04);//基础反射率
    F0 = mix(F0, albedo, metallic);

    float NdotV = max(dot(N, V), 0.0);

    //---------------
 

    //---------------
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; i++)
    {
        //------------Radiance--------------
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);

        float dist = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (dist * dist);
        vec3 radiance = lightColors[i] * attenuation;

        //------------BRDF--------------

        //fresnel
        float HdotV = max(dot(H, V), 0.0);
        vec3 F = fresnelSchlick(HdotV, F0);

        //NDF
        float NDF = DistributionGGX(N, H, roughness);

        //Geometry
        float G = GeometrySmith(N, V, L, roughness);

        float NdotL = max(dot(N, L), 0.0);
        vec3 nominator = NDF * G * F;
        float denominator = 4.0 * NdotV * NdotL + 0.001;//防止除0
        vec3 specular = nominator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;//金属吸收的能量

        //------------Lo--------------
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    //ambient
    //vec3 ambient = vec3(0.03) * albedo * ao;

    //IBL ambient
    vec3 F = fresnelSchlickRoughness(NdotV, F0, roughness);//在fresnel-schlick中加入粗糙度
    vec3 kS = F;
    vec3 kD = 1.0 - kS;

    //漫反射IBL
    vec3 irradiance = texture(irradianceMap, N).rgb;//使用N对漫反射辐照进行采样
    vec3 diffuse = irradiance * albedo;

    //镜面反射IBL
    vec3 R = reflect(-V, N);//使用反射向量对间接镜面反射进行采样
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;//mip采样级别与粗糙度有关
    vec2 envBRDF = texture(brdfLUT, vec2(NdotV), roughness).rg;//使用ndotv与roughness对brdfLUT进行采样
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * ao;//此处specular没有乘ks因为上面已经乘过F了
    vec3 color = ambient + Lo;

    //gamma
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}

/*
* 入射辐射率计算实现
* 对应反射率方程中Li(p, wi)n·wi的部分
vec3 lightColor = vec3(23.47, 21.31, 20.79);
vec3 wi = normalize(lightPos - fragPos);
float cosTheta = max(dot(N, Wi), 0.0);
float attenuation = calculateAttenuation(fragPos, lightPos);

//Li(p, wi) = lightColor * attenuation
//n·wi = cosTheta
float radiance = lightColor * attenuation * cosTheta;
*/

float DistributionGGX(vec3 N, vec3 H, float _roughness)
{
    float a = _roughness * _roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float _roughness)
{
    float r = _roughness + 1.0;
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float _roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);

    float ggx2 = GeometrySchlickGGX(NdotV, _roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, _roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float _roughness)
{
    return F0 + (max(vec3(1.0 - _roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}