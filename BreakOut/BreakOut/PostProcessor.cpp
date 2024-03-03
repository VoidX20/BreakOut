#include "PostProcessor.h"

#include <iostream>

//��֡���彫��Ϸ������Ⱦ��һ�������У�Ȼ�������������һЩ�Ӿ�Ч�����ٽ������������һ������Ļ�ȴ���ı����У���ʵ�ֺ���
PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height)
	: PostProcessingShader(shader), Texture(), Width(width), Height(height), Confuse(GL_FALSE), Chaos(GL_FALSE), Shake(GL_FALSE)
{
	//��ʼ��֡�������Ⱦ�������
	glGenFramebuffers(1, &this->MSFBO);	//Multisampled FBO�����ز���֡���壬���ڿ����
	glGenFramebuffers(1, &this->FBO);	//��ͨ��֡���壬���ڽ����ز�������ɫ���洫�䵽����
	glGenRenderbuffers(1, &this->RBO);	//��Ⱦ�������


	//��֡����
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	//�����ڴ�ռ�
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);
	//����Ⱦ������󸽼ӵ�MSFBO�У�ֻ��Ҫ��ɫ���壬����Ҫ��Ȼ����ģ�建��
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
	//���֡�����������
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;


	//��FBO�������ڽ��ܴ�MSFBO���͹�������ɫֵ�����ں���
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	//������ͼ��������FBO֡�������ɫ�������Ҳ��������ʾ����ɫ����
	this->Texture.Generate(width, height, NULL);
	//�������ͼ���󸽼ӵ�FBO�У���Ϊ������ɫ����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0);
	//���֡�����������
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;


	//����ǰ��д��֡����󶨵�Ĭ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//��ʼ����Ⱦ����
	this->initRenderData();
	this->PostProcessingShader.SetInteger("scene", 0, GL_TRUE);

	//��Ч������
	//ƫ��ֵ�����ڲ���
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {
		{ -offset,  offset  },  // ����
		{  0.0f,    offset  },  // ����
		{  offset,  offset  },  // ����
		{ -offset,  0.0f    },  // ��
		{  0.0f,    0.0f    },  // ��
		{  offset,  0.0f    },  // ��
		{ -offset, -offset  },  // ����
		{  0.0f,   -offset  },  // ����
		{  offset, -offset  }   // ����   
	};
	glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (GLfloat*)offsets);
	//�񻯺�
	GLint edge_kernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	//ģ����
	glUniform1iv(glGetUniformLocation(this->PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
	GLfloat blur_kernel[9] = {
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(this->PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
void PostProcessor::EndRender()
{
	//��MSFBO�е���ɫ���������FBO���Դ洢Ϊ����
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//����ǰ��д��֡����󶨵�Ĭ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void PostProcessor::Render(GLfloat time)
{
	//������Ⱦ����
	this->PostProcessingShader.Use();
	this->PostProcessingShader.SetFloat("time", time);
	this->PostProcessingShader.SetInteger("confuse", this->Confuse);
	this->PostProcessingShader.SetInteger("chaos", this->Chaos);
	this->PostProcessingShader.SetInteger("shake", this->Shake);
	//���������ɵ�������Ⱦ������Ļ�ȴ���ı����У������ʾ
	glActiveTexture(GL_TEXTURE0);
	this->Texture.Bind();
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{
	// ���� VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		//��������		//��ͼ����
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}