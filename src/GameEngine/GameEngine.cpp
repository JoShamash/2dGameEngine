#include "GameEngine.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"

GameEngine::GameEngine()
{
	std::string message = reinterpret_cast<const char*>(u8"创建游戏引擎");
	Logger::Instance().Log(LogLevel::INFO, message);

	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
}

GameEngine::~GameEngine()
{
	std::string message = reinterpret_cast<const char*>(u8"销毁游戏引擎");
	Logger::Instance().Log(LogLevel::INFO, message);
}

void GameEngine::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::string message = reinterpret_cast<const char*>(u8"SDL初始化失败：");
		message += SDL_GetError();
		Logger::Instance().Log(LogLevel::ERROR, message);
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = displayMode.w;
	windowHeight = displayMode.h;
	window = SDL_CreateWindow(reinterpret_cast<const char*>(u8"你好世界"),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		std::string message = reinterpret_cast<const char*>(u8"创建SDL窗口失败：");
		message += SDL_GetError();
		Logger::Instance().Log(LogLevel::ERROR, message);
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		std::string message = reinterpret_cast<const char*>(u8"创建SDL渲染器失败：");
		message += SDL_GetError();
		Logger::Instance().Log(LogLevel::ERROR, message);
		return;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	isRunning = true;
}

void GameEngine::Destroy()
{ 
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void GameEngine::ProcessInput()
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
			break;
		}
	}
}

void GameEngine::LoadLevel(int level)
{
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();

	assetStore->AddImageAsset(renderer, "tank-tiger-right", "./assets/images/tank-tiger-right.png");
	assetStore->AddImageAsset(renderer, "f22", "./assets/images/f22.png");
	assetStore->AddImageAsset(renderer, "jungle", "./assets/tilemaps/jungle.png");
	assetStore->AddMapStyleAsset("jungle", "./assets/tilemaps/jungle.map");

	MapStyle* mapStyle = assetStore->GetMapStyleAsset("jungle");
	double tileSizeX = mapStyle->pixelX * mapStyle->scaleX;
	double tileSizeY = mapStyle->pixelY * mapStyle->scaleY;

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
			SpriteComponent spriteComponent("jungle", { mapStyle->pixelX, mapStyle->pixelY }, { srcRectX, srcRectY });
			tile.AddComponent<TransformComponent>(transformComponent);
			tile.AddComponent<SpriteComponent>(spriteComponent);
		}
	}

	Entity tank = registry->CreateEntity();
	Entity plane = registry->CreateEntity();

	TransformComponent transformComponent({ 500, 500 });
	RigidBodyComponent rigidBodyComponent({ 10, 20 });
	SpriteComponent spriteComponent("tank-tiger-right", { 32, 32 });

	tank.AddComponent<TransformComponent>(transformComponent);
	tank.AddComponent<RigidBodyComponent>(rigidBodyComponent);
	tank.AddComponent<SpriteComponent>(spriteComponent);

	TransformComponent transformComponent1({ 200, 200 }, 90, { 3, 3 });
	RigidBodyComponent rigidBodyComponent1({ 100, 0 });
	SpriteComponent spriteComponent1("f22", { 32, 32 });

	plane.AddComponent<TransformComponent>(transformComponent1);
	plane.AddComponent<RigidBodyComponent>(rigidBodyComponent1);
	plane.AddComponent<SpriteComponent>(spriteComponent1);
}

void GameEngine::SetUp()
{
	LoadLevel(1);
}

void GameEngine::Update()
{
	Uint64 timeToWait = millisecsPerFrame - (SDL_GetTicks64() - millisecsPreFrame);
	if (timeToWait > 0 && timeToWait <= millisecsPerFrame)
	{
		SDL_Delay(timeToWait);
	}
	double deltaTime = (SDL_GetTicks64() - millisecsPreFrame)/1000.0;
	millisecsPreFrame = SDL_GetTicks64();

	registry->Update();

	registry->GetSystem<MovementSystem>().Update(deltaTime);
}

void GameEngine::Run()
{
	SetUp();
	while (isRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void GameEngine::Render()
{
	//static Uint8 r = 0, g = 0, b = 0;
	//static bool flag = false;
	//if (r == 0 || r == 255) flag = !flag;
	//if (flag)
	//	SDL_SetRenderDrawColor(renderer, r++, g++, b++, 255);
	//else
	//	SDL_SetRenderDrawColor(renderer, r--, g--, b--, 255);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 设置背景颜色为黑色
	SDL_RenderClear(renderer);

	//SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	//SDL_FreeSurface(surface);
	//SDL_Rect dstRect = { playerPositon.x, playerPositon.y, 35, 35 };
	//SDL_Rect dstRect1 = { 100, 100, 35, 35 };
	//SDL_RenderCopy(renderer, texture, NULL, &dstRect);
	//SDL_RenderCopy(renderer, texture, NULL, &dstRect1);
	//SDL_DestroyTexture(texture);

	registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

	SDL_RenderPresent(renderer);
}
