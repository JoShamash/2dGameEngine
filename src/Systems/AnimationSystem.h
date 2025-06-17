#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"

// ����ϵͳ�࣬������֡����
class AnimationSystem : public System
{
public:
	inline static const std::string NAME = std::string("AnimationSystem") + U8_TO_CHARPTR("����ϵͳ"); // ϵͳ����

	AnimationSystem()
	{
		RequireComponent<SpriteComponent>();
		RequireComponent<AnimationComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief ���¶���ϵͳ����������ʵ�岢�������ǵĶ���֡
	*/
	void Update()
	{
		for (const auto& entity : GetEntities())
		{
			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			auto& animationComponent = entity.GetComponent<AnimationComponent>();

			const auto& startTime = animationComponent.startTime;
			const auto& frameSpeed = animationComponent.frameSpeed;
			const auto& numFrames = animationComponent.numFrames;
			auto& currentFrame = animationComponent.currentFrame;
			currentFrame = ((SDL_GetTicks64() - startTime) * frameSpeed / 1000) % numFrames;

			const auto& size = spriteComponent.size;
			auto& srcRect = spriteComponent.srcRect;
			srcRect.x = currentFrame * size.x;
		}
	}
};

#endif // !ANIMATIONSYSTEM_H