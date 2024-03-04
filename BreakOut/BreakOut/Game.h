#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>
#include "GameLevel.h"
#include "PowerUp.h"


// ��Ϸ״̬
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

//����
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

/// <summary>
/// ������ײ��⣬���������ֱ��ǣ��Ƿ���ײ������Ͳ�ʸ��
/// </summary>
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

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
	GameState				State;				//��Ϸ״̬
	GLboolean				Keys[1024];			//�洢��������
	GLboolean				KeysProcessed[1024];//�洢�Ѵ���ļ�������
	GLuint					Width, Height;		//��Ļ���
	std::vector<GameLevel>	Levels;				//���йؿ�����ļ���
	GLuint					Level;				//��ǰ�ؿ����
	std::vector<PowerUp>	PowerUps;				//��Ϸ�еĵ���
	GLuint Lives;								//�������ֵ

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
	//��ײ�������
	void DoCollisions();
	//�������
	void ResetLevel();	//���õ�ǰ�ؿ�
	void ResetPlayer();	//������Һ���
	//�������
	void SpawnPowerUps(GameObject& block);	//��ָ��λ������һ�����߿�
	void UpdatePowerUps(GLfloat dt);		//�������е�ǰ������ĵ���
};

