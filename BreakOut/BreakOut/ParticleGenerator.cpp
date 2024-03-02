#include "ParticleGenerator.h"
ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
	: shader(shader), texture(texture), amount(amount)
{
	this->init();
}

void ParticleGenerator::Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset)
{
	//添加新的粒子
	for (GLuint i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	//更新所有粒子的状态
	for (GLuint i = 0; i < this->amount; ++i)
	{
		Particle& p = this->particles[i];
		p.Life -= dt; //减少存活时间
		//如果粒子仍然存活，则更新其状态
		if (p.Life > 0.0f)
		{
			p.Position -= p.Velocity * dt;		//降低速度
			p.Color.a -= dt * 2.5;				//降低亮度
		}
	}
}

//渲染所有粒子
void ParticleGenerator::Draw()
{
	//用于制造发光效果
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.Use();
	for (Particle &particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			this->shader.SetVector2f("offset", particle.Position);
			this->shader.SetVector4f("color", particle.Color);
			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	//恢复默认混合模式
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	//设置顶点坐标和贴图坐标
	GLuint VBO;
	GLfloat particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	//创建粒子实例
	for (GLuint i = 0; i < this->amount; ++i)
		this->particles.push_back(Particle());
}

//寻找第一个未使用的或者已经死亡的粒子
GLuint lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
	//搜索上次返回的粒子的右边，因为下一个死亡的粒子总是在上一个的右边
	for (GLuint i = lastUsedParticle; i < this->amount; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	//搜索上次返回的粒子的左边
	for (GLuint i = 0; i < lastUsedParticle; ++i) {
		if (this->particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	//所有粒子都存活，则覆盖第一个粒子，这意味着存活时间过长
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
	GLfloat random = ((rand() % 100) - 50) / 10.0f;
	GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;		//在物体周围分配随机位置
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);	//随机亮度
	particle.Life = 0.35f;										//初始存活时间
	particle.Velocity = object.Velocity * 0.1f;					//初始速度
}