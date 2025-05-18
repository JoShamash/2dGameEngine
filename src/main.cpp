#if defined(_WIN32) || defined(_WIN64)
#define INIT_LANG system("chcp 65001")
#elif
#define INIT_LANG
#endif

#define SDL_MAIN_HANDLED

#include "GameEngine/GameEngine.h"
#include <iostream>

int main(int argc, char* argv[])
{
	INIT_LANG;

	Logger::Instance().Init();

	GameEngine engine;
	engine.Initialize();
	engine.Run();
	engine.Destroy();

	return 0;
}