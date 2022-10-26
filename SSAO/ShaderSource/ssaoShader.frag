#version 330 core
out float FragColor;
in vec2 TexCoords;

uniform sampler2D gPositionDepth;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform mat4 projection;
uniform vec3 samples[64];

//把texCoord从[0,1]置换到适合从texNoise上采样的区间
const vec2 noiseScale = vec2(800.0 / 4.0, 600.0 / 4.0);
const int kernelSize = 64;
const float radius = 0.5;
const float bias = 0.025;

void main()
{
    vec3 fragPos = texture(gPositionDepth, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; i++)
    {
        vec3 sample = TBN * samples[i];//从切线空间变换到观察空间
        sample = fragPos + sample * radius;

        vec4 offset = vec4(sample, 1.0);
        offset = projection * offset;//观察空间→裁剪空间
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;//[-1,1]→[0,1]

        //float sampleDepth = texture(gPositionDepth, offset.xy).w;//踩坑, 因为此时已经是观察空间, 深度分量是z
        float sampleDepth = texture(gPositionDepth, offset.xy).z;//采样深度值

        //疑惑点
        //1.为什么rangeCheck使用fragPos.z ?
        //2.为什么计算occlusion时使用sample.z ?
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));//范围检测
        //float rangeCheck = smoothstep(0.0, 1.0, radius / abs(sample.z - sampleDepth));
        //occlusion += (sampleDepth >= fragPos.z ? 1.0 : 0.0) * rangeCheck;
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;    
    }

    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = occlusion;
}