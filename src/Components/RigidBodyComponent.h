#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>
#include <string>

#include "../Logger/Logger.h"

/**
* @struct RigidBodyComponent
* @brief 刚体组件，控制物体的速度
*/
struct RigidBodyComponent
{
	inline const static std::string NAME = std::string("RigidBodyComponent") + U8_TO_CHARPTR("刚体组件"); // 组件名称


	glm::f64vec2 volocity; // 速度，单位：像素/秒，(x，y)正方向为右下角

	/**
	* @brief 构造函数
	* @param volocity 初始速度，默认为(0, 0)
	*/
	RigidBodyComponent(glm::f64vec2 volocity = {0, 0})
		: volocity(volocity) { }
};

#endif // !RIGIDBODYCOMPONENT_H
