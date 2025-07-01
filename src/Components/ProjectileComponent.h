#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include <string>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Time/Timer.h"
#include "../Logger/Logger.h"

/**
* @struct ProjectileComponent
* @brief 发射物组件，用于定义发射物的属性
*/
struct ProjectileComponent {
	inline const static std::string NAME = std::string("ProjectileComponent") + U8_TO_CHARPTR("发射物组件"); // 组件名称
			
	Uint64 hitDamage;			// 发射物造成的伤害
	Uint64 duration;			// 发射物持续时间（单位：ms）
	Uint64 createTime;			// 发射物创建时间

	ProjectileComponent(Uint64 duration = 0, Uint64 hitDamage = 0)
		: hitDamage(hitDamage), duration(duration), createTime(Timer::time.gameTime) { }
};

#endif // !PROJECTILECOMPONENT_H
