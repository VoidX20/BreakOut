#pragma once
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "Shader.h"


struct Character {
	GLuint     TextureID;  // 字形纹理的ID
	glm::ivec2 Size;       // 字形大小
	glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
	GLuint     Advance;    // 原点距下一个字形原点的距离
};

//文本渲染系统
class TextRenderer
{
public:
	//存储从字形渲染产生的纹理
	std::map<GLchar, Character> Characters;
	//渲染字形的着色器
	Shader TextShader;
	//构造函数
	TextRenderer(GLuint width, GLuint height);
	//预渲染给定的字形范围，生成一批渲染好的字形纹理缓存，如果要一个字渲染一个字效率就太低了，一下渲染一批要什么取什么
	void Load(std::string font, GLuint fontSize);
	//从Load函数生成的字体纹理缓存列表中取得需要的纹理，构成字符串
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	GLuint VAO, VBO;
};
