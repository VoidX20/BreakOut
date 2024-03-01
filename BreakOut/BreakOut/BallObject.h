#pragma once
#include "GameObject.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Texture2D.h"
#include "SpriteRenderer.h"

class BallObject :
    public GameObject
{
public:
	//球的状态	
	GLfloat   Radius;	//半径
	GLboolean Stuck;	//是否黏着在挡板上
	//构造函数
	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	/// <summary>
	/// 移动球
	/// </summary>
	/// <param name="dt">帧时间</param>
	/// <param name="window_width">屏幕的宽度</param>
	/// <returns>移动后球的位置</returns>
	glm::vec2 Move(GLfloat dt, GLuint window_width);
	/// <summary>
	/// 重置球的状态
	/// </summary>
	/// <param name="position">要重置到的位置</param>
	/// <param name="velocity">重制后的速度</param>
	void      Reset(glm::vec2 position, glm::vec2 velocity);
};

