#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct HealthComponent
* @brief 生命值组件，用于定义生命值属性
*/
struct HealthComponent {
	inline const static std::string NAME = std::string("HealthComponent") + U8_TO_CHARPTR("生命值组件"); // 组件名称


	Uint64 hp; // 生命值

	/**
	* @brief 构造函数
	* @param group	阵营
	*/
	HealthComponent(Uint64 hp = 100)
		: hp(hp) { }
};

#endif // !HEALTHCOMPONENT_H
