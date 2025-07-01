#ifndef PROJECTILECOMPONENT_H
#define PROJECTILECOMPONENT_H

#include <string>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Time/Timer.h"
#include "../Logger/Logger.h"

/**
* @struct ProjectileComponent
* @brief ��������������ڶ��巢���������
*/
struct ProjectileComponent {
	inline const static std::string NAME = std::string("ProjectileComponent") + U8_TO_CHARPTR("���������"); // �������
			
	Uint64 hitDamage;			// ��������ɵ��˺�
	Uint64 duration;			// ���������ʱ�䣨��λ��ms��
	Uint64 createTime;			// �����ﴴ��ʱ��

	ProjectileComponent(Uint64 duration = 0, Uint64 hitDamage = 0)
		: hitDamage(hitDamage), duration(duration), createTime(Timer::time.gameTime) { }
};

#endif // !PROJECTILECOMPONENT_H
