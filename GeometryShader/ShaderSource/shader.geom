#version 330 core
layout (points) in;
//layout (points, max_vertices = 1) out;//Pass-through 最简单的传递几何着色器
layout (triangle_strip, max_vertices = 5) out;//

in VS_OUT
{
    vec3 color;
} gs_in[];//几何着色器中的输入用数组形式

out vec3 fColor;

void build_house(vec4 position)
{
    fColor = gs_in[0].color;
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);//左下
    EmitVertex();
    gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);//右下
    EmitVertex();
    gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);//左上
    EmitVertex();
    gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);//右上
    EmitVertex();
    gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);//顶部
    fColor = vec3(1.0,1.0,1.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    build_house(gl_in[0].gl_Position);
}