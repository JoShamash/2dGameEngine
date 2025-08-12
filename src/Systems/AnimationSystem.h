#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Time/Timer.h"

// 动画系统类，定义一系列逻辑接口，负责处理帧动画
class AnimationSystem : public System
{
public:
	inline static const std::string NAME = std::string("AnimationSystem") + U8_TO_CHARPTR("动画系统"); // 系统名称

	AnimationSystem()
	{
		RequireComponent<SpriteComponent>();
		RequireComponent<AnimationComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief 更新动画系统，遍历所有实体并更新它们的动画帧
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
			
			// 计算当前时间与开始时间的差值
			auto elapsedTime = Timer::time.gameTime - startTime;
			// 计算当前帧索引
			currentFrame = elapsedTime * frameSpeed / 1000;
			if(numFrames == 0)
			{
				currentFrame = 0; // 防止mod零
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