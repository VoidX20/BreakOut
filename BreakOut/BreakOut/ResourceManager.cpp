#include "ResourceManager.h"
#include "stb_image.h"
#include <iostream>
#include <sstream>
#include <fstream>

//实例化静态变量
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
	//清除所有已载入的着色器
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	//清除所有已载入的贴图
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader& ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	//从文件读取文件数据
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		//打开文件
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		//读取文件到流
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		//关闭文件
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//从流转换到字符串
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		//如果提供了几何着色器就读取
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
	//创建新的着色器对象
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D& ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
	//创建贴图对象
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	//读取贴图文件
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	//创建贴图
	texture.Generate(width, height, data);
	//清除已读取的贴图文件的数据
	stbi_image_free(data);
	return texture;
}