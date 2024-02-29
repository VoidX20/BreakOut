#pragma once
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"


SpriteRenderer* Renderer;

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
	// 设置专用于渲染的控制
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	// 加载纹理
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");

}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::Render()
{
	Renderer->DrawSprite(ResourceManager::GetTexture("face"),
		glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}