#ifndef EVNETBUS_H
#define EVNETBUS_H

#include <map>
#include <list>
#include <typeindex>

#include "../ECS/ECS.h"
#include "Event.h"

class IEventCallBack
{
public:
	IEventCallBack() = default;
	virtual ~IEventCallBack() = 0;

	void ExecuteEvent(Event& e)
	{
		CallEvent(e);
	}

private:
	virtual void CallEvent(Event& e) = 0;
};

template <class TOwner, class TEvent>
using CallBackFunc = void (TOwner::*)(TEvent&);

template <class TOwner, class TEvent>
class EventCallBack : IEventCallBack
{
public:
	EventCallBack(TOwner* owner, CallBackFunc callbackFunc) 
		: owner(owner), callbackFunc(callbackFunc) { };

	virtual ~EventCallBack() override = default;

private:
	TOwner* owner;
	CallBackFunc<TOwner, TEvent> callbackFunc;

	virtual void CallEvent(Event& e) override
	{
		std::invoke(callbackFunc, owner, static_cast<TEvent>(e));
	}
};

using HandlerList = std::list<std::unique_ptr<IEventCallBack>>;

class EventBus
{
public:
	EventBus()
	{
		std::string message = U8_TO_CHARPTR("创建事件处理总线");
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	~EventBus()
	{
		std::string message = U8_TO_CHARPTR("销毁事件处理总线");
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	template <class TOwner, class TEvent>
	void SubscribeEvent(TOwner* owner, CallBackFunc<TOwner, TEvent> callbackFunc)
	{
		if (!subscribers[typeid(TEvent)])
		{
			subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
		}

		auto subscriber = std::make_unique<EventCallBack<TOwner, TEvent>>(owner, callbackFunc);
		subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
	}
	 
	template <class TEvent, typename ...TArgs>
	void EmitEvent(TArgs&& ...args)
	{
		auto& handlerListPtr = subscribers[typeid(TEvent)].get();
		if (handlerListPtr)
		{
			auto& handlerList = *handlerListPtr;
			for (auto& handlerUniquePtr : handlerList)
			{
				auto& handlerPtr = handlerUniquePtr.get();
				if (handlerPtr)
				{
					auto& handler = *handlerPtr;
					TEvent event(std::forward<TArgs>(args)...);
					handler.ExecuteEvent(event);
				}
			}
		}
	}

private:
	std::map<std::type_index, std::unique_ptr<HandlerList>> subscribers;
};

#endif // !EVNETBUS_H
