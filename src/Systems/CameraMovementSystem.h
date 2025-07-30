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
	inline static const std::string NAME = std::string("CameraMovementSystem") + U8_TO_CHARPTR("������ƶ�ϵͳ"); // ϵͳ����

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

			// ���������λ�ã�ʹ�����ʵ�壬�����λ��Ϊʵ��λ�ü�ȥ������ӿڿ�ߵ�һ��
			camera.x = position.x - (camera.w / 2.0);
			camera.y = position.y - (camera.h / 2.0);

			// ����������ڵ�ͼ��Χ��
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
