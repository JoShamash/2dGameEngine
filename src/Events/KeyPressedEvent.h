#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include <SDL2/SDL_keycode.h>

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

// ���������¼�������һϵ�����ݣ�����������ʱ����
class KeyPressedEvent : public Event
{
public:
	/**
	* @breif ���캯��
	* @param key ��������
	*/
	KeyPressedEvent(const SDL_Keycode& key)
		: key(key) { }

	const SDL_Keycode& key; // ��������
};

#endif // !KEYPRESSEDEVENT_H
