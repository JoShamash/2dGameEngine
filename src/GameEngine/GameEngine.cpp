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
#include "../Components/KeyboardControlComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CampComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/HealthBarComponent.h"
#include "../Components/EntityLifeCycleComponent.h"

#include "../Systems/MovementSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardControlSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileLifeCycleSystem.h"
#include "../Systems/HealthSystem.h"
#include "../Systems/RenderEntitySystem.h"
#include "../Systems/RenderTextSystem.h"
#include "../Systems/RenderHealthBarSystem.h"
#include "../Systems/RenderGUISystem.h"
#include "../Systems/EntityLifeCycleSystem.h"

GameEngine::GameEngine()
{
	std::string message = U8_TO_CHARPTR("创建游戏引擎");
	Logger::Instance().Log(LogLevel::INFO, message);

	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
	eventBus = std::make_unique<EventBus>();

	Entity::SetRegistry(registry.get());
	timer.targetFrameTime = frameTime;
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

	if(TTF_Init() != 0)
	{
		std::string message = U8_TO_CHARPTR("TTF初始化失败：");
		message += TTF_GetError();
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
		SDL_WINDOW_FULLSCREEN_DESKTOP);
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

	if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) != 0) {
		std::string message = U8_TO_CHARPTR("设置渲染器混合模式失败：");
		message += SDL_GetError();
		Logger::Instance().Log(LogLevel::FATAL, message);
		throw std::runtime_error(message);
		return;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	SDL_StartTextInput();
	//SDL_StopTextInput();

	// 1. 创建 ImGui 上下文
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// 2. 设置 ImGui 样式
	ImGui::StyleColorsDark();
	
	// 3. 添加字体（这里是加载 simhei.ttf）
	//io.Fonts->AddFontFromFileTTF("./assets/fonts/simhei.ttf", 18.0f);

	ImFont* font = io.Fonts->AddFontFromFileTTF("./assets/fonts/simhei.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

	// 4. 初始化平台 & 渲染器绑定
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// 可选：立即构建字体贴图（一般不用手动调用，ImGui_ImplXXX_Init 会处理）
	// 手动构建贴图（关键！）
	io.Fonts->Build();
	ImGui_ImplSDLRenderer2_CreateFontsTexture(); // 手动上传贴图
	
	

	camera.x = camera.y = 0;
	camera.w = windowWidth;
	camera.h = windowHeight;

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
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

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
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyboardControlSystem>();
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<ProjectileEmitSystem>();
	registry->AddSystem<ProjectileLifeCycleSystem>();
	registry->AddSystem<HealthSystem>();
	registry->AddSystem<RenderTextSystem>();
	registry->AddSystem<RenderHealthBarSystem>();
	registry->AddSystem<RenderGUISystem>();
	registry->AddSystem<EntityLifeCycleSystem>();

	registry->GetSystem<DamageSystem>().SubscribeEvents(eventBus);
	registry->GetSystem<KeyboardControlSystem>().SubscribeEvents(eventBus);
	registry->GetSystem<ProjectileEmitSystem>().SubscribeEvents(eventBus);

	assetStore->AddImageAsset(renderer, "tank-tiger-right", "./assets/images/tank-tiger-right.png");
	assetStore->AddImageAsset(renderer, "f22", "./assets/images/f22.png");
	assetStore->AddImageAsset(renderer, "chopper", "./assets/images/chopper.png");
	assetStore->AddImageAsset(renderer, "whitechopper_sheet", "./assets/images/chopper-white-spritesheet.png");
	assetStore->AddImageAsset(renderer, "radar", "./assets/images/radar.png");
	assetStore->AddImageAsset(renderer, "bullet", "./assets/images/bullet.png");

	assetStore->AddImageAsset(renderer, "jungle", "./assets/tilemaps/jungle.png");
	assetStore->AddMapStyleAsset("jungle", "./assets/tilemaps/jungle.map");

	assetStore->AddFontAsset("simhei_32", "./assets/fonts/simhei.ttf", 32);
	assetStore->AddFontAsset("simhei_16", "./assets/fonts/simhei.ttf", 16);


	// Read map file
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
			tile.AddGroup("map");

			TransformComponent transformComponent({x * tileSizeX, y * tileSizeY}, 0, {mapStyle->scaleX, mapStyle->scaleY});
			SpriteComponent spriteComponent("jungle", {mapStyle->pixelX, mapStyle->pixelY}, RenderLayer::Terrain, {srcRectX, srcRectY});
			tile.AddComponent<TransformComponent>(transformComponent);
			tile.AddComponent<SpriteComponent>(spriteComponent);
		}
	}

	mapWidth = tileSizeX * mapStyle->rangeX;
	mapHeight = tileSizeY * mapStyle->rangeY;

	// Create tank
	Entity tank = registry->CreateEntity();
	tank.AddGroup("enemy");

	TransformComponent transformComponent({500, 1280});
	RigidBodyComponent rigidBodyComponent(10, {1, 0});
	SpriteComponent spriteComponent("tank-tiger-right", {32, 32}, RenderLayer::Characters);
	BoxColliderComponent boxColliderComponent({32, 32});
	ProjectileEmitterComponent projectileEmitterComponent(true);
	CampComponent campComponent(Camp::Enemy);
	HealthComponent healthComponent;
	HealthBarComponent healthBarComponent("simhei_16");

	tank.AddComponent<TransformComponent>(transformComponent);
	tank.AddComponent<RigidBodyComponent>(rigidBodyComponent);
	tank.AddComponent<SpriteComponent>(spriteComponent);
	tank.AddComponent<BoxColliderComponent>(boxColliderComponent);
	tank.AddComponent<ProjectileEmitterComponent>(projectileEmitterComponent);
	tank.AddComponent<CampComponent>(campComponent);
	tank.AddComponent<HealthComponent>(healthComponent);
	tank.AddComponent<HealthBarComponent>(healthBarComponent);
	tank.AddComponent<EntityLifeCycleComponent>();


	// Create plane
	Entity plane = registry->CreateEntity();
	plane.AddGroup("enemy");

	TransformComponent transformComponent1({600, 500}, 90, {3, 3});
	RigidBodyComponent rigidBodyComponent1(70, {1, 0});
	SpriteComponent spriteComponent1("f22", {32, 32}, RenderLayer::Characters);
	BoxColliderComponent boxColliderComponent1({32, 32});
	CampComponent campComponent1(Camp::Enemy);
	HealthComponent healthComponent1;
	HealthBarComponent healthBarComponent1("simhei_16");

	plane.AddComponent<TransformComponent>(transformComponent1);
	plane.AddComponent<RigidBodyComponent>(rigidBodyComponent1);
	plane.AddComponent<SpriteComponent>(spriteComponent1);
	plane.AddComponent<BoxColliderComponent>(boxColliderComponent1);
	plane.AddComponent<CampComponent>(campComponent1);
	plane.AddComponent<HealthComponent>(healthComponent1);
	plane.AddComponent<HealthBarComponent>(healthBarComponent1);
	plane.AddComponent<EntityLifeCycleComponent>();

	// Create chopper
	Entity chopper = registry->CreateEntity();
	chopper.AddGroup("enemy");

	TransformComponent transformComponent2({1000, 500}, 0, {1, 1});
	RigidBodyComponent rigidBodyComponent2(50, {1, 0});
	SpriteComponent spriteComponent2("chopper", {32, 32}, RenderLayer::Characters);
	AnimationComponent animationComponent2(2, 15);
	BoxColliderComponent boxColliderComponent2({32, 32});
	CampComponent campComponent2(Camp::Enemy);
	HealthComponent healthComponent2;
	HealthBarComponent healthBarComponent2("simhei_16");

	chopper.AddComponent<TransformComponent>(transformComponent2);
	chopper.AddComponent<RigidBodyComponent>(rigidBodyComponent2);
	chopper.AddComponent<SpriteComponent>(spriteComponent2);
	chopper.AddComponent<AnimationComponent>(animationComponent2);
	chopper.AddComponent<BoxColliderComponent>(boxColliderComponent2);
	chopper.AddComponent<CampComponent>(campComponent2);
	chopper.AddComponent<HealthComponent>(healthComponent2);
	chopper.AddComponent<HealthBarComponent>(healthBarComponent2);
	chopper.AddComponent<EntityLifeCycleComponent>();

	// Create radar
	Entity radar = registry->CreateEntity();
	radar.AddGroup("UI");

	TransformComponent transformComponent3({windowWidth - 74, 10}, 0, {1, 1});
	SpriteComponent spriteComponent3("radar", {64, 64}, RenderLayer::UIBackground);
	AnimationComponent animationComponent3(8, 8);

	radar.AddComponent<TransformComponent>(transformComponent3);
	radar.AddComponent<SpriteComponent>(spriteComponent3);
	radar.AddComponent<AnimationComponent>(animationComponent3);

	// Create whiteChopper
	Entity whiteChopper = registry->CreateEntity();
	whiteChopper.AddTag("player");

	TransformComponent transformComponent4({300, 900}, 0, {1, 1});
	RigidBodyComponent rigidBodyComponent4(150, {0, 0});
	SpriteComponent spriteComponent4("whitechopper_sheet", {32, 32}, RenderLayer::Characters);
	AnimationComponent animationComponent4(2, 15);
	BoxColliderComponent boxColliderComponent4({32, 32});
	KeyboardControlComponent keyboardControlComponent4;
	CameraFollowComponent cameraFollowComponent4;
	ProjectileEmitterComponent projectileEmitterComponent4(false, 100);
	CampComponent campComponent4;
	HealthComponent healthComponent4;
	TextLabelComponent textLabelComponent4({1200, 10}, U8_TO_CHARPTR("2D游戏引擎"), "simhei_32", {255, 255, 255, 255}, true);
	HealthBarComponent healthBarComponent4("simhei_16");

	whiteChopper.AddComponent<TransformComponent>(transformComponent4);
	whiteChopper.AddComponent<RigidBodyComponent>(rigidBodyComponent4);
	whiteChopper.AddComponent<SpriteComponent>(spriteComponent4);
	whiteChopper.AddComponent<AnimationComponent>(animationComponent4);
	whiteChopper.AddComponent<BoxColliderComponent>(boxColliderComponent4);
	whiteChopper.AddComponent<KeyboardControlComponent>(keyboardControlComponent4);
	whiteChopper.AddComponent<CameraFollowComponent>(cameraFollowComponent4);
	whiteChopper.AddComponent<ProjectileEmitterComponent>(projectileEmitterComponent4);
	whiteChopper.AddComponent<CampComponent>(campComponent4);
	whiteChopper.AddComponent<HealthComponent>(healthComponent4);
	whiteChopper.AddComponent<TextLabelComponent>(textLabelComponent4);
	whiteChopper.AddComponent<HealthBarComponent>(healthBarComponent4);
	whiteChopper.AddComponent<EntityLifeCycleComponent>();
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
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type)
		{
			case SDL_QUIT:
			{
				isRunning = false;
				break;
			}
			case SDL_KEYDOWN:
			{
				const auto& keyCode = event.key.keysym.sym;
				eventBus->EmitEvent<KeyPressedEvent>(keyCode);
				// 退格键->退出运行
				if (keyCode == SDLK_ESCAPE)
				{
					isRunning = false;
				}
				// F10->进入/退出调试模式
				else if (keyCode == SDLK_F10)
				{
					//registry->GetSystem<DamageSystem>().UnSubscribeEvents(eventBus);
					isDebug = !isDebug;
					if (isDebug)
					{
						Logger::Instance().Log(LogLevel::INFO, U8_TO_CHARPTR("进入调试模式"));
					}
					else
					{
						Logger::Instance().Log(LogLevel::INFO, U8_TO_CHARPTR("退出调试模式"));
					}
				}
				// P键暂停恢复程序
				else if (keyCode == SDLK_p)
				{
					timer.time.paused = !timer.time.paused;
				}
				break;
			}
			case SDL_WINDOWEVENT:
			{
				const auto& windowEvent = event.window.event;
				if (windowEvent == SDL_WINDOWEVENT_FOCUS_LOST || windowEvent == SDL_WINDOWEVENT_MINIMIZED || windowEvent == SDL_WINDOWEVENT_LEAVE)
				{
					timer.pause();
				}
				else if (windowEvent == SDL_WINDOWEVENT_FOCUS_GAINED || windowEvent == SDL_WINDOWEVENT_RESTORED || windowEvent == SDL_WINDOWEVENT_ENTER)
				{
					timer.resume();
				}
				break;
			}
		}
	}
}

void GameEngine::Update()
{
	//auto timeToWait = millisecsPerFrame - (SDL_GetTicks64() - millisecsPreFrame);
	//if (timeToWait > 0 && timeToWait <= millisecsPerFrame)
	//{
	//	SDL_Delay(timeToWait);
	//}
	//auto millisecsCurFrame = SDL_GetTicks64();
	//auto deltaTime = (millisecsCurFrame - millisecsPreFrame) / 1000.0;
	//millisecsPreFrame = millisecsCurFrame; 
	//eventBus->Reset();

	timer.update();

	registry->Update();

	registry->GetSystem<EntityLifeCycleSystem>().Update();
	registry->GetSystem<ProjectileEmitSystem>().Update(registry);
	registry->GetSystem<ProjectileLifeCycleSystem>().Update();
	registry->GetSystem<MovementSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update(eventBus);
	registry->GetSystem<HealthSystem>().Update();
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CameraMovementSystem>().Update(camera);
}

// 确保“清屏→绘制→呈现”整个流程只执行一次。
void GameEngine::Render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 设置背景颜色为黑色
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderEntitySystem>().Update(renderer, assetStore, camera);
	registry->GetSystem<RenderHealthBarSystem>().Update(renderer, assetStore, camera);
	registry->GetSystem<RenderTextSystem>().Update(renderer, assetStore, camera);

	if (isDebug)
	{
		registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);
		registry->GetSystem<RenderGUISystem>().Update(registry, renderer, assetStore, camera);
	}

	SDL_RenderPresent(renderer);
}
