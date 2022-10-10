#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main()
{
    //FragColor = texture(screenTexture, TexCoords);

    //Inversion 反相
    //FragColor = vec4(vec3(1 - texture(screenTexture, TexCoords)), 1.0);

    //Grayscale 灰度化
    //FragColor = texture(screenTexture, TexCoords);
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;//简单平均
    //float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;//灰度加权平均
    //FragColor = vec4(average, average, average, 1.0);

    //Kernel 核
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset),//左上
        vec2(0.0f,    offset),//上
        vec2(offset,  offset),//右上
        vec2(-offset, 0.0f),  //左  
        vec2(0.0f,    0.0f),  //中  
        vec2(offset,  0.0f),  //右  
        vec2(-offset, -offset),//左下
        vec2(0.0f,    -offset),//下
        vec2(offset,  -offset)//右下
    );

    //Sharpen 锐化
    /*
    float kernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );*/

    //Blur 模糊
    /*
    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );*/

    //Edge-detection 边缘检测
    float kernel[9] = float[](
        1.0, 1.0, 1.0,
        1.0,-8.0, 1.0,
        1.0, 1.0, 1.0  
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);

}