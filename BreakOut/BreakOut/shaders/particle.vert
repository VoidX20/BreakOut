#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 TextureCoords>，二合一传进来

out vec2 TextureCoords; //UV坐标，传给片段着色器
out vec4 ParticleColor; //粒子颜色，传给片段着色器

uniform mat4 projection;    //投影矩阵
uniform vec2 offset;    //粒子偏离产生中心的offset
uniform vec4 color;     //粒子颜色

void main()
{
    float scale = 10.0f;    //粒子大小
    TextureCoords = vertex.zw;  //从传入的顶点数据取出UV坐标
    ParticleColor = color;
    gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}