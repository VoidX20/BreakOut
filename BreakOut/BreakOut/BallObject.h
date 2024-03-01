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
	//���״̬	
	GLfloat   Radius;	//�뾶
	GLboolean Stuck;	//�Ƿ�����ڵ�����
	//���캯��
	BallObject();
	BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	/// <summary>
	/// �ƶ���
	/// </summary>
	/// <param name="dt">֡ʱ��</param>
	/// <param name="window_width">��Ļ�Ŀ��</param>
	/// <returns>�ƶ������λ��</returns>
	glm::vec2 Move(GLfloat dt, GLuint window_width);
	/// <summary>
	/// �������״̬
	/// </summary>
	/// <param name="position">Ҫ���õ���λ��</param>
	/// <param name="velocity">���ƺ���ٶ�</param>
	void      Reset(glm::vec2 position, glm::vec2 velocity);
};

