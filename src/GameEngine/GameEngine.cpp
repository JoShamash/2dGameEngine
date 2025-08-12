#include "GameEngine.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include <sol/sol.hpp>

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/CampComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
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
	Initialize();

	std::string message = U8_TO_CHARPTR("创建游戏引擎");
	Logger::Instance().Log(LogLevel::INFO, message);

	luaState = std::make_unique<sol::state>();
	luaState->open_libraries(sol::lib::base, sol::lib::os, sol::lib::package, sol::lib::string, sol::lib::math);

	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
	eventBus = std::make_unique<EventBus>();

	levelLoader = std::make_unique<LevelLoader>(*luaState, registry, assetStore, renderer);

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

void GameEngine::SetUp()
{
	// 添加系统
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

	// 订阅事件
	registry->GetSystem<DamageSystem>().SubscribeEvents(eventBus);
	registry->GetSystem<KeyboardControlSystem>().SubscribeEvents(eventBus);
	registry->GetSystem<ProjectileEmitSystem>().SubscribeEvents(eventBus);
	registry->GetSystem<MovementSystem>().SubscribeEvents(eventBus);

	levelLoader->LoadLevel(1);
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
