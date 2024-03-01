#pragma once
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

//��Ϸ�ؿ�
class GameLevel
{
public:
	//�ؿ�ש������
	std::vector<GameObject> Bricks;
	//���캯��
	GameLevel() { }
	/// <summary>
	/// ���ļ����عؿ�
	/// </summary>
	/// <param name="file">�ؿ��ļ�·��</param>
	/// <param name="levelWidth">�ؿ���һ��ש�������</param>
	/// <param name="levelHeight">�ؿ��ж�����</param>
	void      Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
	//��Ⱦ�ؿ�
	void      Draw(SpriteRenderer& renderer);
	//���ؿ��Ƿ������
	GLboolean IsCompleted();
private:
	//�ӹؿ����ݳ�ʼ���ؿ�����
	void      init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

