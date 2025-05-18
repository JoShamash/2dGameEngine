#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>

#include <string>

/**
* @struct RigidBodyComponent
* @brief �������������������ٶ�
*/
struct RigidBodyComponent
{
	inline const static std::string NAME = std::string("RigidBodyComponent") + reinterpret_cast<const char*>(u8"�������"); // �������

	glm::f64vec2 volocity; // �ٶ�

	RigidBodyComponent(glm::f64vec2 volocity = glm::f64vec2(0, 0)) :
		volocity(volocity){ }
};

#endif // !RIGIDBODYCOMPONENT_H
