#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <SDL2/SDL.h>
#include <string>

#include "../Logger/Logger.h"

/**
* @struct RigidBodyComponent
* @brief 刚体组件，控制物体的速度
*/
struct RigidBodyComponent
{
	inline const static std::string NAME = std::string("RigidBodyComponent") + U8_TO_CHARPTR("刚体组件"); // 组件名称


	double speed;			// 速率，单位：像素/秒
	double maxSpeed;		// 最大速率，单位：像素/秒	
	glm::f64vec2 velocity;	// 速度标准化矢量方向
	

	/**
	* @brief 构造函数
	* @param speed 
	* @param velocity 初始速度标准化矢量，默认为(√2/2, √2/2)，始终保证velocity.x^2+velocity.y^2=1
	*/
	RigidBodyComponent(double speed = 0, glm::f64vec2 velocity = { glm::one_over_root_two<double>(), glm::one_over_root_two<double>() })
		: speed(speed), maxSpeed(speed), velocity(velocity) { }
};

#endif // !RIGIDBODYCOMPONENT_H
