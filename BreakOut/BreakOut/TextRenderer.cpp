#include "TextRenderer.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "ResourceManager.h"


TextRenderer::TextRenderer(GLuint width, GLuint height)
{
	//载入并配置着色器
	this->TextShader = ResourceManager::LoadShader("shaders/text_2d.vert", "shaders/text_2d.frag", nullptr, "text");
	this->TextShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
	this->TextShader.SetInteger("text", 0);
	//配置VAO和VBO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::Load(std::string font, GLuint fontSize)
{
	//清除内存中已有的字体缓存
	this->Characters.clear();
	//初始化FreeType2
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) //发生任何错误时，返回值都不为0
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	//字体加载为一个FreeType称之为面(Face)的东西
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	//设置字体大小
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	//设置为按1字节对其，OpenGL默认是4字节对其的，由于我们选择将字形渲染出的纹理设置为8位的灰度图（单通道），因此一个像素的颜色只需要1字节就行了
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//预渲染ASC2的128个字符
	for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
	{
		//加载字形
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		//生成纹理
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,		//设置为单通道8位灰度图
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		//设置纹理过滤选项
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//将字体存储在字体纹理缓存中
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	//释放资源
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	//激活对应的渲染状态
	this->TextShader.Use();
	this->TextShader.SetVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	//遍历文本中所有的字符
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;	//要注意在游戏中y向下增长，因此需要改变动态计算四边形顶点位置的方法，'H'这个字符是正好与顶部接触的，'X'之类的也可以，但是'g'、'j'之类的是偏的

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		//对每个字符更新VBO
		//动态计算要渲染的字体的要贴上纹理的四边形的顶点
		//即：
		//|---------------|					*********
		//|				  |						*
		//|				  |						*
		//|(预渲染的纹理) |---------->			*
		//|				  |						*
		//|				  |						*
		//|---------------|					*********
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos,     ypos,       0.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 }
		};
		//在四边形上绘制字形纹理
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		//更新VBO内存的内容
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); //必须使用glBufferSubData而不是glBufferData
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//渲染四边形
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//更新位置到下一个字形的原点，注意单位是1/64像素
		x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}