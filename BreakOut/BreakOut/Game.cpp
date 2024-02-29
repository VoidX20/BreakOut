#pragma once
#include "Game.h"
#include "ResourceManager.h"

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

}

void Game::Update(GLfloat dt)
{

}


void Game::ProcessInput(GLfloat dt)
{

}

void Game::Render()
{

}