#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <SDL2/SDL.h>
#include <string>

#include "../Logger/Logger.h"

/**
* @struct RigidBodyComponent
* @brief �������������������ٶ�
*/
struct RigidBodyComponent
{
	inline const static std::string NAME = std::string("RigidBodyComponent") + U8_TO_CHARPTR("�������"); // �������


	double speed;			// ���ʣ���λ������/��
	double maxSpeed;		// ������ʣ���λ������/��	
	glm::f64vec2 velocity;	// �ٶȱ�׼��ʸ������
	

	/**
	* @brief ���캯��
	* @param speed 
	* @param velocity ��ʼ�ٶȱ�׼��ʸ����Ĭ��Ϊ(��2/2, ��2/2)��ʼ�ձ�֤velocity.x^2+velocity.y^2=1
	*/
	RigidBodyComponent(double speed = 0, glm::f64vec2 velocity = { glm::one_over_root_two<double>(), glm::one_over_root_two<double>() })
		: speed(speed), maxSpeed(speed), velocity(velocity) { }
};

#endif // !RIGIDBODYCOMPONENT_H
