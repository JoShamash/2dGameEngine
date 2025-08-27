#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

// �˶�ϵͳ�࣬����һϵ���߼��ӿڣ�������ʵ����˶��߼�
class MovementSystem : public System 
{
public:
	inline static const std::string NAME = std::string("MovementSystem") + U8_TO_CHARPTR("�˶�ϵͳ"); // ϵͳ����

	MovementSystem() 
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief �����¼�
	* @param eventBus �¼����ߣ������Ļ򴥷��¼�
	*/
	void SubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
	}

	/**
	* @brief ȡ�������¼�
	* @param eventBus �¼����ߣ������Ļ򴥷��¼�
	*/
	void UnSubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->DisSubscribeEvent<CollisionEvent>(this, &MovementSystem::OnCollision);
	}

	/**
	* @brief �����˶�״̬������ʵ����˶��߼�
	* @param deltaTime ʱ����������ʾ���ϴθ�������������ʱ��
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
				// �������ʵ���ڵ�ͼ��Χ��
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
					// ȷ�����ʵ��ľ��������С��������ͼ�߽�
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
					// ���û�о����������ֱ������λ���ڵ�ͼ��Χ��
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
	* @brief CollisionEvent��ײ�¼��Ļص����������¼�����ʱ���øú��������¼����߼�����
	* @param event CollisionEvent��ײ�¼�����
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

		// �򵥵���ײ�����߼�����ʵ���ٶȷ�����
		entityRigidBodyComponent.velocity.x = -entityRigidBodyComponent.velocity.x;
		entityRigidBodyComponent.velocity.y = -entityRigidBodyComponent.velocity.y;
	}
};

#endif // !MOVEMENTSYSTEM_H
