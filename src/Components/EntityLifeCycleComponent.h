#ifndef ENTITYLIFECYCLECOMPONENT_H
#define ENTITYLIFECYCLECOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct EntityLifeCycleComponent
* @brief 实体生命周期组件，用于管理实体的生命周期
*/
struct EntityLifeCycleComponent {
	inline const static std::string NAME = std::string("EntityLifeCycleComponent") + U8_TO_CHARPTR("实体生命周期组件"); // 组件名称


	bool isDead = false;			// 实体是否死亡
	bool isDestroyed = false;		// 实体是否被销毁
	bool isOutOfMap = false;		// 实体是否超出地图边界

	Uint64 deathTime = 0;			// 实体死亡时间戳，单位：毫秒
	Uint64 outOfMapTime = 0;		// 实体超出地图边界累计时间，单位：毫秒
	Uint64 destroyTime = 0;			// 实体被销毁时间戳，单位：毫秒

	Uint64 destroyDelay = 0;		// 实体销毁延迟时间，单位：毫秒
	Uint64 outOfMapDelay = 15000;	// 实体超出地图边界销毁延迟时间，单位：毫秒




	/**
	 * @brief 构造函数
	*/
	EntityLifeCycleComponent() = default;
};

#endif // !ENTITYLIFECYCLECOMPONENT_H
