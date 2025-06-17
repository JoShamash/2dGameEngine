#include "GameEngine.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderEntitySystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"

GameEngine::GameEngine()
{
	std::string message = U8_TO_CHARPTR("创建游戏引擎");
	Logger::Instance().Log(LogLevel::INFO, message);

	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
}

GameEngine::~GameEngine()
{
	std::string message = U8_TO_CHARPTR("销毁游戏引擎");
	Logger::Instance().Log(LogLevel::INFO, message);
}

void GameEngine::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::string message = U8_TO_CHARPTR("SDL初始化失败：");
		message += SDL_GetError();
		Logger::Instance().Log(LogLevel::FATAL, message);
		throw std::runtime_error(message);
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = displayMode.w;
	windowHeight = displayMode.h;
	window = SDL_CreateWindow(U8_TO_CHARPTR("你好世界"),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		std::string message = U8_TO_CHARPTR("创建SDL窗口失败：");
		message += SDL_GetError();
		Logger::Instance().Log(LogLevel::FATAL, message);
		throw std::runtime_error(message);
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		std::string message = U8_TO_CHARPTR("创建SDL渲染器失败：");
		message += SDL_GetError();
		Logger::Instance().Log(LogLevel::FATAL, message);
		throw std::runtime_error(message);
		return;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	isRunning = true;
}

void GameEngine::Run()
{
	SetUp();
	while (isRunning)
	{
		Input();
		Update();
		Render();
	}
}

void GameEngine::Destroy()
{ 
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void GameEngine::LoadLevel(int level)
{
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderEntitySystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<RenderColliderSystem>();

	assetStore->AddImageAsset(renderer, "tank-tiger-right", "./assets/images/tank-tiger-right.png");
	assetStore->AddImageAsset(renderer, "f22", "./assets/images/f22.png");
	assetStore->AddImageAsset(renderer, "chopper", "./assets/images/chopper.png");
	assetStore->AddImageAsset(renderer, "radar", "./assets/images/radar.png");

	assetStore->AddImageAsset(renderer, "jungle", "./assets/tilemaps/jungle.png");
	assetStore->AddMapStyleAsset("jungle", "./assets/tilemaps/jungle.map");

	MapStyle* mapStyle = assetStore->GetMapStyleAsset("jungle");
	auto tileSizeX = mapStyle->pixelX * mapStyle->scaleX;
	auto tileSizeY = mapStyle->pixelY * mapStyle->scaleY;

	for (size_t y = 0; y < mapStyle->rangeY; ++y)
	{
		for (size_t x = 0; x < mapStyle->rangeX; ++x)
		{
			size_t tileId = mapStyle->map[y][x];
			size_t xPos = tileId % 10;
			size_t yPos = tileId / 10;

			double srcRectX = mapStyle->pixelX * xPos;
			double srcRectY = mapStyle->pixelY * yPos;

			Entity tile = registry->CreateEntity();

			TransformComponent transformComponent({ x * tileSizeX, y * tileSizeY }, 0, { mapStyle->scaleX, mapStyle->scaleY });
			SpriteComponent spriteComponent("jungle", { mapStyle->pixelX, mapStyle->pixelY }, RenderLayer::Terrain, { srcRectX, srcRectY });
			tile.AddComponent<TransformComponent>(transformComponent);
			tile.AddComponent<SpriteComponent>(spriteComponent);
		}
	}

	Entity tank = registry->CreateEntity();
	Entity plane = registry->CreateEntity();
	Entity chopper = registry->CreateEntity();
	Entity radar = registry->CreateEntity();

	TransformComponent transformComponent({500, 500});
	RigidBodyComponent rigidBodyComponent({10, 0});
	SpriteComponent spriteComponent("tank-tiger-right", {32, 32}, RenderLayer::Characters);

	tank.AddComponent<TransformComponent>(transformComponent);
	tank.AddComponent<RigidBodyComponent>(rigidBodyComponent);
	tank.AddComponent<SpriteComponent>(spriteComponent);

	TransformComponent transformComponent1({600, 500}, 90, {3, 3});
	RigidBodyComponent rigidBodyComponent1({100, 0}); 
	SpriteComponent spriteComponent1("f22", {32, 32}, RenderLayer::Characters);
	BoxColliderComponent boxColliderComponent1({96, 96});

	plane.AddComponent<TransformComponent>(transformComponent1);
	plane.AddComponent<RigidBodyComponent>(rigidBodyComponent1);
	plane.AddComponent<SpriteComponent>(spriteComponent1);
	plane.AddComponent<BoxColliderComponent>(boxColliderComponent1);

	TransformComponent transformComponent2({1000, 500}, 0, {1, 1});
	RigidBodyComponent rigidBodyComponent2({50, 0});
	SpriteComponent spriteComponent2("chopper", {32, 32}, RenderLayer::Characters);
	AnimationComponent animationComponent2(2, 15);
	BoxColliderComponent boxColliderComponent2({32, 32});

	chopper.AddComponent<TransformComponent>(transformComponent2);
	chopper.AddComponent<RigidBodyComponent>(rigidBodyComponent2);
	chopper.AddComponent<SpriteComponent>(spriteComponent2);
	chopper.AddComponent<AnimationComponent>(animationComponent2);
	chopper.AddComponent<BoxColliderComponent>(boxColliderComponent2);

	TransformComponent transformComponent3({windowWidth - 74, 10}, 0, {1, 1});
	SpriteComponent spriteComponent3("radar", {64, 64}, RenderLayer::UIBackground);
	AnimationComponent animationComponent3(8, 8);

	radar.AddComponent<TransformComponent>(transformComponent3);
	radar.AddComponent<SpriteComponent>(spriteComponent3);
	radar.AddComponent<AnimationComponent>(animationComponent3);

	tank.KillSelf();
}

void GameEngine::SetUp()
{
	LoadLevel(1);
}

void GameEngine::Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
			}
			else if (event.key.keysym.sym == SDLK_d)
			{
				isDebug = !isDebug;
			}
			break;
		}
	}
}

void GameEngine::Update()
{
	auto timeToWait = millisecsPerFrame - (SDL_GetTicks64() - millisecsPreFrame);
	if (timeToWait > 0 && timeToWait <= millisecsPerFrame)
	{
		SDL_Delay(timeToWait);
	}
	auto millisecsCurFrame = SDL_GetTicks64();
	auto deltaTime = (millisecsCurFrame - millisecsPreFrame) / 1000.0;
	millisecsPreFrame = millisecsCurFrame;

	registry->Update();

	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<CollisionSystem>().Update();
	registry->GetSystem<AnimationSystem>().Update();
}

// 确保“清屏→绘制→呈现”整个流程只执行一次。
void GameEngine::Render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 设置背景颜色为黑色
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderEntitySystem>().Update(renderer, assetStore);

	if (isDebug)
	{
		registry->GetSystem<RenderColliderSystem>().Update(renderer);
	}

	SDL_RenderPresent(renderer);
}
