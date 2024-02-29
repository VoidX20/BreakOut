#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	//着色器索引，就是glCreateProgram()返回的那个索引
	unsigned int ID;
	//构造函数
	Shader() { }
	/// <summary>
	/// 激活当前着色器，就是glUseProgram()
	/// </summary>
	/// <returns>着色器ID</returns>
	Shader& Use();
	/// <summary>
	/// 从文件编译着色器
	/// </summary>
	/// <param name="vertexSource">顶点着色器文件路径</param>
	/// <param name="fragmentSource">片段着色器文件路径</param>
	/// <param name="geometrySource">几何着色器文件路径（可选）</param>
	void    Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
	//setter 函数
	void    SetFloat(const char* name, float value, bool useShader = false);
	void    SetInteger(const char* name, int value, bool useShader = false);
	void    SetVector2f(const char* name, float x, float y, bool useShader = false);
	void    SetVector2f(const char* name, const glm::vec2& value, bool useShader = false);
	void    SetVector3f(const char* name, float x, float y, float z, bool useShader = false);
	void    SetVector3f(const char* name, const glm::vec3& value, bool useShader = false);
	void    SetVector4f(const char* name, float x, float y, float z, float w, bool useShader = false);
	void    SetVector4f(const char* name, const glm::vec4& value, bool useShader = false);
	void    SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader = false);
private:
	/// <summary>
	/// 检查编译是否成功
	/// </summary>
	/// <param name="object">可为编译后的顶点着色器、片元着色器、几何着色器或其它对象的索引</param>
	/// <param name="type">以"PROGRAM"代表链接结果，以"VERTEX"、"FRAGMENT"等代表当前阶段的编译结果（可自定义）</param>
	void    checkCompileErrors(unsigned int object, std::string type);
};


