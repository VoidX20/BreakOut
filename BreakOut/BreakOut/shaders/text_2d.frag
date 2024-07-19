#version 330 core
in vec2 TextureCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TextureCoords).r); //字体解码得到的是单通道灰度图，因此选择作为透明度使用，颜色另外传进来
    color = vec4(textColor, 1.0) * sampled;
}  