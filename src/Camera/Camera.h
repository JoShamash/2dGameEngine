#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct Camera
{
	double x = 0; // 摄像机位置X
	double y = 0; // 摄像机位置Y
	double w = 0; // 摄像机宽度W
	double h = 0; // 摄像机高度H

private:
	double scale = 1.0;			// 摄像机缩放
	double smoothFactor = 1.0;  // 平滑系数
	bool enable_smooth = false; // 是否开启平滑处理

public:
	void enableSmooth(bool enable, double factor = 1.0) 
	{ 
		enable_smooth = enable; 
		smoothFactor = factor;
	}

	bool isSmoothEnabled() const 
	{ 
		return enable_smooth; 
	}

	void setSmoothFactor(double factor) 
	{ 
		smoothFactor = factor; 
		if (smoothFactor < 0.0) smoothFactor = 0.0;
		if (smoothFactor > 1.0) smoothFactor = 1.0;
	}

	double getSmoothFactor() const
	{
		return smoothFactor;
	}

	void setScale(double s) 
	{ 
		scale = s; 
		if (scale < 0.1) scale = 0.1;
		if (scale > 10.0) scale = 10.0;
	}

	double getScale() const 
	{ 
		return scale; 
	}
};

#endif // !CAMERA_H