#ifndef EVNETBUS_H
#define EVNETBUS_H

#include <map>
#include <unordered_set>
#include <typeindex>

#include "../ECS/ECS.h"
#include "Event.h"

//   _____                          _      ____           _   _   ____                   _    
//  | ____| __   __   ___   _ __   | |_   / ___|   __ _  | | | | | __ )    __ _    ___  | | __
//  |  _|   \ \ / /  / _ \ | '_ \  | __| | |      / _` | | | | | |  _ \   / _` |  / __| | |/ /
//  | |___   \ V /  |  __/ | | | | | |_  | |___  | (_| | | | | | | |_) | | (_| | | (__  |   < 
//  |_____|   \_/    \___| |_| |_|  \__|  \____|  \__,_| |_| |_| |____/   \__,_|  \___| |_|\_\
                                                                                            
// 事件回调类的接口父类
class IEventCallBack
{
public:
	IEventCallBack() = default;
	virtual ~IEventCallBack() = default;

	// 触发事件后执行回调函数
	void ExecuteEvent(Event& e)
	{
		CallBackEvent(e);
	}

private:
	// 纯虚函数，事件的回调函数
	virtual void CallBackEvent(Event& e) = 0;
};

// CallBackFunc类型，TOwner类的函数指针，函数参数为TEvent&
template <class TEvent, class TOwner>
using CallBackFunc = void (TOwner::*)(TEvent&);

// 具体事件回调类
template <class TOwner, class TEvent>
class EventCallBack : public IEventCallBack
{
public:
	EventCallBack(TOwner* owner, CallBackFunc<TEvent, TOwner> callbackFunc)
		: owner(owner), callbackFunc(callbackFunc) { };

	virtual ~EventCallBack() override = default;

	inline TOwner* GetOwner()
	{
		return owner;
	}

	inline CallBackFunc<TEvent, TOwner> GetCallbackFunc()
	{
		return callbackFunc;
	}

private:
	TOwner* owner;								// 具体调用类
	CallBackFunc<TEvent, TOwner> callbackFunc;	// 调用类的回调函数

	// 调用owner的回调函数callbackFunc
	virtual void CallBackEvent(Event& event) override
	{
		std::invoke(callbackFunc, owner, static_cast<TEvent&>(event));
	}
};

// HandlerList类型，负责维护所有事件回调类列表
using Handlers = std::vector<std::unique_ptr<IEventCallBack>>;

//   _____                          _     ____                
//  | ____| __   __   ___   _ __   | |_  | __ )   _   _   ___ 
//  |  _|   \ \ / /  / _ \ | '_ \  | __| |  _ \  | | | | / __|
//  | |___   \ V /  |  __/ | | | | | |_  | |_) | | |_| | \__ \
//  |_____|   \_/    \___| |_| |_|  \__| |____/   \__,_| |___/
                                       
// 事件总线，负责事件的订阅与触发事件后分发
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

	// 重置事件总线，清空所有订阅者
	void Reset()
	{
		subscribers.clear();
	}

	// 订阅事件，owner为订阅者对象，callbackFunc为回调函数
	template <class TEvent, class TOwner>
	void SubscribeEvent(TOwner* owner, CallBackFunc<TEvent, TOwner> callbackFunc)
	{
		// 如果该事件类型还没有订阅者列表，则创建一个新的列表
		if (!subscribers[typeid(TEvent)].get())
		{
			subscribers[typeid(TEvent)] = std::make_unique<Handlers>();
		}

		// 创建事件回调对象并添加到订阅者列表
		auto subscriber = std::make_unique<EventCallBack<TOwner, TEvent>>(owner, callbackFunc);
		subscribers[typeid(TEvent)]->emplace_back(std::move(subscriber));
	}

	// 取消订阅事件，owner为订阅者对象，callbackFunc为回调函数
	template <class TEvent, class TOwner>
	void DisSubscribeEvent(TOwner* owner, CallBackFunc<TEvent, TOwner> callbackFunc)
	{
		auto handlersPtr = subscribers[typeid(TEvent)].get();
		if (!handlersPtr)
		{
			return; // 如果没有订阅者列表，则直接返回
		}
		// 从订阅者列表中移除匹配的事件回调对象
		auto& handlers = *handlersPtr;
		handlers.erase(
			std::remove_if(handlers.begin(), handlers.end(),
				[owner, callbackFunc](const std::unique_ptr<IEventCallBack>& handler)
				{
					// 尝试将 handler 转换为我们想要的具体类型
					auto concreteHandler = dynamic_cast<EventCallBack<TOwner, TEvent>*>(handler.get());
					if (concreteHandler)
					{
						// 检查 owner 和 callbackFunc 是否匹配
						return concreteHandler->GetOwner() == owner && concreteHandler->GetCallbackFunc() == callbackFunc;
					}
					return false;
				}
			),
			handlers.end()
		);
	}

  
	// 触发事件后发布，args为事件构造参数
	template <class TEvent, typename ...TArgs>
	void EmitEvent(TArgs&& ...args)
	{
		auto handlersPtr = subscribers[typeid(TEvent)].get();
		if (handlersPtr)
		{
			auto& handlers = *handlersPtr;
			// 遍历事件回调列表的事件回调对象
			for (auto& handlerUniquePtr : handlers)
			{
				auto& handler = *handlerUniquePtr;
				TEvent event(std::forward<TArgs>(args)...);
				handler.ExecuteEvent(event);
			}
		}
	}

private:
	// 事件类型到事件处理回调列表的映射
	std::map<std::type_index, std::unique_ptr<Handlers>> subscribers;
};

#endif // !EVNETBUS_H
