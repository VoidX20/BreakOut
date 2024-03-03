#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"


//粒子当前的状态
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

//粒子发射器
class ParticleGenerator
{
public:
	// 构造函数
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	// 更新所有粒子的状态
	void Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// 渲染所有的粒子
	void Draw();
private:
	// 粒子集合
	std::vector<Particle> particles;
	GLuint amount;		//粒子数量
	// 渲染状态
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	// 初始化粒子发射器
	void init();
	// 寻找第一个未使用的或者已经死亡的粒子
	GLuint firstUnusedParticle();
	// 重新激活粒子
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
