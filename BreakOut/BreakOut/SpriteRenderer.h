#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture2D.h"
#include "shader.h"

/// <summary>
/// 
/// </summary>
class SpriteRenderer
{
public:
	//构造函数
	SpriteRenderer(Shader& shader);
	//析构函数
	~SpriteRenderer();
	//渲染精灵
	void DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	//渲染状态
	Shader shader;
	GLuint quadVAO;
	//初始化渲染数据
	void initRenderData();
};
