#pragma once
#include <glad/glad.h>

//��ͼ��
class Texture2D
{
public:
	//��ͼID
	unsigned int ID;
	//��ͼ��Ⱥ͸߶�
	unsigned int Width, Height;
	//��ͼ��ʽ
	unsigned int Internal_Format; //��ͼ����ĸ�ʽ
	unsigned int Image_Format; //��ͼ�ļ��ĸ�ʽ
	//��ͼ����
	unsigned int Wrap_S; //S��������Ʒ�ʽ�����ᣬ��U�ᣩ
	unsigned int Wrap_T; //T��������Ʒ�ʽ�����ᣬ��V�ᣩ
	unsigned int Filter_Min; //��С���˵��������ģʽ
	unsigned int Filter_Max; //�Ŵ���˵��������ģʽ
	//���캯��
	Texture2D();
	//�ӽ�����ͼƬ���ݴ�����ͼ
	void Generate(unsigned int width, unsigned int height, unsigned char* data);
	//����ǰ��ͼ�����ΪGL_TEXTURE_2D��ʽ
	void Bind() const;
};

