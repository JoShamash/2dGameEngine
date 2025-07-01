#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct HealthComponent
* @brief ����ֵ��������ڶ�������ֵ����
*/
struct HealthComponent {
	inline const static std::string NAME = std::string("HealthComponent") + U8_TO_CHARPTR("����ֵ���"); // �������


	Uint64 hp; // ����ֵ

	/**
	* @brief ���캯��
	* @param group	��Ӫ
	*/
	HealthComponent(Uint64 hp = 100)
		: hp(hp) { }
};

#endif // !HEALTHCOMPONENT_H
