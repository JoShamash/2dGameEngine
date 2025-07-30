#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct BoxColliderComponent
* @brief 碰撞组件，用于处理实体的碰撞检测和位置更新。
*/
struct BoxColliderComponent {
	inline const static std::string NAME = std::string("BoxColliderComponent") + U8_TO_CHARPTR("盒体碰撞组件"); // 组件名称
	                  
	
	glm::f64vec2 size;			// 碰撞盒大小，x为宽度，y为高度  
	glm::f64vec2 offset;		// 偏移量
	bool isCollision = false;	// 是否发生碰撞


	/**
	 * @brief 构造函数
	 * @param size 碰撞盒大小，默认为(0, 0)
	 * @param offset 偏移量，默认为(0, 0)
	 */
	BoxColliderComponent(const glm::f64vec2& size = {0, 0}, const glm::f64vec2& offset = {0, 0})
		: size(size), offset(offset) { }
};

#endif // !BOXCOLLIDERCOMPONENT_H
