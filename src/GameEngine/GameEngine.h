#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_gfx/SDL2_gfxPrimitives.h>
#include <glm/glm.hpp>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"
#include "../Camera/Camera.h"
#include "../Time/Timer.h"

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

	void Input();
	void Update();
	void Render();
	
private:
	std::unique_ptr<Registry> registry;		// ʵ�����ע���
	std::unique_ptr<AssetStore> assetStore; // ��Դ�洢��
	std::unique_ptr<EventBus> eventBus;		// �¼�����

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
