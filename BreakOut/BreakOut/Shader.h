#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	//��ɫ������������glCreateProgram()���ص��Ǹ�����
	unsigned int ID;
	//���캯��
	Shader() { }
	/// <summary>
	/// ���ǰ��ɫ��������glUseProgram()
	/// </summary>
	/// <returns>��ɫ��ID</returns>
	Shader& Use();
	/// <summary>
	/// ���ļ�������ɫ��
	/// </summary>
	/// <param name="vertexSource">������ɫ���ļ�·��</param>
	/// <param name="fragmentSource">Ƭ����ɫ���ļ�·��</param>
	/// <param name="geometrySource">������ɫ���ļ�·������ѡ��</param>
	void    Compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);
	//setter ����
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
	/// �������Ƿ�ɹ�
	/// </summary>
	/// <param name="object">��Ϊ�����Ķ�����ɫ����ƬԪ��ɫ����������ɫ�����������������</param>
	/// <param name="type">��"PROGRAM"�������ӽ������"VERTEX"��"FRAGMENT"�ȴ���ǰ�׶εı����������Զ��壩</param>
	void    checkCompileErrors(unsigned int object, std::string type);
};


