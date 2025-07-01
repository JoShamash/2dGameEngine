#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

// �˺�ϵͳ������һϵ���߼��ӿڣ�����ʵ�����˵��߼�����
class DamageSystem : public System
{
public:
	inline static const std::string NAME = std::string("DamageSystem") + U8_TO_CHARPTR("�˺�ϵͳ"); // ϵͳ����

	DamageSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief �����¼�
	* @param eventBus �¼����ߣ������Ļ򴥷��¼�
	*/
	void SubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
	}

	/**
	* @brief ȡ�������¼�
	* @param eventBus �¼����ߣ������Ļ򴥷��¼�
	*/
	void UnSubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		
	}

	/**
	* @brief CollisionEvent��ײ�¼��Ļص����������¼�����ʱ���øú��������¼����߼�����
	* @param event CollisionEvent��ײ�¼�����
	*/
	void OnCollision(CollisionEvent& event)
	{
		const Entity& entityA = event.a;
		const Entity& entityB = event.b;

		// ʵ��A��B��Ϊ������ݲ�����
		if (entityA.HasGroup("projectile") && entityB.HasGroup("projectile"))
		{

		}
		// ʵ��AΪ��ң�ʵ��BΪ���ˣ��ݲ�����
		else if (entityA.HasTag("player") && entityB.HasGroup("enemy"))
		{

		}
		// ʵ��AΪ���ˣ�ʵ��BΪ��ң��ݲ�����
		else if (entityA.HasTag("enemy") && entityB.HasGroup("player"))
		{

		}
		// ʵ��AΪ�����ʵ��BΪ���
		else if (entityA.HasGroup("projectile") && entityB.HasTag("player"))
		{
			OnProjectileHitObject(entityA, entityB);
		}
		// ʵ��AΪ��ң�ʵ��BΪ������
		else if (entityA.HasTag("player") && entityB.HasGroup("projectile"))
		{
			OnProjectileHitObject(entityB, entityA);
		}
		// ʵ��AΪ�����ʵ��BΪ����
		else if (entityA.HasGroup("projectile") && entityB.HasGroup("enemy"))
		{
			OnProjectileHitObject(entityA, entityB);
		}
		// ʵ��AΪ���ˣ�ʵ��BΪ������
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