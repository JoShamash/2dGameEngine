#if defined(_WIN32) || defined(_WIN64)
#define INIT_LANG system("chcp 65001")
#else
#define INIT_LANG
#endif

#define SDL_MAIN_HANDLED

#include <iostream>

#include "GameEngine/GameEngine.h"

int main(int argc, char* argv[])
{
	INIT_LANG;

	Logger::Instance().Init();

	GameEngine engine;
	engine.Run();
	engine.Destroy();

	return 0;
}