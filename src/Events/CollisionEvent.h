#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

// ��ײ�¼�������һϵ�����ݣ�����ʵ�巢����ײʱ����
class CollisionEvent : public Event
{
public:
	/**
	* @breif ���캯��
	* @param a ʵ��a
	* @param b ʵ��b
	*/
	CollisionEvent(const Entity &a, const Entity &b) 
		: a(a), b(b) { }

	const Entity& a; // ʵ��a
	const Entity& b; // ʵ��b
};

#endif // !COLLISIONEVENT_H
