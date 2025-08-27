#ifndef KEYRELEASEDEVENT_H
#define KEYRELEASEDEVENT_H

#include <SDL2/SDL_keycode.h>

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

// �����ɿ��¼�������һϵ�����ݣ��������ɿ�ʱ����
class KeyReleasedEvent : public Event
{
public:
	/**
	* @breif ���캯��
	* @param key ��������
	*/
	KeyReleasedEvent(const SDL_Keycode& key)
		: key(key) {
	}

	const SDL_Keycode& key; // ��������
};


#endif // !KEYRELEASEDEVENT_H
