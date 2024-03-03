#pragma once
#include "GameObject.h"
#include <glad/glad.h>
#include <glm/glm.hpp>


//���߷���Ĵ�С
const glm::vec2 SIZE(60, 20);
//���߷�����ٶ�
const glm::vec2 VELOCITY(0.0f, 150.0f);

//�����࣬��ש�鱻������������һ������
class PowerUp : public GameObject
{
public:
	//���࣬��Ϊ Speed��Sticky��Pass-Through��Pad-Size-Increase��Confuse��Chaos ������֮һ
	// 	Speed : ����С��20 % ���ٶ�
	// 	Sticky : ��С������ҵ���Ӵ�ʱ��С��ᱣ��ճ�ڵ����ϵ�״ֱ̬���ٴΰ��¿ո�����������������ͷ�С��ǰ�ҵ������ʵ�λ��
	// 	Pass - Through : ��ʵ��ש�����ײ�������ã�ʹС����Դ������ݻٶ��ש��
	// 	Pad - Size - Increase : ������ҵ���50���صĿ��
	// 	Confuse : ��ʱ���ڼ���confuse������Ч���Ի����
	// 	Chaos : ��ʱ���ڼ���chaos������Ч��ʹ�����ʧ����
	std::string Type;
	//����ʱ��
	GLfloat     Duration;
	//�Ƿ��Ѽ���
	GLboolean   Activated;
	//���캯��
	PowerUp(std::string type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture)
		: GameObject(position, SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() { 
	}
};

