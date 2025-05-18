#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>

#include <string>

/**
* @struct RigidBodyComponent
* @brief 刚体组件，控制物体的速度
*/
struct RigidBodyComponent
{
	inline const static std::string NAME = std::string("RigidBodyComponent") + reinterpret_cast<const char*>(u8"刚体组件"); // 组件名称

	glm::f64vec2 volocity; // 速度

	RigidBodyComponent(glm::f64vec2 volocity = glm::f64vec2(0, 0)) :
		volocity(volocity){ }
};

#endif // !RIGIDBODYCOMPONENT_H
