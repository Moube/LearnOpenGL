#version 330 core
out vec4 FragColor;

in vec3 ourColor;// 从顶点着色器传来的输入变量（名称相同、类型相同）
in vec2 texCoord;
//in vec2 texCoord2;//练习2
uniform sampler2D texture1; //2D采样器, 可将纹理赋给该uniform
//在代码中没有赋值sampler2D依旧成功画出了纹理, 这是为什么? 
//glBindTexture会自动吧纹理赋值给片段着色器的采样器
uniform sampler2D texture2;

void main()
{
    //使用texture()来采样纹理颜色
    //参数1是采样器
    //参数2是纹理坐标
    //输出为纹理的（插值）纹理坐标上的(过滤后的)颜色。
    FragColor = texture(texture1, texCoord);
    //FragColor = texture(texture1, texCoord) * vec4(ourColor, 1.0);
    //FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);

    //练习1.修改片段着色器，仅让笑脸图案朝另一个方向看
    //vec2 texCoord2 = vec2(1-texCoord.x, texCoord.y);
    //FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord2), 0.2);

    //练习2
    //FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord2), 0.2);
}