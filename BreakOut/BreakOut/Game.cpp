#pragma once
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "BallObject.h"
#include <iostream>
#include <algorithm>
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "PowerUp.h"
#include "audio/irrKlang/include/irrKlang.h"
using namespace irrklang;

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

//初始化音频引擎
ISoundEngine* SoundEngine = createIrrKlangDevice();

//一些全局变量
SpriteRenderer* Renderer;
GameObject* Player;
BallObject* Ball;
ParticleGenerator* Particles;
PostProcessor* Effects;
GLfloat            ShakeTime = 0.0f;

// 初始化球的速度
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// 球的半径
const GLfloat BALL_RADIUS = 12.5f;


/// <summary>
/// 构造函数
/// </summary>
/// <param name="width">窗口宽度</param>
/// <param name="height">窗口高度</param>
Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

/// <summary>
/// 析构函数
/// </summary>
Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
}

void Game::Init()
{
	// 加载着色器
	ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("shaders/particle.vert", "shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadShader("shaders/post_processing.vert", "shaders/post_processing.frag", nullptr, "postprocessing");

	// 配置着色器的投影矩阵，采用正射投影，参数分别为左、右、下、上边界，以及标准化设备坐标的区域
	// 分别为(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f)
	// 也就是把所有在0到800之间的x坐标变换到-1到1之间，并把所有在0到600之间的y坐标变换到-1到1之间
	// 这样观察空间坐标直接对应最终像素的坐标
	//最终效果为：
	/*
	|-----------------------------------|x+		|---------------------------------|
	|(0,0)						 (800,0)|		|(-1,-1)					(1,-1)|
	|									|		|								  |
	|									|		|								  |
	|				A					|		|				A				  |
	|									|---->>	|								  |
	|									|		|								  |
	|									|		|								  |
	|(0,600)				   (800,600)|		|(-1,1)						 (1,1)|
	|-----------------------------------|		|---------------------------------|
	y+
	*/
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);

	//配置着色器
	ResourceManager::GetShader("sprite").Use();
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use();
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

	// 加载纹理
	ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", GL_TRUE, "paddle");
	ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("textures/particle.png", GL_TRUE, "particle");
	ResourceManager::LoadTexture("textures/powerup_speed.png", GL_TRUE, "powerup_speed");
	ResourceManager::LoadTexture("textures/powerup_sticky.png", GL_TRUE, "powerup_sticky");
	ResourceManager::LoadTexture("textures/powerup_increase.png", GL_TRUE, "powerup_increase");
	ResourceManager::LoadTexture("textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
	ResourceManager::LoadTexture("textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");
	ResourceManager::LoadTexture("textures/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");
	// 设置渲染器对象
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
	//加载关卡
	GameLevel one; one.Load("levels/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("levels/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("levels/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("levels/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;

	//配置玩家参数
	glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	//配置球的参数
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::GetTexture("face"));

	//配置粒子系统
	Particles = new ParticleGenerator(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		500
	);

	//播放背景音乐
	SoundEngine->play2D("audio/breakout.mp3", GL_TRUE);
}

void Game::Update(GLfloat dt)
{
	// 更新对象
	Ball->Move(dt, this->Width);	//更新球
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2));	//更新粒子
	this->UpdatePowerUps(dt);	//更新道具

	// 检测碰撞
	this->DoCollisions();
	//减少抖动时间
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = GL_FALSE;
	}

	// 球是否接触底部边界？
	if (Ball->Position.y >= this->Height) 
	{
		this->ResetLevel();
		this->ResetPlayer();
	}
}


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// 移动玩家挡板
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0)
			{
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x)
			{
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = GL_FALSE;
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		//开始离屏渲染
		Effects->BeginRender();
		{
			//绘制背景
			Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
			//绘制关卡砖块
			this->Levels[this->Level].Draw(*Renderer);
			//绘制玩家
			Player->Draw(*Renderer);
			//绘制道具
			for (PowerUp& powerUp : this->PowerUps) {
				if (!powerUp.Destroyed)
					powerUp.Draw(*Renderer);
			}
			//绘制粒子
			Particles->Draw();
			//绘制球
			Ball->Draw(*Renderer);
		}
		//结束离屏渲染
		Effects->EndRender();
		//将后处理结果渲染到屏幕上
		Effects->Render(glfwGetTime());
	}
}

void Game::ResetLevel()
{
	if (this->Level == 0)this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 3)
		this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
	//重置玩家和球的状态
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
	//重置所有道具
	Effects->Chaos = Effects->Confuse = GL_FALSE;
	Ball->PassThrough = Ball->Sticky = GL_FALSE;
	Player->Color = glm::vec3(1.0f);
	Ball->Color = glm::vec3(1.0f);
}

void ActivatePowerUp(PowerUp& powerUp)
{
	// 根据道具类型发动道具
	if (powerUp.Type == "speed")
	{
		Ball->Velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->Sticky = GL_TRUE;
		Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		Ball->PassThrough = GL_TRUE;
		Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = GL_TRUE; // 只在chaos未激活时生效，chaos同理
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = GL_TRUE;
	}
}


Direction VectorDirection(glm::vec2 target);

/// <summary>
/// 碰撞检测函数
/// </summary>
/// <param name="one">球</param>
/// <param name="two">砖块</param>
/// <returns>检测结果</returns>
Collision CheckCollision(BallObject& one, GameObject& two) // AABB - Circle collision
{
	// 获取圆的中心 
	glm::vec2 center(one.Position + one.Radius);
	// 计算AABB的信息（中心、半边长）
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);
	// 获取两个中心的差矢量
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);	//用半边长矢量限制中心差矢量
	// AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
	glm::vec2 closest = aabb_center + clamped;
	// 获得圆心center和最近点closest的矢量并判断是否 length <= radius
	difference = closest - center;
	if (glm::length(difference) <= one.Radius){
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	}
	else {
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
	}
}

GLboolean CheckCollision(GameObject& one, GameObject& two) // 方形AABB - AABB碰撞
{
	// X轴碰撞
	GLboolean collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// Y轴碰撞
	GLboolean collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// 两个轴都碰撞时才是真碰撞
	return collisionX && collisionY;
}

/// <summary>
/// 碰撞处理函数
/// </summary>
void Game::DoCollisions()
{
	for (GameObject& box : this->Levels[this->Level].Bricks)
	{
		//砖块碰撞部分
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision)) // 如果collision 是 true
			{
				// 如果砖块不是实心就销毁砖块
				if (!box.IsSolid) {
					box.Destroyed = GL_TRUE;
					this->SpawnPowerUps(box);
					SoundEngine->play2D("audio/bleep.mp3", GL_FALSE);
				}
				else {
					// 如果是实心的砖块则激活shake特效
					ShakeTime = 0.05f;
					Effects->Shake = GL_TRUE;
					SoundEngine->play2D("audio/solid.wav", GL_FALSE);
				}
				// 碰撞处理
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);

				if (!(Ball->PassThrough && !box.IsSolid)) {
					if (dir == LEFT || dir == RIGHT) // 水平方向碰撞
					{
						Ball->Velocity.x = -Ball->Velocity.x; // 反转水平速度
						// 重定位
						GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
						if (dir == LEFT)
							Ball->Position.x += penetration; // 将球右移
						else
							Ball->Position.x -= penetration; // 将球左移
					}
					else // 垂直方向碰撞
					{
						Ball->Velocity.y = -Ball->Velocity.y; // 反转垂直速度
						// 重定位
						GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
						if (dir == UP)
							Ball->Position.y -= penetration; // 将球上移
						else
							Ball->Position.y += penetration; // 将球下移
					}
				}
			}
		}

		//挡板碰撞
		Collision result = CheckCollision(*Ball, *Player);
		if (!Ball->Stuck && std::get<0>(result))
		{
			//检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
			//基于撞击挡板的点与（挡板）中心的距离来改变球的水平速度,撞击点距离挡板的中心点越远，则水平方向的速度就会越大。
			GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
			GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
			GLfloat percentage = distance / (Player->Size.x / 2);	//计算球的中心与挡板中心的距离和挡板的半边长的百分比
			// 依据结果移动
			GLfloat strength = 2.0f;
			glm::vec2 oldVelocity = Ball->Velocity;
			Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
			//新的速度矢量会正交化然后乘以旧速度矢量的长度，这样一来，球的力量和速度将总是一致的，无论它撞击到挡板的哪个地方。
			//如果只是反转y速度那么不同的碰撞位置会产生不同的碰撞速度，例如正中心和45度的夹角不同，导致矢量长度发生变化
			Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);		
			//修复黏板问题，出现的原因是玩家挡板以较高的速度移向球，导致球的中心进入玩家挡板，致使执行了多次碰撞检测，
			//导致y轴速度翻转多次，解决方案是直接返回一个正速度而不是反转y轴速度
			Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
			//道具效果，检测粘附
			Ball->Stuck = Ball->Sticky;

			SoundEngine->play2D("audio/bleep.wav", GL_FALSE);
		}
	}
	//道具更新部分
	for (PowerUp& powerUp : this->PowerUps)
	{
		if (!powerUp.Destroyed)
		{
			if (powerUp.Position.y >= this->Height)
				powerUp.Destroyed = GL_TRUE;
			if (CheckCollision(*Player, powerUp))
			{   // 道具与挡板接触，激活它！
				ActivatePowerUp(powerUp);
				powerUp.Destroyed = GL_TRUE;
				powerUp.Activated = GL_TRUE;
				SoundEngine->play2D("audio/powerup.wav", GL_FALSE);
			}
		}
	}
}

/// <summary>
/// 判断撞击方向
/// </summary>
/// <param name="target">圆心center和最近点closest的矢量</param>
/// <returns>撞击方向</returns>
Direction VectorDirection(glm::vec2 target)
{
	//四个标准方向的矢量，必须与Game.h中定义的枚举变量顺序相同
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),  // 上
		glm::vec2(1.0f, 0.0f),  // 右
		glm::vec2(0.0f, -1.0f), // 下
		glm::vec2(-1.0f, 0.0f)  // 左
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	//用圆心center和最近点closest的矢量点乘四个标准方向的矢量
	//获取最大值，即最接近的标准方向矢量
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);		
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

// 1/chance的概率返回true，(chance-1)/chance的概率返回false
GLboolean ShouldSpawn(GLuint chance)
{
	GLuint random = rand() % chance;
	return random == 0;
}
void Game::SpawnPowerUps(GameObject& block)
{
	if (ShouldSpawn(75)) // 1/75的几率
		this->PowerUps.push_back(
			PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")
			));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(
			PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")
			));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(
			PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_passthrough")
			));
	if (ShouldSpawn(75))
		this->PowerUps.push_back(
			PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")
			));
	if (ShouldSpawn(15)) // 负面道具被更频繁地生成
		this->PowerUps.push_back(
			PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")
			));
	if (ShouldSpawn(15))
		this->PowerUps.push_back(
			PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")
			));
}

//检查是否有同类型的道具仍在激活状态
GLboolean IsOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
	for (const PowerUp& powerUp : powerUps)
	{
		if (powerUp.Activated)
			if (powerUp.Type == type)
				return GL_TRUE;
	}
	return GL_FALSE;
}


//更新道具的位置和持续时间
void Game::UpdatePowerUps(GLfloat dt)
{
	for (PowerUp& powerUp : this->PowerUps)
	{
		powerUp.Position += powerUp.Velocity * dt;
		if (powerUp.Activated)
		{
			powerUp.Duration -= dt;

			if (powerUp.Duration <= 0.0f)
			{
				// 之后会将这个道具移除
				powerUp.Activated = GL_FALSE;
				// 停用效果
				if (powerUp.Type == "sticky")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "sticky"))
					{   // 仅当没有其他sticky效果处于激活状态时重置，以下同理
						Ball->Sticky = GL_FALSE;
						Player->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "pass-through")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "pass-through"))
					{
						Ball->PassThrough = GL_FALSE;
						Ball->Color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "confuse"))
					{
						Effects->Confuse = GL_FALSE;
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(this->PowerUps, "chaos"))
					{
						Effects->Chaos = GL_FALSE;
					}
				}
			}
		}
	}
	//remove_if函数将lamda表达式为true的元素移动至容器的末尾并返回一个迭代器指向应被移除的元素范围的开始部分。容器的erase函数接着擦除这个迭代器指向的元素与容器末尾元素之间的所有元素。
	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const PowerUp& powerUp) { return powerUp.Destroyed && !powerUp.Activated; }
	), this->PowerUps.end());
}