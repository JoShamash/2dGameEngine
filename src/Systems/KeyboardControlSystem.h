#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"


class KeyboardControlSystem : public System
{
public:
	inline static const std::string NAME = std::string("KeyboardControlSystem") + U8_TO_CHARPTR("键盘控制系统"); // 系统名称

	KeyboardControlSystem()
	{
		RequireComponent<RigidBodyComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<ProjectileEmitterComponent>();
		RequireComponent<KeyboardControlComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	void SubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
		eventBus->SubscribeEvent<KeyReleasedEvent>(this, &KeyboardControlSystem::OnKeyReleased);
	}

	void OnKeyPressed(KeyPressedEvent& event)
	{
		for (const auto& entity : GetEntities())
		{
			auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			auto& keyboardControlComponent = entity.GetComponent<KeyboardControlComponent>();
			auto& projectileEmitterComponent = entity.GetComponent<ProjectileEmitterComponent>();

			auto& velocity = rigidBodyComponent.velocity;
			auto& srcRect = spriteComponent.srcRect;
			const auto& size = spriteComponent.size;
			switch (event.key)
			{
				case SDLK_UP: // ↑向上移动
				{
					auto& upVelocity = keyboardControlComponent.upVelocity;
					velocity = upVelocity;
					rigidBodyComponent.speed = rigidBodyComponent.maxSpeed;

					srcRect.y = size.y * static_cast<int>(DirectionTexture::Up);
					keyboardControlComponent.isMove = true;
					break;
				}
				case SDLK_RIGHT: // →向右移动
				{
					auto& rightVelocity = keyboardControlComponent.rightVelocity;
					velocity = rightVelocity;
					rigidBodyComponent.speed = rigidBodyComponent.maxSpeed;

					srcRect.y = size.y * static_cast<int>(DirectionTexture::Right);
					keyboardControlComponent.isMove = true;
					break;
				}
				case SDLK_DOWN: // ↓向下移动
				{
					auto& downVelocity = keyboardControlComponent.downVelocity;
					velocity = downVelocity;
					rigidBodyComponent.speed = rigidBodyComponent.maxSpeed;

					srcRect.y = size.y * static_cast<int>(DirectionTexture::Down);
					keyboardControlComponent.isMove = true;
					break;
				}
				case SDLK_LEFT: // ←向左移动
				{
					auto& leftVelocity = keyboardControlComponent.leftVelocity;
					velocity = leftVelocity;
					rigidBodyComponent.speed = rigidBodyComponent.maxSpeed;

					srcRect.y = size.y * static_cast<int>(DirectionTexture::Left);
					keyboardControlComponent.isMove = true;
					break;
				}
				//case SDLK_SPACE: // 空格发射
				//{
				//	auto& isEmit = projectileEmitterComponent.isEmit;
				//	isEmit = !isEmit;
				//	break;
				//}
				default:
				{
					keyboardControlComponent.isMove = false;
				}
			}
		}
	}

	void OnKeyReleased(KeyReleasedEvent& event)
	{
		for (const auto& entity : GetEntities())
		{
			auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
			auto& keyboardControlComponent = entity.GetComponent<KeyboardControlComponent>();

			if(!keyboardControlComponent.isMove)
			{
				switch (event.key)
				{
					case SDLK_UP:		// ↑向上移动
					case SDLK_RIGHT:	// →向右移动
					case SDLK_DOWN:		// ↓向下移动
					case SDLK_LEFT:		// ←向左移动
					{
						rigidBodyComponent.speed = 0;
					}
				}
			}
		}
	}
};

#endif // !KEYBOARDCONTROLSYSTEM_H
