#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

struct Camera
{
	double x = 0; // �����λ��X
	double y = 0; // �����λ��Y
	double w = 0; // ��������W
	double h = 0; // ������߶�H

private:
	double scale = 1.0;			// ���������
	double smoothFactor = 1.0;  // ƽ��ϵ��
	bool enable_smooth = false; // �Ƿ���ƽ������

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