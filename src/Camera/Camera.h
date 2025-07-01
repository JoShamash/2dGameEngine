#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

struct Camera
{
	double x; // 摄像机位置X
	double y; // 摄像机位置Y
	double w; // 摄像机宽度
	double h; // 摄像机高度
};

#endif // !CAMERA_H