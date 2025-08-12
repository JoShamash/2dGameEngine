#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Time/Timer.h"

// ����ϵͳ�࣬����һϵ���߼��ӿڣ�������֡����
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
			const auto& isLooping = animationComponent.isLooping;
			const auto& frameSpeed = animationComponent.frameSpeed;
			const auto& numFrames = animationComponent.numFrames;
			auto& currentFrame = animationComponent.currentFrame;
			
			// ���㵱ǰʱ���뿪ʼʱ��Ĳ�ֵ
			auto elapsedTime = Timer::time.gameTime - startTime;
			// ���㵱ǰ֡����
			currentFrame = elapsedTime * frameSpeed / 1000;
			if(numFrames == 0)
			{
				currentFrame = 0; // ��ֹmod��
			}
			else if(isLooping)
			{
				currentFrame %= numFrames;
			}

			const auto& size = spriteComponent.size;
			auto& srcRect = spriteComponent.srcRect;
			srcRect.x = currentFrame * size.x;
		}
	}
};

#endif // !ANIMATIONSYSTEM_H