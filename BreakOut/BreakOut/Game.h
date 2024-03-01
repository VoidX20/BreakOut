#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "GameLevel.h"


// ��Ϸ״̬
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

//��ҵ����С
const glm::vec2 PLAYER_SIZE(100, 20);
//��ҵ����ٶ�
const GLfloat PLAYER_VELOCITY(500.0f);

//Game�������������Ϸ�йص�״̬�����ͺ��������õ���ģʽ���
class Game
{

private:
	// ���캯��
	Game(GLuint width, GLuint height);

public:
	GameState              State;				//��Ϸ״̬
	GLboolean              Keys[1024];			//�洢��������
	GLuint                 Width, Height;		//��Ļ���
	std::vector<GameLevel> Levels;				//�ؿ�ש������
	GLuint                 Level;

	//�ֲ���̬����ʵ�ֵ�ʵ������������ҪC++11����
	static Game& instantce(GLuint Width, GLuint Height) {
		static Game instantce_(Width,Height);
		return instantce_;
	}

	~Game();
	// ��ʼ����Ϸ״̬���������е���ɫ��/����/�ؿ���
	void Init();
	// ��Ϸѭ��
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};

