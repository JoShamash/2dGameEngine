#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>
#include <string>

#include "../Logger/Logger.h"

/**
* @struct RigidBodyComponent
* @brief �������������������ٶ�
*/
struct RigidBodyComponent
{
	inline const static std::string NAME = std::string("RigidBodyComponent") + U8_TO_CHARPTR("�������"); // �������


	glm::f64vec2 volocity; // �ٶȣ���λ������/�룬(x��y)������Ϊ���½�

	/**
	* @brief ���캯��
	* @param volocity ��ʼ�ٶȣ�Ĭ��Ϊ(0, 0)
	*/
	RigidBodyComponent(glm::f64vec2 volocity = {0, 0})
		: volocity(volocity) { }
};

#endif // !RIGIDBODYCOMPONENT_H
