#pragma once
#include "Game.h"
#include "ResourceManager.h"

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