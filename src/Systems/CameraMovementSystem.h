#ifndef  CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Camera/Camera.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/CameraFollowComponent.h"

#include <algorithm>

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
        auto& entities = GetEntities();
        auto entityNum = entities.size();

        if (entityNum == 1)
        {
            auto& entity = entities.front();
            const auto& transformComponent = entity.GetComponent<TransformComponent>();
            const auto& position = transformComponent.position;

            // 计算目标摄像机位置，使实体居中
            double targetX = position.x - (camera.w / 2.0f);
            double targetY = position.y - (camera.h / 2.0f);

            // 限制目标位置在地图范围内
            targetX = std::clamp(targetX, 0.0, GameEngine::mapWidth - camera.w);
            targetY = std::clamp(targetY, 0.0, GameEngine::mapHeight - camera.h);

            if(!camera.isSmoothEnabled())
            {
                // 如果未启用平滑，直接设置位置
                camera.x = targetX;
                camera.y = targetY;
                return;
			}
            else
            {
                // 使用线性插值平滑更新摄像机位置
				double smoothFactor = camera.getSmoothFactor();
                camera.x = Lerp(camera.x, targetX, smoothFactor);
                camera.y = Lerp(camera.y, targetY, smoothFactor);
            }
            
        }
        else if (entityNum > 1)
        {
            // 多人摄像机绑定，类似逻辑，计算所有实体的包围盒中心作为目标位置
            double minX = std::numeric_limits<double>::max();
            double minY = std::numeric_limits<double>::max();
            double maxX = std::numeric_limits<double>::lowest();
            double maxY = std::numeric_limits<double>::lowest();

            for (const auto& entity : entities)
            {
                const auto& transformComponent = entity.GetComponent<TransformComponent>();
                const auto& position = transformComponent.position;

                if (position.x < minX) minX = position.x;
                if (position.y < minY) minY = position.y;
                if (position.x > maxX) maxX = position.x;
                if (position.y > maxY) maxY = position.y;
            }

            // 计算包围盒中心
            double centerX = (minX + maxX) / 2.0;
            double centerY = (minY + maxY) / 2.0;

            // 计算目标摄像机位置，使包围盒中心居中
            double targetX = centerX - (camera.w / 2.0);
            double targetY = centerY - (camera.h / 2.0);

            // 限制目标位置在地图范围内
            targetX = std::clamp(targetX, 0.0, GameEngine::mapWidth - camera.w);
            targetY = std::clamp(targetY, 0.0, GameEngine::mapHeight - camera.h);

            if (!camera.isSmoothEnabled())
            {
                // 如果未启用平滑，直接设置位置
                camera.x = targetX;
                camera.y = targetY;
                return;
            }
            else
            {
                // 使用线性插值平滑更新摄像机位置
                double smoothFactor = camera.getSmoothFactor();
                camera.x = Lerp(camera.x, targetX, smoothFactor);
                camera.y = Lerp(camera.y, targetY, smoothFactor);
            }
        }
	}

private:
	// 线性插值函数
    template <typename T>
	T Lerp(T a, T b, float t)
	{
		return a + t * (b - a);
	}
};

#endif // ! CAMERAMOVEMENTSYSTEM_H
