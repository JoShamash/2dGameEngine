#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

// ��ײϵͳ�࣬���ڴ�����ײ������Ӧ
class CollisionSystem : public System
{
public:
	inline static const std::string NAME = std::string("CollisionSystem") + U8_TO_CHARPTR("��ײϵͳ"); // ϵͳ����

	CollisionSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief ������ײϵͳ����������ʵ�岢ͨ��AABB�����ײ
	*/
	void Update()
	{
		const auto& entities = GetEntities();

		for (size_t i = 0; i < entities.size(); ++i)
		{
			auto& entityA = entities[i];
			for (size_t j = i + 1; j < entities.size(); ++j)
			{
				auto& entityB = entities[j];
				if (entityA == entityB) continue; // ����������ײ���

				bool isCollision = CheckAABBCollision(entityA, entityB);
				if (isCollision)
				{
					entityA.GetComponent<BoxColliderComponent>().isCollision = true;
					entityB.GetComponent<BoxColliderComponent>().isCollision = true;
					entityA.KillSelf();
					entityB.KillSelf();
				}
				else
				{
					entityA.GetComponent<BoxColliderComponent>().isCollision = false;
					entityB.GetComponent<BoxColliderComponent>().isCollision = false;
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
		auto AW = colliderA.size.x;
		auto AH = colliderA.size.y;

		auto BX = transformB.position.x + colliderB.offset.x;
		auto BY = transformB.position.y + colliderB.offset.y;
		auto BW = colliderB.size.x;
		auto BH = colliderB.size.y;

		return (AX < BX + BW && BX < AX + AW && AY < BY + BH && BY < AY + AH);
	}
};

#endif // !COLLISIONSYSTEM_H