#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <glm/glm.hpp>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Logger/Logger.h"

class GameEngine
{
public:
	GameEngine();
	~GameEngine();
	void Initialize();
	void Run();
	void Destroy();

private:
	void LoadLevel(int level);
	void SetUp();

	void ProcessInput();
	void Update();
	void Render();
	
private:
	std::unique_ptr<Registry> registry;		// 实体组件注册表
	std::unique_ptr<AssetStore> assetStore; // 资源存储器

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	bool isRunning = true;
	Uint64 windowWidth = 0;
	Uint64 windowHeight = 0;

	Uint64 fps = 120;
	Uint64 millisecsPreFrame = 0;
	Uint64 millisecsPerFrame = 1000 / 120;
};


#endif // !GAMEENGINE_H
