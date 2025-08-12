#ifndef KEYRELEASEDEVENT_H
#define KEYRELEASEDEVENT_H

#include <SDL2/SDL_keycode.h>

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

// 按键松开事件，定义一系列数据，当按键松开时触发
class KeyReleasedEvent : public Event
{
public:
	/**
	* @breif 构造函数
	* @param key 按键代码
	*/
	KeyReleasedEvent(const SDL_Keycode& key)
		: key(key) {
	}

	const SDL_Keycode& key; // 按键代码
};


#endif // !KEYRELEASEDEVENT_H
