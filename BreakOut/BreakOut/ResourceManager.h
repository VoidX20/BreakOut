#pragma once
#include <glad/glad.h>
#include <map>
#include <string>
#include "Texture2D.h"
#include "Shader.h"

/// <summary>
/// 工具类，用于管理着色器和贴图
/// </summary>
class ResourceManager
{
public:
	//存储着色器和贴图对象，实现按名存取
	static std::map<std::string, Shader>    Shaders;
	static std::map<std::string, Texture2D> Textures;
	/// <summary>
	/// 从文件载入着色器
	/// </summary>
	/// <param name="vShaderFile">顶点着色器文件路径</param>
	/// <param name="fShaderFile">片段着色器文件路径</param>
	/// <param name="gShaderFile">几何着色器文件路径（可选）</param>
	/// <param name="name">着色器名称</param>
	/// <returns>着色器对象</returns>
	static Shader&    LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name);
	/// <summary>
	/// 按名称获取着色器对象
	/// </summary>
	/// <param name="name">着色器名称</param>
	/// <returns>着色器对象</returns>
	static Shader&    GetShader(std::string name);
	/// <summary>
	/// 从文件载入贴图
	/// </summary>
	/// <param name="file">贴图文件路径</param>
	/// <param name="alpha">是否有alpha通道</param>
	/// <param name="name">贴图名称</param>
	/// <returns>贴图对象</returns>
	static Texture2D& LoadTexture(const char* file, bool alpha, std::string name);
	/// <summary>
	/// 按名称获取贴图对象
	/// </summary>
	/// <param name="name">贴图名称</param>
	/// <returns>贴图对象</returns>
	static Texture2D& GetTexture(std::string name);
	/// <summary>
	/// 清理所有已加载的资源
	/// </summary>
	static void      Clear();
private:
	/// <summary>
	/// 构造函数，私有的原因是这个类不需要任何实例化的对象，它的所有方法都是静态的
	/// </summary>
	ResourceManager() { }
	/// <summary>
	/// 从文件载入着色器
	/// </summary>
	/// <param name="vShaderFile">顶点着色器文件路径</param>
	/// <param name="fShaderFile">片段着色器文件路径</param>
	/// <param name="gShaderFile">几何着色器文件路径（可选）</param>
	/// <returns>着色器对象</returns>
	static Shader&    loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	/// <summary>
	/// 从文件载入贴图
	/// </summary>
	/// <param name="file">贴图文件路径</param>
	/// <param name="alpha">是否有alpha通道</param>
	/// <returns>贴图对象</returns>
	static Texture2D& loadTextureFromFile(const char* file, bool alpha);
};
