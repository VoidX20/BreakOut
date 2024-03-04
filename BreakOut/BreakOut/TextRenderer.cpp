#include "TextRenderer.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "ResourceManager.h"


TextRenderer::TextRenderer(GLuint width, GLuint height)
{
	//���벢������ɫ��
	this->TextShader = ResourceManager::LoadShader("shaders/text_2d.vert", "shaders/text_2d.frag", nullptr, "text");
	this->TextShader.SetMatrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
	this->TextShader.SetInteger("text", 0);
	//����VAO��VBO
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
	//����ڴ������е����建��
	this->Characters.clear();
	//��ʼ��FreeType2
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) //�����κδ���ʱ������ֵ����Ϊ0
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	//�������Ϊһ��FreeType��֮Ϊ��(Face)�Ķ���
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	//���������С
	FT_Set_Pixel_Sizes(face, 0, fontSize);
	//����Ϊ��1�ֽڶ��䣬OpenGLĬ����4�ֽڶ���ģ���������ѡ��������Ⱦ������������Ϊ8λ�ĻҶ�ͼ����ͨ���������һ�����ص���ɫֻ��Ҫ1�ֽھ�����
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//Ԥ��ȾASC2��128���ַ�
	for (GLubyte c = 0; c < 128; c++) // lol see what I did there 
	{
		//��������
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		//��������
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,		//����Ϊ��ͨ��8λ�Ҷ�ͼ
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		//�����������ѡ��
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//������洢��������������
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	//�ͷ���Դ
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	//�����Ӧ����Ⱦ״̬
	this->TextShader.Use();
	this->TextShader.SetVector3f("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	//�����ı������е��ַ�
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (this->Characters['H'].Bearing.y - ch.Bearing.y) * scale;	//Ҫע������Ϸ��y���������������Ҫ�ı䶯̬�����ı��ζ���λ�õķ�����'H'����ַ��������붥���Ӵ��ģ�'X'֮���Ҳ���ԣ�����'g'��'j'֮�����ƫ��

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		//��ÿ���ַ�����VBO
		//��̬����Ҫ��Ⱦ�������Ҫ����������ı��εĶ���
		//����
		//|---------------|					*********
		//|				  |						*
		//|				  |						*
		//|(Ԥ��Ⱦ������) |---------->			*
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
		//���ı����ϻ�����������
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		//����VBO�ڴ������
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); //����ʹ��glBufferSubData������glBufferData
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//��Ⱦ�ı���
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
		x += (ch.Advance >> 6) * scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}