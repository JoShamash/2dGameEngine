#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

// 伤害系统，定义一系列逻辑接口，用于实体损伤的逻辑处理
class DamageSystem : public System
{
public:
	inline static const std::string NAME = std::string("DamageSystem") + U8_TO_CHARPTR("伤害系统"); // 系统名称

	DamageSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief 订阅事件
	* @param eventBus 事件总线，负责订阅或触发事件
	*/
	void SubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
	}

	/**
	* @brief 取消订阅事件
	* @param eventBus 事件总线，负责订阅或触发事件
	*/
	void UnSubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		
	}

	/**
	* @brief CollisionEvent碰撞事件的回调函数，当事件触发时调用该函数进行事件的逻辑处理
	* @param event CollisionEvent碰撞事件引用
	*/
	void OnCollision(CollisionEvent& event)
	{
		const Entity& entityA = event.a;
		const Entity& entityB = event.b;

		// 实体A、B都为发射物，暂不处理
		if (entityA.HasGroup("projectile") && entityB.HasGroup("projectile"))
		{

		}
		// 实体A为玩家，实体B为敌人，暂不处理
		else if (entityA.HasTag("player") && entityB.HasGroup("enemy"))
		{

		}
		// 实体A为敌人，实体B为玩家，暂不处理
		else if (entityA.HasTag("enemy") && entityB.HasGroup("player"))
		{

		}
		// 实体A为发射物，实体B为玩家
		else if (entityA.HasGroup("projectile") && entityB.HasTag("player"))
		{
			OnProjectileHitObject(entityA, entityB);
		}
		// 实体A为玩家，实体B为发射物
		else if (entityA.HasTag("player") && entityB.HasGroup("projectile"))
		{
			OnProjectileHitObject(entityB, entityA);
		}
		// 实体A为发射物，实体B为敌人
		else if (entityA.HasGroup("projectile") && entityB.HasGroup("enemy"))
		{
			OnProjectileHitObject(entityA, entityB);
		}
		// 实体A为敌人，实体B为发射物
		else if (entityA.HasGroup("enemy") && entityB.HasGroup("projectile"))
		{
			OnProjectileHitObject(entityB, entityA);
		}
	}

	void OnProjectileHitObject(const Entity& projectile, const Entity& object)
	{
		const auto& projectileComponent = projectile.GetComponent<ProjectileComponent>();
		auto& healthComponent = object.GetComponent<HealthComponent>();

		auto finalHP = healthComponent.hp - projectileComponent.hitDamage;
		healthComponent.hp = finalHP <= 0 ? 0 : finalHP;
		projectile.KillSelf();
	}
};

#endif // !DAMAGESYSTEM_H