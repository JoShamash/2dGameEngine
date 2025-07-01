#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include <vector>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CampComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

// ��ײϵͳ�࣬����һϵ���߼��ӿڣ����ڴ�����ײ������Ӧ
class CollisionSystem : public System
{
public:
	inline static const std::string NAME = std::string("CollisionSystem") + U8_TO_CHARPTR("��ײϵͳ"); // ϵͳ����

	CollisionSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
		RequireComponent<CampComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief ������ײϵͳ����������ʵ�岢ͨ��AABB�����ײ
	* @param eventBus �¼����ߣ������Ļ򴥷��¼�
	*/
	void Update(std::unique_ptr<EventBus>& eventBus)
	{
		const auto& entities = GetEntities();

		// ���Ƚ�isCollisionȫ�����Ϊfalse��֮��ֻҪ������ײ�����Ϊtrue��ʣ��Ϊfalse��ʵ������û�����κ�����ʵ�巢����ײ
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
				// ����������ײ����ͬ��Ӫ
				if (entityA == entityB || groupA == groupB) 
				{
					continue;
				}

				bool isCollision = CheckAABBCollision(entityA, entityB);

				if (isCollision)
				{
					// �����������ײ������Ϊ true
					entityA.GetComponent<BoxColliderComponent>().isCollision = true;
					entityB.GetComponent<BoxColliderComponent>().isCollision = true;
					
					// ����CollisionEvent��ײ�¼���������¼��������
					eventBus->EmitEvent<CollisionEvent>(entityA, entityB);
				}
			}
		}
	}

private:
	/**
	* @brief AABB�������߽����ײ���
	* @param A ʵ��A
	* @param B ʵ��B
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