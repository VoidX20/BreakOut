#version 330 core
in  vec2  TextureCoords;
out vec4  color;

uniform sampler2D scene;    //渲染好的帧缓冲贴图
uniform vec2      offsets[9];   //存储偏移值的核，用于采样
uniform int       edge_kernel[9];   //锐化核
uniform float     blur_kernel[9];   //模糊核

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main()
{
    color = vec4(0.0f);
    vec3 sample[9];

    if(chaos || shake){ //使用混乱或者震动时，使用核对周围采样，也就是取中心9格的RGB
        for(int i = 0; i < 9; i++){
            sample[i] = vec3(texture(scene, TextureCoords.st + offsets[i]));
        }
    }
    if(chaos)
    {           
        for(int i = 0; i < 9; i++){
            color += vec4(sample[i] * edge_kernel[i], 0.0f);    //将采样得到的纹理乘以核并加到一起
        }
        color.a = 1.0f;
    }
    else if(confuse)
    {
        color = vec4(1.0 - texture(scene, TextureCoords).rgb, 1.0); //反色
    }
    else if(shake)
    {
        for(int i = 0; i < 9; i++){
            color += vec4(sample[i] * blur_kernel[i], 0.0f);
        }
        color.a = 1.0f;
    }
    else
    {
        color =  texture(scene, TextureCoords);
    }
}