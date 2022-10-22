#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{
    vec3 result = vec3(1.0);
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    //混合
    hdrColor += bloomColor; 

    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    result = mapped;

    //Gamma校正
    result = pow(result, vec3(1.0 / gamma));
    
    FragColor = vec4(result, 1.0);
}