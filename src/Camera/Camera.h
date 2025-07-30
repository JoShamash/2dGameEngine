#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

struct Camera
{
	Sint64 x; // 摄像机位置X
	Sint64 y; // 摄像机位置Y
	Sint64 w; // 摄像机宽度W
	Sint64 h; // 摄像机高度H
};

#endif // !CAMERA_H