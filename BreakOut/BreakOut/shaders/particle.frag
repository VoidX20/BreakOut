#version 330 core

in vec2 TextureCoords;  //UV坐标
in vec4 ParticleColor;  //粒子颜色
out vec4 color;         //最终输出颜色

uniform sampler2D sprite;   //贴图

void main()
{
    color = (texture(sprite, TextureCoords) * ParticleColor);
}