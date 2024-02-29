#pragma once
#include <glad/glad.h>

//贴图类
class Texture2D
{
public:
	//贴图ID
	unsigned int ID;
	//贴图宽度和高度
	unsigned int Width, Height;
	//贴图格式
	unsigned int Internal_Format; //贴图对象的格式
	unsigned int Image_Format; //贴图文件的格式
	//贴图配置
	unsigned int Wrap_S; //S轴的纹理环绕方式（横轴，或U轴）
	unsigned int Wrap_T; //T轴的纹理环绕方式（纵轴，或V轴）
	unsigned int Filter_Min; //缩小过滤的纹理过滤模式
	unsigned int Filter_Max; //放大过滤的纹理过滤模式
	//构造函数
	Texture2D();
	//从解析的图片数据创建贴图
	void Generate(unsigned int width, unsigned int height, unsigned char* data);
	//将当前贴图对象绑定为GL_TEXTURE_2D格式
	void Bind() const;
};

