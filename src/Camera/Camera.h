#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

struct Camera
{
	Sint64 x; // �����λ��X
	Sint64 y; // �����λ��Y
	Sint64 w; // ��������W
	Sint64 h; // ������߶�H
};

#endif // !CAMERA_H