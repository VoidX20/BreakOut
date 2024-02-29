#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// 游戏状态
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

//Game类持有所有与游戏有关的状态变量和函数，采用单例模式设计
class Game
{

private:
	// 构造函数
	Game(GLuint width, GLuint height);

public:
	GameState              State;				//游戏状态
	GLboolean              Keys[1024];			//存储键盘输入
	GLuint                 Width, Height;		//屏幕宽高

	//局部静态变量实现的实例化函数，需要C++11以上
	static Game& instantce(GLuint Width, GLuint Height) {
		static Game instantce_(Width,Height);
		return instantce_;
	}

	~Game();
	// 初始化游戏状态（加载所有的着色器/纹理/关卡）
	void Init();
	// 游戏循环
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
};

