#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <glm/glm.hpp>
#include <sol/forward.hpp>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"
#include "../Camera/Camera.h"
#include "../Time/Timer.h"
#include "../LevelLoader/LevelLoader.h"

class GameEngine
{
public:
	GameEngine();
	~GameEngine();

	void Run();
	void Destroy();

private:
	void Initialize();
	void SetUp();

	void Input();
	void Update();
	void Render();
	
private:
	std::unique_ptr<sol::state> luaState;		// Lua状态机
	std::unique_ptr<Registry> registry;			// 实体组件注册表
	std::unique_ptr<AssetStore> assetStore;		// 资源存储器
	std::unique_ptr<EventBus> eventBus;			// 事件总线
	std::unique_ptr<LevelLoader> levelLoader;	// 关卡加载器

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	Camera camera = {};
	Timer timer = {};

	bool isRunning = true;
	bool isPause = false;
	bool isResume = false;

	Uint64 fps = 60;
	double frameTime = 1000 / fps;

	bool isDebug = false;

public:
	inline static Uint64 windowWidth = 0;
	inline static Uint64 windowHeight = 0;
	inline static Uint64 mapWidth = 0;
	inline static Uint64 mapHeight = 0;
};


#endif // !GAMEENGINE_H
