#version 330 core
layout (location = 0) in vec3 aPos; //位置变量的属性位置为0
layout (location = 1) in vec3 aColor;//颜色变量的属性位置为1
out vec3 vertexColor; //为片段着色器指定一个颜色输出

uniform float offset;//练习2.使用uniform定义一个水平偏移量，在顶点着色器中使用这个偏移量把三角形移动到屏幕右侧
out vec4 position;//练习3.使用out关键字把顶点位置输出到片段着色器，并将片段的颜色设置为与顶点位置相等（来看看连顶点位置值都在三角形中被插值的结果）
void main()
{
    //course
    gl_Position = vec4(aPos.xyz,  1.0);

    //练习1.修改顶点着色器让三角形上下颠倒
    //gl_Position = vec4(aPos.x, -aPos.y, aPos.z,  1.0);

    //2.
    //gl_Position = vec4(aPos.x + offset, aPos.yz,  1.0);

    vertexColor = aColor;   //将vertexColor设置为从顶点数据那里得到的输入颜色

    //3.
    position = gl_Position;
}