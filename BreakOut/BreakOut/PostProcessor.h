#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "Shader.h"

//����ϵͳ
class PostProcessor
{
public:
	//״̬
	Shader PostProcessingShader;
	Texture2D Texture;
	GLuint Width, Height;
	//��Чѡ��
	GLboolean Confuse, Chaos, Shake;
	//���캯��
	PostProcessor(Shader shader, GLuint width, GLuint height);
	// ��ʼ������Ⱦ
	void BeginRender();
	// ����������Ⱦ
	void EndRender();
	// ��Ⱦ����������Ļ��
	void Render(GLfloat time);
private:
	// Render state
	GLuint MSFBO, FBO; // Multisampled FBO�����ز���֡����
	GLuint RBO; //��Ⱦ�������
	GLuint VAO;
	// ��ʼ����������
	void initRenderData();
};

