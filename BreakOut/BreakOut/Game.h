#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// ��Ϸ״̬
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

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

