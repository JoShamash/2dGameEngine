#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>
#include <string>

#include "../Logger/Logger.h"

/**
* @struct TransformComponent
* @brief 转换组件，控制坐标、角度、大小
*/
struct TransformComponent 
{
	inline const static std::string NAME = std::string("TransformComponent") + U8_TO_CHARPTR("转换组件"); // 组件名称


	glm::f64vec2 position; // 坐标位置（x, y）
	double rotation;	   // 角度 
	glm::f64vec2 scale;    // 缩放比例（x, y）
	

	/**
	* @brief 构造函数
	* @param position 初始位置，默认为 (0, 0)
	* @param rotation 初始角度，默认为 0
	* @param scale 初始缩放比例，默认为 (1, 1)
	*/
	TransformComponent(glm::f64vec2 position = {0, 0}, double rotation = 0, glm::f64vec2 scale = {1, 1})
		: position(position), rotation(rotation), scale(scale) { }

};

#endif // !TRANSFORMCOMPONENT_H
