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
	Uint64 repeatFrequency;			// ����Ƶ�ʣ���λ��ms��
	Uint64 projectileDuration;		// ���������ʱ�䣨��λ��ms��
	Uint64 hitDamage;				// ��������ɵ��˺�
	Uint64 speed;					// ����������
	Uint64 lastEmissionTime;		// ��һ�η���ʱ�䣨��λ��ms��
	
	ProjectileEmitterComponent(bool isEmit = false, Uint64 repeatFrequency = 2000, Uint64 projectileDuration = 10000, Uint64 hitDamage = 20, Uint64 speed = 100, Uint64 lastEmissionTime = 0)
		: isEmit(isEmit), repeatFrequency(repeatFrequency), projectileDuration(projectileDuration), hitDamage(hitDamage), speed(speed), lastEmissionTime(lastEmissionTime) { }
};

#endif // !PROJECTILEEMITTERCOMPONENT