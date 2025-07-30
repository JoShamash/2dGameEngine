#ifndef PROJECTILEEMITTERCOMPONENT
#define PROJECTILEEMITTERCOMPONENT

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct ProjectileEmitterComponent
* @brief ��������������ڶ��巢�������Ա㴴��������
*/
struct ProjectileEmitterComponent
{
	inline const static std::string NAME = std::string("ProjectileEmitterComponent") + U8_TO_CHARPTR("���������"); // �������


	bool   isEmit;					// �Ƿ������书��
	int repeatFrequency;			// ����Ƶ�ʣ���λ��ms��
	int projectileDuration;			// ���������ʱ�䣨��λ��ms��
	double hitDamage;				// ��������ɵ��˺�
	double speed;					// ����������
	Uint64 lastEmissionTime;		// ��һ�η���ʱ�䣨��λ��ms��
	
	ProjectileEmitterComponent(bool isEmit = false, int repeatFrequency = 2000, int projectileDuration = 10000, double hitDamage = 10, double speed = 100, Uint64 lastEmissionTime = 0)
		: isEmit(isEmit), repeatFrequency(repeatFrequency), projectileDuration(projectileDuration), hitDamage(hitDamage), speed(speed), lastEmissionTime(lastEmissionTime) { }
};

#endif // !PROJECTILEEMITTERCOMPONENT