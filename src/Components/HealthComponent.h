#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

// 生命值等级枚举，0-3 分别表示危险、较低、良好和健康
enum class HealthLevel : Uint16
{
	Danger = 0,		// 生命值危险
	Low,			// 生命值较低
	Good,			// 生命值良好
	Healthy			// 生命值健康
};

/**
* @struct HealthComponent
* @brief 生命值组件，用于定义生命值属性
*/
struct HealthComponent {
	inline const static std::string NAME = std::string("HealthComponent") + U8_TO_CHARPTR("生命值组件"); // 组件名称


	double hp;		// 生命值
	double maxHp;	// 最大生命值

	/**
	* @brief 构造函数
	* @param hp 最大生命值，默认为100
	*/
	HealthComponent(double maxHp = 100.0)
		: hp(maxHp), maxHp(hp) { }
};

#endif // !HEALTHCOMPONENT_H
