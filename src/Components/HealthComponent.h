#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

// ����ֵ�ȼ�ö�٣�0-3 �ֱ��ʾΣ�ա��ϵ͡����úͽ���
enum class HealthLevel : Uint16
{
	Danger = 0,		// ����ֵΣ��
	Low,			// ����ֵ�ϵ�
	Good,			// ����ֵ����
	Healthy			// ����ֵ����
};

/**
* @struct HealthComponent
* @brief ����ֵ��������ڶ�������ֵ����
*/
struct HealthComponent {
	inline const static std::string NAME = std::string("HealthComponent") + U8_TO_CHARPTR("����ֵ���"); // �������


	double hp;		// ����ֵ
	double maxHp;	// �������ֵ

	/**
	* @brief ���캯��
	* @param hp �������ֵ��Ĭ��Ϊ100
	*/
	HealthComponent(double maxHp = 100.0)
		: hp(maxHp), maxHp(hp) { }
};

#endif // !HEALTHCOMPONENT_H
