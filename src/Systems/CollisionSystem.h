#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include <vector>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CampComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

// 碰撞系统类，定义一系列逻辑接口，用于处理碰撞检测和响应
class CollisionSystem : public System
{
public:
	inline static const std::string NAME = std::string("CollisionSystem") + U8_TO_CHARPTR("碰撞系统"); // 系统名称

	CollisionSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
		RequireComponent<CampComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief 更新碰撞系统，遍历所有实体并通过AABB检测碰撞
	* @param eventBus 事件总线，负责订阅或触发事件
	*/
	void Update(std::unique_ptr<EventBus>& eventBus)
	{
		const auto& entities = GetEntities();

		// 首先将isCollision全部标记为false，之后只要发生碰撞即标记为true，剩下为false的实体则是没有与任何其他实体发生碰撞
		for (const auto& entity : entities)
		{
			entity.GetComponent<BoxColliderComponent>().isCollision = false;
		}

		for (size_t i = 0; i < entities.size(); ++i)
		{
			auto& entityA = entities[i];
			const auto& groupA = entityA.GetComponent<CampComponent>().camp;

			for (size_t j = i + 1; j < entities.size(); ++j)
			{
				auto& entityB = entities[j];
				const auto& groupB = entityB.GetComponent<CampComponent>().camp;
				// 跳过自身碰撞检测和同阵营
				if (entityA == entityB || groupA == groupB) 
				{
					continue;
				}

				bool isCollision = CheckAABBCollision(entityA, entityB);

				if (isCollision)
				{
					// 如果发生了碰撞，则标记为 true
					entityA.GetComponent<BoxColliderComponent>().isCollision = true;
					entityB.GetComponent<BoxColliderComponent>().isCollision = true;
					
					// 触发CollisionEvent碰撞事件，传入该事件构造参数
					eventBus->EmitEvent<CollisionEvent>(entityA, entityB);
				}
			}
		}
	}

private:
	/**
	* @brief AABB（轴对齐边界框）碰撞检测
	* @param A 实体A
	* @param B 实体B
	*/
	bool CheckAABBCollision(const Entity& A, const Entity& B)
	{
		auto& transformA = A.GetComponent<TransformComponent>();
		auto& colliderA = A.GetComponent<BoxColliderComponent>();
		auto& transformB = B.GetComponent<TransformComponent>();
		auto& colliderB = B.GetComponent<BoxColliderComponent>();

		auto AX = transformA.position.x + colliderA.offset.x;
		auto AY = transformA.position.y + colliderA.offset.y;
		auto AW = colliderA.size.x * transformA.scale.x;
		auto AH = colliderA.size.y * transformA.scale.y;

		auto BX = transformB.position.x + colliderB.offset.x;
		auto BY = transformB.position.y + colliderB.offset.y;
		auto BW = colliderB.size.x * transformB.scale.x;
		auto BH = colliderB.size.y * transformB.scale.y;

		return (AX < BX + BW && BX < AX + AW && AY < BY + BH && BY < AY + AH);
	}
};

#endif // !COLLISIONSYSTEM_H