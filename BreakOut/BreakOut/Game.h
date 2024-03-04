#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>
#include "GameLevel.h"
#include "PowerUp.h"


// 游戏状态
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

//方向
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

/// <summary>
/// 用于碰撞检测，三个参数分别是：是否碰撞，方向和差矢量
/// </summary>
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

//玩家挡板大小
const glm::vec2 PLAYER_SIZE(100, 20);
//玩家挡板速度
const GLfloat PLAYER_VELOCITY(500.0f);

//Game类持有所有与游戏有关的状态变量和函数，采用单例模式设计
class Game
{

private:
	// 构造函数
	Game(GLuint width, GLuint height);

public:
	GameState				State;				//游戏状态
	GLboolean				Keys[1024];			//存储键盘输入
	GLboolean				KeysProcessed[1024];//存储已处理的键盘输入
	GLuint					Width, Height;		//屏幕宽高
	std::vector<GameLevel>	Levels;				//所有关卡对象的集合
	GLuint					Level;				//当前关卡序号
	std::vector<PowerUp>	PowerUps;				//游戏中的道具
	GLuint Lives;								//玩家生命值

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
	//碰撞处理相关
	void DoCollisions();
	//重置相关
	void ResetLevel();	//重置当前关卡
	void ResetPlayer();	//重置玩家和球
	//道具相关
	void SpawnPowerUps(GameObject& block);	//在指定位置生成一个道具块
	void UpdatePowerUps(GLfloat dt);		//更新所有当前被激活的道具
};

