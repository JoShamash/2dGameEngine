#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

#include <string>

#include "../Logger/Logger.h"

/**
* @struct TransformComponent
* @brief 转换组件，控制坐标、大小、角度
*/
struct TransformComponent 
{
	inline const static std::string NAME = std::string("TransformComponent") + U8_TO_CHARPTR("转换组件"); // 组件名称

	glm::f64vec2 position; // 坐标位置（x, y）
	double rotation;	   // 角度
	glm::f64vec2 scale;    // 缩放比例（x, y）
	

	TransformComponent(glm::f64vec2 position = glm::f64vec2(0, 0), double rotation = 0, glm::f64vec2 scale = glm::f64vec2(1, 1))
		: position(position), rotation(rotation), scale(scale) { }

};

#endif // !TRANSFORMCOMPONENT_H
