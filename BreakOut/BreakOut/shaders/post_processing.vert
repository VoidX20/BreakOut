#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TextureCoords;

uniform bool  chaos;    //混乱效果，锐化边缘并使得xy随时间移动
uniform bool  confuse;  //迷惑效果，反色并颠倒xy坐标
uniform bool  shake;    //震动效果
uniform float time;     //时间，用于处理随时间变化的情况

void main()
{
    gl_Position = vec4(vertex.xy, 0.0f, 1.0f); 
    vec2 texture = vertex.zw;
    if(chaos)
    {
        float strength = 0.3;   //强度
        vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);        //使得xy坐标随时间移动
        TextureCoords = pos;
    }
    else if(confuse)
    {
        TextureCoords = vec2(1.0 - texture.x, 1.0 - texture.y); //颠倒xy坐标
    }
    else
    {
        TextureCoords = texture;    //正常情况
    }
    if (shake)  //震动效果
    {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;        
        gl_Position.y += cos(time * 15) * strength;        
    }
}  