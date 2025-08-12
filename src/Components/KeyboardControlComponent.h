#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct KeyboardControlComponent
* @brief 键盘控制组件，用于处理键盘控制实体的移动速度方向，常常和RigidBodyComponent联合使用获得最终速度矢量。
*/
struct KeyboardControlComponent {
	inline const static std::string NAME = std::string("KeyboardControlComponent") + U8_TO_CHARPTR("键盘控制组件"); // 组件名称

	glm::f64vec2 upVelocity = {0, -1};		// 向上↑移动的速度方向
	glm::f64vec2 rightVelocity = {1, 0};	// 向右→移动的速度方向
	glm::f64vec2 downVelocity = {0, 1};		// 向下↓移动的速度方向
	glm::f64vec2 leftVelocity = {-1, 0};	// 向左←移动的速度方向

	bool isFire = false;					// 空格控制是否开火
	bool isMove = false;					// 是否移动


	/**
	 * @brief 构造函数
	 * @param upVelocity	↑移动速度方向	 
	 * @param rightVelocity →移动速度方向
	 * @param downVelocity  ↓移动速度方向
	 * @param leftVelocity  ←移动速度方向
	 * @param isFire		空格控制是否开火
	 */
	KeyboardControlComponent() = default;
};

#endif // !KEYBOARDCONTROLCOMPONENT_H
