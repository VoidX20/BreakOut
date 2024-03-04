#pragma once
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "Shader.h"


struct Character {
	GLuint     TextureID;  // ���������ID
	glm::ivec2 Size;       // ���δ�С
	glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
	GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
};

//�ı���Ⱦϵͳ
class TextRenderer
{
public:
	//�洢��������Ⱦ����������
	std::map<GLchar, Character> Characters;
	//��Ⱦ���ε���ɫ��
	Shader TextShader;
	//���캯��
	TextRenderer(GLuint width, GLuint height);
	//Ԥ��Ⱦ���������η�Χ������һ����Ⱦ�õ����������棬���Ҫһ������Ⱦһ����Ч�ʾ�̫���ˣ�һ����Ⱦһ��Ҫʲôȡʲô
	void Load(std::string font, GLuint fontSize);
	//��Load�������ɵ������������б���ȡ����Ҫ�����������ַ���
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
	GLuint VAO, VBO;
};
