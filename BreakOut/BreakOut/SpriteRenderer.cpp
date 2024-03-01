#include "SpriteRenderer.h"
SpriteRenderer::SpriteRenderer(Shader& shader)
{
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::DrawSprite(const Texture2D& texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color)
{
	//构造变换矩阵
	this->shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));  //3.平移到指定位置

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));	//再将原点移回去 
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); //2.旋转
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));	//将变换原点移至四边形的中心

	model = glm::scale(model, glm::vec3(size, 1.0f)); //1.缩放

	//注意矩阵是左乘的，因此变换顺序要反过来，先缩放，再旋转，最后平移
	this->shader.SetMatrix4("model", model);

	this->shader.SetVector3f("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	//渲染精灵
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRenderData()
{
	// 配置 VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = {
		//一个以左上角为(0,0)原点的四边形
		// 注意y是向下增长的，这由我们选择的透视投影矩阵所决定,在Game::Init()中配置
		// (0,0)
		// |--------------------->x+
		// |				|
		// |				|
		// |				|
		// |----------------|
		// |				(1,1)
		// y+
		// 
		// 位置      // 纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}