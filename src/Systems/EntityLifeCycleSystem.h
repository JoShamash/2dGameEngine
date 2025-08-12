#ifndef ENTITYLIFECYCLESYSTEM_H
#define ENTITYLIFECYCLESYSTEM_H

#include "../ECS/ECS.h"
#include "../GameEngine/GameEngine.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/EntityLifeCycleComponent.h"

// 实体生命周期系统类，定义一系列逻辑接口，负责处理实体的生命周期
class EntityLifeCycleSystem : public System
{
public:
	inline static const std::string NAME = std::string("EntityLifeCycleSystem") + U8_TO_CHARPTR("实体生命周期系统"); // 系统名称

	EntityLifeCycleSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		RequireComponent<EntityLifeCycleComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief 更新实体生命周期系统，遍历所有实体并处理它们的生命周期
	*/
	void Update()
	{
		for (const auto& entity : GetEntities())
		{
			auto& entityLifeCycleComponent = entity.GetComponent<EntityLifeCycleComponent>();

			// 检查实体是否超出地图边界，如果超出则outOfMapDelay延迟时间后销毁实体，延迟时间内返回到地图内则不销毁
			if (IsEntityOutOfMap(entity))
			{
				// 如果实体第一次超出地图边界，则设置isOutOfMap为true，并重置outOfMapTime
				if (entityLifeCycleComponent.isOutOfMap == false)
				{
					entityLifeCycleComponent.isOutOfMap = true;
					entityLifeCycleComponent.outOfMapTime = 0;
				}
				// 如果实体已经超出地图边界，则增加outOfMapTime
				else
				{
					entityLifeCycleComponent.outOfMapTime += Timer::time.deltaTime;
					// 如果超出地图边界的时间超过了延迟时间，则销毁实体
					if (entityLifeCycleComponent.outOfMapTime >= entityLifeCycleComponent.outOfMapDelay)
					{
						entity.KillSelf();
					}
				}
			}
			else
			{
				entityLifeCycleComponent.isOutOfMap = false;
			}
		}
	}

private:
	bool IsEntityOutOfMap(const Entity& entity) const
	{
		const auto& transformComponent = entity.GetComponent<TransformComponent>();
		const auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();

		const auto& position = transformComponent.position;
		const auto& mapHeight = GameEngine::mapHeight;
		const auto& mapWidth = GameEngine::mapWidth;

		bool isOutOfMap = false;

		// 有精灵组件时，使用精灵组件的大小范围来判断是否超出地图边界
		if (entity.HasComponent<SpriteComponent>())
		{
			const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			isOutOfMap = (position.x + spriteComponent.size.x * transformComponent.scale.x < 0 ||
				position.x > mapWidth ||
				position.y + spriteComponent.size.y * transformComponent.scale.y < 0 ||
				position.y > mapHeight);
		}
		// 没有精灵组件时，通过坐标判断是否超出地图边界
		else
		{
			isOutOfMap = (position.x < 0 ||
				position.x > mapWidth ||
				position.y < 0 ||
				position.y > mapHeight);
		}

		return isOutOfMap;
	}
};

#endif // !ENTITYLIFECYCLESYSTEM_H
