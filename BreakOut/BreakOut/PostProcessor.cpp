#include "PostProcessor.h"

#include <iostream>

//用帧缓冲将游戏离屏渲染到一个纹理中，然后给这个纹理添加一些视觉效果，再将这个纹理贴到一个与屏幕等大的四边形中，以实现后处理
PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height)
	: PostProcessingShader(shader), Texture(), Width(width), Height(height), Confuse(GL_FALSE), Chaos(GL_FALSE), Shake(GL_FALSE)
{
	//初始化帧缓冲和渲染缓冲对象
	glGenFramebuffers(1, &this->MSFBO);	//Multisampled FBO，多重采样帧缓冲，用于抗锯齿
	glGenFramebuffers(1, &this->FBO);	//普通的帧缓冲，用于将多重采样的颜色缓存传输到纹理
	glGenRenderbuffers(1, &this->RBO);	//渲染缓冲对象


	//绑定帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
	//分配内存空间
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);
	//将渲染缓冲对象附加到MSFBO中，只需要颜色缓冲，不需要深度缓冲和模板缓冲
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
	//检查帧缓冲的完整性
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;


	//绑定FBO对象，用于接受从MSFBO传送过来的颜色值，用于后处理
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
	//创建贴图对象，用作FBO帧缓冲的颜色缓冲对象，也是最终显示的颜色数据
	this->Texture.Generate(width, height, NULL);
	//将这个贴图对象附加到FBO中，作为它的颜色附件
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0);
	//检查帧缓冲的完整性
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;


	//将当前读写的帧缓冲绑定到默认帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//初始化渲染数据
	this->initRenderData();
	this->PostProcessingShader.SetInteger("scene", 0, GL_TRUE);

	//核效果部分
	//偏移值，用于采样
	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {
		{ -offset,  offset  },  // 左上
		{  0.0f,    offset  },  // 正上
		{  offset,  offset  },  // 右上
		{ -offset,  0.0f    },  // 左
		{  0.0f,    0.0f    },  // 中
		{  offset,  0.0f    },  // 右
		{ -offset, -offset  },  // 左下
		{  0.0f,   -offset  },  // 正下
		{  offset, -offset  }   // 右下   
	};
	glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (GLfloat*)offsets);
	//锐化核
	GLint edge_kernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	//模糊核
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
	//将MSFBO中的颜色缓冲解析到FBO中以存储为纹理
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
	glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//将当前读写的帧缓冲绑定到默认帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void PostProcessor::Render(GLfloat time)
{
	//设置渲染数据
	this->PostProcessingShader.Use();
	this->PostProcessingShader.SetFloat("time", time);
	this->PostProcessingShader.SetInteger("confuse", this->Confuse);
	this->PostProcessingShader.SetInteger("chaos", this->Chaos);
	this->PostProcessingShader.SetInteger("shake", this->Shake);
	//将后处理生成的纹理渲染到与屏幕等大的四边形中，完成显示
	glActiveTexture(GL_TEXTURE0);
	this->Texture.Bind();
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{
	// 配置 VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		//顶点坐标		//贴图坐标
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