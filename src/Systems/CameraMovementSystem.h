#ifndef  CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Camera/Camera.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/CameraFollowComponent.h"

class CameraMovementSystem : public System
{
public:
	inline static const std::string NAME = std::string("CameraMovementSystem") + U8_TO_CHARPTR("摄像机移动系统"); // 系统名称

	CameraMovementSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<CameraFollowComponent>();
	}

	void Update(Camera& camera)
	{
		for (const auto& entity : GetEntities())
		{
			const auto& transformComponent = entity.GetComponent<TransformComponent>();
			const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			const auto& position = transformComponent.position;

			// 更新摄像机位置，使其跟随实体，摄像机位置为实体位置减去摄像机视口宽高的一半
			camera.x = position.x - (camera.w / 2.0);
			camera.y = position.y - (camera.h / 2.0);

			// 限制摄像机在地图范围内
			if (camera.x < 0) 
			{
				camera.x = 0;
			}
			if (camera.y < 0) 
			{
				camera.y = 0;
			}
			if (camera.x > GameEngine::mapWidth - camera.w)
			{
				camera.x = GameEngine::mapWidth - camera.w;
			}
			if (camera.y > GameEngine::mapHeight - camera.h)
			{
				camera.y = GameEngine::mapHeight - camera.h;
			}

		}
	}
};

#endif // ! CAMERAMOVEMENTSYSTEM_H
