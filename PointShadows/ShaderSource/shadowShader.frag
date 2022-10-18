#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    //计算片段与光源的距离
    float lightDistance = length(FragPos.xyz - lightPos);

    //把距离映射到[0,1]
    lightDistance = lightDistance / far_plane;

    //写入
    gl_FragDepth = lightDistance;
}