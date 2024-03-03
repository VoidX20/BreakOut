#pragma once
#include "GameObject.h"
#include <glad/glad.h>
#include <glm/glm.hpp>


//道具方块的大小
const glm::vec2 SIZE(60, 20);
//道具方块的速度
const glm::vec2 VELOCITY(0.0f, 150.0f);

//道具类，在砖块被击碎后随机产生一个道具
class PowerUp : public GameObject
{
public:
	//种类，可为 Speed、Sticky、Pass-Through、Pad-Size-Increase、Confuse、Chaos 的其中之一
	// 	Speed : 增加小球20 % 的速度
	// 	Sticky : 当小球与玩家挡板接触时，小球会保持粘在挡板上的状态直到再次按下空格键，这可以让玩家在释放小球前找到更合适的位置
	// 	Pass - Through : 非实心砖块的碰撞处理被禁用，使小球可以穿过并摧毁多个砖块
	// 	Pad - Size - Increase : 增加玩家挡板50像素的宽度
	// 	Confuse : 短时间内激活confuse后期特效，迷惑玩家
	// 	Chaos : 短时间内激活chaos后期特效，使玩家迷失方向
	std::string Type;
	//持续时间
	GLfloat     Duration;
	//是否已激活
	GLboolean   Activated;
	//构造函数
	PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
		: GameObject(position, SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() { 
	}
};

