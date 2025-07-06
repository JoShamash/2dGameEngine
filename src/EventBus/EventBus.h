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
                                                                                            
// �¼��ص���Ľӿڸ���
class IEventCallBack
{
public:
	IEventCallBack() = default;
	virtual ~IEventCallBack() = default;

	// �����¼���ִ�лص�����
	void ExecuteEvent(Event& e)
	{
		CallBackEvent(e);
	}

private:
	// ���麯�����¼��Ļص�����
	virtual void CallBackEvent(Event& e) = 0;
};

// CallBackFunc���ͣ�TOwner��ĺ���ָ�룬��������ΪTEvent&
template <class TEvent, class TOwner>
using CallBackFunc = void (TOwner::*)(TEvent&);

// �����¼��ص���
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
	TOwner* owner;								// ���������
	CallBackFunc<TEvent, TOwner> callbackFunc;	// ������Ļص�����

	// ����owner�Ļص�����callbackFunc
	virtual void CallBackEvent(Event& event) override
	{
		std::invoke(callbackFunc, owner, static_cast<TEvent&>(event));
	}
};

// HandlerList���ͣ�����ά�������¼��ص����б�
using Handlers = std::vector<std::unique_ptr<IEventCallBack>>;

//   _____                          _     ____                
//  | ____| __   __   ___   _ __   | |_  | __ )   _   _   ___ 
//  |  _|   \ \ / /  / _ \ | '_ \  | __| |  _ \  | | | | / __|
//  | |___   \ V /  |  __/ | | | | | |_  | |_) | | |_| | \__ \
//  |_____|   \_/    \___| |_| |_|  \__| |____/   \__,_| |___/
                                       
// �¼����ߣ������¼��Ķ����봥���¼���ַ�
class EventBus
{
public:
	EventBus()
	{
		std::string message = U8_TO_CHARPTR("�����¼���������");
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	~EventBus()
	{
		std::string message = U8_TO_CHARPTR("�����¼���������");
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	// �����¼����ߣ�������ж�����
	void Reset()
	{
		subscribers.clear();
	}

	// �����¼���ownerΪ�����߶���callbackFuncΪ�ص�����
	template <class TEvent, class TOwner>
	void SubscribeEvent(TOwner* owner, CallBackFunc<TEvent, TOwner> callbackFunc)
	{
		// ������¼����ͻ�û�ж������б��򴴽�һ���µ��б�
		if (!subscribers[typeid(TEvent)].get())
		{
			subscribers[typeid(TEvent)] = std::make_unique<Handlers>();
		}

		// �����¼��ص�������ӵ��������б�
		auto subscriber = std::make_unique<EventCallBack<TOwner, TEvent>>(owner, callbackFunc);
		subscribers[typeid(TEvent)]->emplace_back(std::move(subscriber));
	}

	// ȡ�������¼���ownerΪ�����߶���callbackFuncΪ�ص�����
	template <class TEvent, class TOwner>
	void DisSubscribeEvent(TOwner* owner, CallBackFunc<TEvent, TOwner> callbackFunc)
	{
		auto handlersPtr = subscribers[typeid(TEvent)].get();
		if (!handlersPtr)
		{
			return; // ���û�ж������б���ֱ�ӷ���
		}
		// �Ӷ������б����Ƴ�ƥ����¼��ص�����
		auto& handlers = *handlersPtr;
		handlers.erase(
			std::remove_if(handlers.begin(), handlers.end(),
				[owner, callbackFunc](const std::unique_ptr<IEventCallBack>& handler)
				{
					// ���Խ� handler ת��Ϊ������Ҫ�ľ�������
					auto concreteHandler = dynamic_cast<EventCallBack<TOwner, TEvent>*>(handler.get());
					if (concreteHandler)
					{
						// ��� owner �� callbackFunc �Ƿ�ƥ��
						return concreteHandler->GetOwner() == owner && concreteHandler->GetCallbackFunc() == callbackFunc;
					}
					return false;
				}
			),
			handlers.end()
		);
	}

  
	// �����¼��󷢲���argsΪ�¼��������
	template <class TEvent, typename ...TArgs>
	void EmitEvent(TArgs&& ...args)
	{
		auto handlersPtr = subscribers[typeid(TEvent)].get();
		if (handlersPtr)
		{
			auto& handlers = *handlersPtr;
			// �����¼��ص��б���¼��ص�����
			for (auto& handlerUniquePtr : handlers)
			{
				auto& handler = *handlerUniquePtr;
				TEvent event(std::forward<TArgs>(args)...);
				handler.ExecuteEvent(event);
			}
		}
	}

private:
	// �¼����͵��¼�����ص��б��ӳ��
	std::map<std::type_index, std::unique_ptr<Handlers>> subscribers;
};

#endif // !EVNETBUS_H
