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
	int repeatFrequency;			// 发射频率（单位：ms）
	int projectileDuration;			// 发射物持续时间（单位：ms）
	double hitDamage;				// 发射物造成的伤害
	double speed;					// 发射物速率
	Uint64 lastEmissionTime;		// 上一次发射时间（单位：ms）
	
	ProjectileEmitterComponent(bool isEmit = false, int repeatFrequency = 2000, int projectileDuration = 10000, double hitDamage = 10, double speed = 100, Uint64 lastEmissionTime = 0)
		: isEmit(isEmit), repeatFrequency(repeatFrequency), projectileDuration(projectileDuration), hitDamage(hitDamage), speed(speed), lastEmissionTime(lastEmissionTime) { }
};

#endif // !PROJECTILEEMITTERCOMPONENT