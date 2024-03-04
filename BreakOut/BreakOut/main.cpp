﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "game.h"
#include "ResourceManager.h"


/// <summary>
/// 键盘输入的回调函数
/// </summary>
/// <param name="window">游戏窗口指针</param>
/// <param name="key">变动的按键</param>
/// <param name="scancode"></param>
/// <param name="action">动作，按下或弹起</param>
/// <param name="mode"></param>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//窗口宽度
const GLuint SCREEN_WIDTH = 800;
//窗口高度
const GLuint SCREEN_HEIGHT = 600;

Game& Breakout = Game::instantce(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
	std::cout << "*******************" << std::endl;
	std::cout << "*     BREAKOUT    *" << std::endl;
	std::cout << "*******************" << std::endl;

	std::cout << "初始化GLFW...";
	//glfw初始化，用于窗口管理
	glfwInit();
	//设置目标OpenGL版本330 core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//设置OpenGL大版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					//设置OpenGL小版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//使用核心模式
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//创建窗口
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//载入所有的OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//注册回调函数
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::cout << "成功" << std::endl;

	std::cout << "配置OpenGL...";
	//配置OpenGL
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);		//配置viewport大小
	glEnable(GL_CULL_FACE);								//启用面剔除
	glEnable(GL_DEPTH);									//启用深度测试
	glEnable(GL_BLEND);									//启用混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//配置混合模式
	std::cout << "成功" << std::endl;

	//初始化游戏
	Breakout.Init();

	//用于计算帧时间以控制游戏速度
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	//游戏循环
	while (!glfwWindowShouldClose(window))
	{
		//计算帧时间
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//获取输入
		Breakout.ProcessInput(deltaTime);

		//更新游戏状态
		Breakout.Update(deltaTime);

		//渲染
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		Breakout.Render();

		glfwSwapBuffers(window);
	}

	//释放资源
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

//键盘输入回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//esc键关闭游戏
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		std::cout << "游戏关闭！" << std::endl;
	}
	//记录按键状态,bitmap模式
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			Breakout.Keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			Breakout.Keys[key] = false;
			Breakout.KeysProcessed[key] = false;
		}
	}
}

//窗口大小发生变动时的回调函数（备用）
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}