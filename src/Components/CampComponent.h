#ifndef CAMPCOMPONENT_H
#define CAMPCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

enum class Camp : Uint8
{
	Friend = 0,		// 友方
	Enemy,			// 敌方
	Neutral,		// 中立方

	Count
};

/**
* @struct CampComponent
* @brief 阵营组件，用于定义阵营属性
*/
struct CampComponent {
	inline const static std::string NAME = std::string("CampComponent") + U8_TO_CHARPTR("阵营组件"); // 组件名称


	Camp camp;	// 阵营

	/**
	 * @brief 构造函数
	 * @param Camp	阵营
	*/
	CampComponent(Camp camp = Camp::Friend)
		: camp(camp) { }
};

#endif // !CAMPCOMPONENT_H
