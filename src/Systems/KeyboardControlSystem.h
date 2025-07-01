#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"


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

					srcRect.y = size.y * static_cast<int>(DirectionTexture::Up);
					break;
				}
				case SDLK_RIGHT: // →向右移动
				{
					auto& rightVelocity = keyboardControlComponent.rightVelocity;
					velocity = rightVelocity;

					srcRect.y = size.y * static_cast<int>(DirectionTexture::Right);
					break;
				}
				case SDLK_DOWN: // ↓向下移动
				{
					auto& downVelocity = keyboardControlComponent.downVelocity;
					velocity = downVelocity;

					srcRect.y = size.y * static_cast<int>(DirectionTexture::Down);
					break;
				}
				case SDLK_LEFT: // ←向左移动
				{
					auto& leftVelocity = keyboardControlComponent.leftVelocity;
					velocity = leftVelocity;

					srcRect.y = size.y * static_cast<int>(DirectionTexture::Left);
					break;
				}
				//case SDLK_SPACE: // 空格发射
				//{
				//	auto& isEmit = projectileEmitterComponent.isEmit;
				//	isEmit = !isEmit;
				//	break;
				//}
			}
		}
	}
};

#endif // !KEYBOARDCONTROLSYSTEM_H
