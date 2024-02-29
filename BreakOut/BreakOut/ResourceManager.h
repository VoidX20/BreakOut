#pragma once
#include <glad/glad.h>
#include <map>
#include <string>
#include "Texture2D.h"
#include "Shader.h"

/// <summary>
/// �����࣬���ڹ�����ɫ������ͼ
/// </summary>
class ResourceManager
{
public:
	//�洢��ɫ������ͼ����ʵ�ְ�����ȡ
	static std::map<std::string, Shader>    Shaders;
	static std::map<std::string, Texture2D> Textures;
	/// <summary>
	/// ���ļ�������ɫ��
	/// </summary>
	/// <param name="vShaderFile">������ɫ���ļ�·��</param>
	/// <param name="fShaderFile">Ƭ����ɫ���ļ�·��</param>
	/// <param name="gShaderFile">������ɫ���ļ�·������ѡ��</param>
	/// <param name="name">��ɫ������</param>
	/// <returns>��ɫ������</returns>
	static Shader&    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	/// <summary>
	/// �����ƻ�ȡ��ɫ������
	/// </summary>
	/// <param name="name">��ɫ������</param>
	/// <returns>��ɫ������</returns>
	static Shader&    GetShader(std::string name);
	/// <summary>
	/// ���ļ�������ͼ
	/// </summary>
	/// <param name="file">��ͼ�ļ�·��</param>
	/// <param name="alpha">�Ƿ���alphaͨ��</param>
	/// <param name="name">��ͼ����</param>
	/// <returns>��ͼ����</returns>
	static Texture2D& LoadTexture(const char* file, bool alpha, std::string name);
	/// <summary>
	/// �����ƻ�ȡ��ͼ����
	/// </summary>
	/// <param name="name">��ͼ����</param>
	/// <returns>��ͼ����</returns>
	static Texture2D& GetTexture(std::string name);
	/// <summary>
	/// ���������Ѽ��ص���Դ
	/// </summary>
	static void      Clear();
private:
	/// <summary>
	/// ���캯����˽�е�ԭ��������಻��Ҫ�κ�ʵ�����Ķ����������з������Ǿ�̬��
	/// </summary>
	ResourceManager() { }
	/// <summary>
	/// ���ļ�������ɫ��
	/// </summary>
	/// <param name="vShaderFile">������ɫ���ļ�·��</param>
	/// <param name="fShaderFile">Ƭ����ɫ���ļ�·��</param>
	/// <param name="gShaderFile">������ɫ���ļ�·������ѡ��</param>
	/// <returns>��ɫ������</returns>
	static Shader&    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	/// <summary>
	/// ���ļ�������ͼ
	/// </summary>
	/// <param name="file">��ͼ�ļ�·��</param>
	/// <param name="alpha">�Ƿ���alphaͨ��</param>
	/// <returns>��ͼ����</returns>
	static Texture2D& loadTextureFromFile(const char* file, bool alpha);
};
