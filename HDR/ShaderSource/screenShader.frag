#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float exposure;
uniform bool hdr;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(screenTexture, TexCoords).rgb;
    vec3 result = vec3(1.0);
    if(hdr)
    {
        // Reinhard tone mapping
        //vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

        // Exposure tone mapping
        vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

        result = mapped;
    }
    else
    {
        result = hdrColor;
    }

    //Gamma校正
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}