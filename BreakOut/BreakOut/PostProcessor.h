#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"
#include "Shader.h"

//后处理系统
class PostProcessor
{
public:
	//状态
	Shader PostProcessingShader;
	Texture2D Texture;
	GLuint Width, Height;
	//特效选项
	GLboolean Confuse, Chaos, Shake;
	//构造函数
	PostProcessor(Shader shader, GLuint width, GLuint height);
	// 开始离屏渲染
	void BeginRender();
	// 结束离屏渲染
	void EndRender();
	// 渲染后处理结果到屏幕中
	void Render(GLfloat time);
private:
	// Render state
	GLuint MSFBO, FBO; // Multisampled FBO，多重采样帧缓冲
	GLuint RBO; //渲染缓冲对象
	GLuint VAO;
	// 初始化后处理数据
	void initRenderData();
};

