#pragma once
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"


SpriteRenderer* Renderer;
GameObject* Player;

/// <summary>
/// 构造函数
/// </summary>
/// <param name="width">窗口宽度</param>
/// <param name="height">窗口高度</param>
Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

/// <summary>
/// 析构函数
/// </summary>
Game::~Game()
{
	delete Renderer;
	delete Player;
}

void Game::Init()
{
	// 加载着色器
	ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	// 配置着色器
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	// 加载纹理
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");

	// 设置渲染器对象
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	//加载关卡
	GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;

	//配置玩家参数
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		//移动玩家挡板
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
				Player->Position.x -= velocity;
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
				Player->Position.x += velocity;
		}
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		//绘制背景
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		//绘制关卡砖块
		this->Levels[this->Level].Draw(*Renderer);
		//绘制玩家
		Player->Draw(*Renderer);
	}
}