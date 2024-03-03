#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture2D.h"
#include "GameObject.h"


//���ӵ�ǰ��״̬
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	GLfloat Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

//���ӷ�����
class ParticleGenerator
{
public:
	// ���캯��
	ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);
	// �����������ӵ�״̬
	void Update(GLfloat dt, GameObject& object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	// ��Ⱦ���е�����
	void Draw();
private:
	// ���Ӽ���
	std::vector<Particle> particles;
	GLuint amount;		//��������
	// ��Ⱦ״̬
	Shader shader;
	Texture2D texture;
	GLuint VAO;
	// ��ʼ�����ӷ�����
	void init();
	// Ѱ�ҵ�һ��δʹ�õĻ����Ѿ�����������
	GLuint firstUnusedParticle();
	// ���¼�������
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
