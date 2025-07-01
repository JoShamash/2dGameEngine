#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include <SDL2/SDL_keycode.h>

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

// 按键按下事件，定义一系列数据，当按键按下时触发
class KeyPressedEvent : public Event
{
public:
	/**
	* @breif 构造函数
	* @param key 按键代码
	*/
	KeyPressedEvent(const SDL_Keycode& key)
		: key(key) { }

	const SDL_Keycode& key; // 按键代码
};

#endif // !KEYPRESSEDEVENT_H
