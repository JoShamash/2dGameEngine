#ifndef PROJECTILEEMITTERCOMPONENT
#define PROJECTILEEMITTERCOMPONENT

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct ProjectileEmitterComponent
* @brief 发射器组件，用于定义发射器，以便创建发射物
*/
struct ProjectileEmitterComponent
{
	inline const static std::string NAME = std::string("ProjectileEmitterComponent") + U8_TO_CHARPTR("发射器组件"); // 组件名称


	bool   isEmit;					// 是否开启发射功能
	Uint64 repeatFrequency;			// 发射频率（单位：ms）
	Uint64 projectileDuration;		// 发射物持续时间（单位：ms）
	Uint64 hitDamage;				// 发射物造成的伤害
	Uint64 speed;					// 发射物速率
	Uint64 lastEmissionTime;		// 上一次发射时间（单位：ms）
	
	ProjectileEmitterComponent(bool isEmit = false, Uint64 repeatFrequency = 2000, Uint64 projectileDuration = 10000, Uint64 hitDamage = 20, Uint64 speed = 100, Uint64 lastEmissionTime = 0)
		: isEmit(isEmit), repeatFrequency(repeatFrequency), projectileDuration(projectileDuration), hitDamage(hitDamage), speed(speed), lastEmissionTime(lastEmissionTime) { }
};

#endif // !PROJECTILEEMITTERCOMPONENT