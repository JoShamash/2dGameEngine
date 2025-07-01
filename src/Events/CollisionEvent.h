#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

// 碰撞事件，定义一系列数据，当两实体发生碰撞时触发
class CollisionEvent : public Event
{
public:
	/**
	* @breif 构造函数
	* @param a 实体a
	* @param b 实体b
	*/
	CollisionEvent(const Entity &a, const Entity &b) 
		: a(a), b(b) { }

	const Entity& a; // 实体a
	const Entity& b; // 实体b
};

#endif // !COLLISIONEVENT_H
