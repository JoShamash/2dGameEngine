#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

// 运动系统类，定义一系列逻辑接口，负责处理实体的运动逻辑
class MovementSystem : public System 
{
public:
	inline static const std::string NAME = std::string("MovementSystem") + U8_TO_CHARPTR("运动系统"); // 系统名称

	MovementSystem() 
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief 订阅事件
	* @param eventBus 事件总线，负责订阅或触发事件
	*/
	void SubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
	}

	/**
	* @brief 取消订阅事件
	* @param eventBus 事件总线，负责订阅或触发事件
	*/
	void UnSubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->DisSubscribeEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
	}

	/**
	* @brief 更新运动状态，处理实体的运动逻辑
	* @param deltaTime 时间增量，表示自上次更新以来经过的时间
	*/
	void Update()
	{
		for (const auto& entity : GetEntities())
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
			
			transformComponent.position.x += rigidBodyComponent.velocity.x * rigidBodyComponent.speed * Timer::time.deltaTime / 1000.0;
			transformComponent.position.y += rigidBodyComponent.velocity.y * rigidBodyComponent.speed * Timer::time.deltaTime / 1000.0;

			if(entity.HasTag("player"))
			{
				int padding = 10;
				// 限制玩家实体在地图范围内
				if (transformComponent.position.x <= 0 + padding)
				{
					transformComponent.position.x = 0 + padding;
				}
				if (transformComponent.position.y <= 0 + padding)
				{
					transformComponent.position.y = 0 + padding;
				}

				if(entity.HasComponent<SpriteComponent>())
				{
					const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
					// 确保玩家实体的精灵组件大小不超过地图边界
					if (transformComponent.position.x + spriteComponent.size.x >= GameEngine::mapWidth - padding)
					{
						transformComponent.position.x = GameEngine::mapWidth - spriteComponent.size.x - padding;
					}
					if (transformComponent.position.y + spriteComponent.size.y >= GameEngine::mapHeight - padding)
					{
						transformComponent.position.y = GameEngine::mapHeight - spriteComponent.size.y - padding;
					}
				}
				else
				{
					// 如果没有精灵组件，则直接限制位置在地图范围内
					if (transformComponent.position.x >= GameEngine::mapWidth - padding)
					{
						transformComponent.position.x = GameEngine::mapWidth - padding;
					}
					if (transformComponent.position.y >= GameEngine::mapHeight - padding)
					{
						transformComponent.position.y = GameEngine::mapHeight - padding;
					}
				}
			}
		}
	}

	/**
	* @brief CollisionEvent碰撞事件的回调函数，当事件触发时调用该函数进行事件的逻辑处理
	* @param event CollisionEvent碰撞事件引用
	*/
	void OnCollision(CollisionEvent& event)
	{
		const Entity& entityA = event.a;
		const Entity& entityB = event.b;

		if(entityA.HasTag("player") && entityB.HasGroup("obstacle"))
		{
			ObstacleCollision(entityA, entityB);
		}
		else if(entityB.HasTag("player") && entityA.HasGroup("obstacle"))
		{
			ObstacleCollision(entityB, entityA);
		}
		else if(entityA.HasGroup("enemy") && entityB.HasGroup("obstacle"))
		{
			ObstacleCollision(entityA, entityB);
		}
		else if(entityB.HasGroup("enemy") && entityA.HasGroup("obstacle"))
		{
			ObstacleCollision(entityB, entityA);
		}
	}

	void ObstacleCollision(const Entity& entity, const Entity& obstacle)
	{
		auto& entityTransformComponent = entity.GetComponent<TransformComponent>();
		auto& entityRigidBodyComponent = entity.GetComponent<RigidBodyComponent>();

		// 简单的碰撞处理逻辑：将实体速度方向反向
		entityRigidBodyComponent.velocity.x = -entityRigidBodyComponent.velocity.x;
		entityRigidBodyComponent.velocity.y = -entityRigidBodyComponent.velocity.y;
	}
};

#endif // !MOVEMENTSYSTEM_H
