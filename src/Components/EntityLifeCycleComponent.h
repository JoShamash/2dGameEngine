#ifndef ENTITYLIFECYCLECOMPONENT_H
#define ENTITYLIFECYCLECOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct EntityLifeCycleComponent
* @brief ʵ������������������ڹ���ʵ�����������
*/
struct EntityLifeCycleComponent {
	inline const static std::string NAME = std::string("EntityLifeCycleComponent") + U8_TO_CHARPTR("ʵ�������������"); // �������


	bool isDead = false;			// ʵ���Ƿ�����
	bool isDestroyed = false;		// ʵ���Ƿ�����
	bool isOutOfMap = false;		// ʵ���Ƿ񳬳���ͼ�߽�

	Uint64 deathTime = 0;			// ʵ������ʱ�������λ������
	Uint64 outOfMapTime = 0;		// ʵ�峬����ͼ�߽��ۼ�ʱ�䣬��λ������
	Uint64 destroyTime = 0;			// ʵ�屻����ʱ�������λ������

	Uint64 destroyDelay = 0;		// ʵ�������ӳ�ʱ�䣬��λ������
	Uint64 outOfMapDelay = 15000;	// ʵ�峬����ͼ�߽������ӳ�ʱ�䣬��λ������




	/**
	 * @brief ���캯��
	*/
	EntityLifeCycleComponent() = default;
};

#endif // !ENTITYLIFECYCLECOMPONENT_H
