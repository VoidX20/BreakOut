#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

//游戏关卡
class GameLevel
{
public:
	//关卡砖块数据
	std::vector<GameObject> Bricks;
	//构造函数
	GameLevel() { }
	/// <summary>
	/// 从文件加载关卡
	/// </summary>
	/// <param name="file">关卡文件路径</param>
	/// <param name="levelWidth">关卡中一行砖块的数量</param>
	/// <param name="levelHeight">关卡有多少行</param>
	void      Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	//渲染关卡
	void      Draw(SpriteRenderer& renderer);
	//检查关卡是否已完成
	GLboolean IsCompleted();
private:
	//从关卡数据初始化关卡对象
	void      init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

