#include "ResourceManager.h"
#include "stb_image.h"
#include <iostream>
#include <sstream>
#include <fstream>

//ʵ������̬����
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader& ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D& ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	//����������������ɫ��
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	//����������������ͼ
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader& ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	//���ļ���ȡ�ļ�����
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		//���ļ�
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		//��ȡ�ļ�����
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		//�ر��ļ�
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//����ת�����ַ���
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//����ṩ�˼�����ɫ���Ͷ�ȡ
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	//�����µ���ɫ������
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D& ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
	//������ͼ����
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	//��ȡ��ͼ�ļ�
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	//������ͼ
	texture.Generate(width, height, data);
	//����Ѷ�ȡ����ͼ�ļ�������
	stbi_image_free(data);
	return texture;
}