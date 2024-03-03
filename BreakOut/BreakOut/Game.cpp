#pragma once
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "BallObject.h"
#include <iostream>
#include <algorithm>
#include "ParticleGenerator.h"

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;

// ��ʼ������ٶ�
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// ��İ뾶
const GLfloat BALL_RADIUS = 12.5f;


/// <summary>
/// ���캯��
/// </summary>
/// <param name="width">���ڿ��</param>
/// <param name="height">���ڸ߶�</param>
Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

/// <summary>
/// ��������
/// </summary>
Game::~Game()
{
	delete Renderer;
	delete Player;
}

void Game::Init()
{
	// ������ɫ��
	ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("shaders/particle.vert", "shaders/particle.frag", nullptr, "particle");

	// ������ɫ����ͶӰ���󣬲�������ͶӰ�������ֱ�Ϊ���ҡ��¡��ϱ߽磬�Լ���׼���豸���������
	// �ֱ�Ϊ(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f)
	// Ҳ���ǰ�������0��800֮���x����任��-1��1֮�䣬����������0��600֮���y����任��-1��1֮��
	// �����۲�ռ�����ֱ�Ӷ�Ӧ�������ص�����
	//����Ч��Ϊ��
	/*
	|-----------------------------------|x+		|---------------------------------|
	|(0,0)						 (800,0)|		|(-1,-1)					(1,-1)|
	|									|		|								  |
	|									|		|								  |
	|				A					|		|				A				  |
	|									|---->>	|								  |
	|									|		|								  |
	|									|		|								  |
	|(0,600)				   (800,600)|		|(-1,1)						 (1,1)|
	|-----------------------------------|		|---------------------------------|
	y+
	*/
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

	//������ɫ��
	ResourceManager::GetShader("sprite").Use();
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use();
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

	// ��������
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/particle.png", GL_TRUE, "particle");

	// ������Ⱦ������
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	//���عؿ�
	GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;

	//������Ҳ���
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	//������Ĳ���
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::GetTexture("face"));

	//��������ϵͳ
	Particles = new ParticleGenerator(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		500
	);
}

void Game::Update(GLfloat dt)
{
	// ���¶���
	Ball->Move(dt, this->Width);
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));
	// �����ײ
	this->DoCollisions();
	if (Ball->Position.y >= this->Height) // ���Ƿ�Ӵ��ײ��߽磿
	{
		this->ResetLevel();
		this->ResetPlayer();
	}
}


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// �ƶ���ҵ���
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		//���Ʊ���
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		//���ƹؿ�ש��
		this->Levels[this->Level].Draw(*Renderer);
		//�������
		Player->Draw(*Renderer);
		//��������
		Particles->Draw();
		//������
		Ball->Draw(*Renderer);
	}
}

void Game::ResetLevel()
{
	if (this->Level == 0)this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 3)
		this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}



Direction VectorDirection(glm::vec2 target);

/// <summary>
/// ��ײ��⺯��
/// </summary>
/// <param name="one">��</param>
/// <param name="two">ש��</param>
/// <returns>�����</returns>
Collision CheckCollision(BallObject& one, GameObject& two) // AABB - Circle collision
{
	// ��ȡԲ������ 
	glm::vec2 center(one.Position + one.Radius);
	// ����AABB����Ϣ�����ġ���߳���
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);
	// ��ȡ�������ĵĲ�ʸ��
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);	//�ð�߳�ʸ���������Ĳ�ʸ��
	// AABB_center����clamped�����͵õ�����ײ���Ͼ���Բ����ĵ�closest
	glm::vec2 closest = aabb_center + clamped;
	// ���Բ��center�������closest��ʸ�����ж��Ƿ� length <= radius
	difference = closest - center;
	if (glm::length(difference) <= one.Radius){
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	}
	else {
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
	}
}

/// <summary>
/// ��ײ������
/// </summary>
void Game::DoCollisions()
{
	for (GameObject& box : this->Levels[this->Level].Bricks)
	{
		//ש����ײ����
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision)) // ���collision �� true
			{
				// ���ש�鲻��ʵ�ľ�����ש��
				if (!box.IsSolid)
					box.Destroyed = GL_TRUE;
				// ��ײ����
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) // ˮƽ������ײ
				{
					Ball->Velocity.x = -Ball->Velocity.x; // ��תˮƽ�ٶ�
					// �ض�λ
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->Position.x += penetration; // ��������
					else
						Ball->Position.x -= penetration; // ��������
				}
				else // ��ֱ������ײ
				{
					Ball->Velocity.y = -Ball->Velocity.y; // ��ת��ֱ�ٶ�
					// �ض�λ
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->Position.y -= penetration; // ��������
					else
						Ball->Position.y += penetration; // ��������
				}
			}
		}

		//������ײ
		Collision result = CheckCollision(*Ball, *Player);
		if (!Ball->Stuck && std::get<0>(result))
		{
			//��������˵�����ĸ�λ�ã������������ĸ�λ�����ı��ٶ�
			//����ײ������ĵ��루���壩���ĵľ������ı����ˮƽ�ٶ�,ײ������뵲������ĵ�ԽԶ����ˮƽ������ٶȾͻ�Խ��
			GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
			GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
			GLfloat percentage = distance / (Player->Size.x / 2);	//������������뵲�����ĵľ���͵���İ�߳��İٷֱ�
			// ���ݽ���ƶ�
			GLfloat strength = 2.0f;
			glm::vec2 oldVelocity = Ball->Velocity;
			Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
			//�µ��ٶ�ʸ����������Ȼ����Ծ��ٶ�ʸ���ĳ��ȣ�����һ��������������ٶȽ�����һ�µģ�������ײ����������ĸ��ط���
			//���ֻ�Ƿ�תy�ٶ���ô��ͬ����ײλ�û������ͬ����ײ�ٶȣ����������ĺ�45�ȵļнǲ�ͬ������ʸ�����ȷ����仯
			Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);		
			//�޸������⣬���ֵ�ԭ������ҵ����Խϸߵ��ٶ������򣬵���������Ľ�����ҵ��壬��ʹִ���˶����ײ��⣬
			//����y���ٶȷ�ת��Σ����������ֱ�ӷ���һ�����ٶȶ����Ƿ�תy���ٶ�
			Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
		}
	}
}

/// <summary>
/// �ж�ײ������
/// </summary>
/// <param name="target">Բ��center�������closest��ʸ��</param>
/// <returns>ײ������</returns>
Direction VectorDirection(glm::vec2 target)
{
	//�ĸ���׼�����ʸ����������Game.h�ж����ö�ٱ���˳����ͬ
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),  // ��
		glm::vec2(1.0f, 0.0f),  // ��
		glm::vec2(0.0f, -1.0f), // ��
		glm::vec2(-1.0f, 0.0f)  // ��
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	//��Բ��center�������closest��ʸ������ĸ���׼�����ʸ��
	//��ȡ���ֵ������ӽ��ı�׼����ʸ��
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);		
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}