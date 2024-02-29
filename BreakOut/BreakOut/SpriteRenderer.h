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
	//���캯��
	SpriteRenderer(Shader& shader);
	//��������
	~SpriteRenderer();
	//��Ⱦ����
	void DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	//��Ⱦ״̬
	Shader shader;
	GLuint quadVAO;
	//��ʼ����Ⱦ����
	void initRenderData();
};
