#ifndef ENTITYLIFECYCLESYSTEM_H
#define ENTITYLIFECYCLESYSTEM_H

#include "../ECS/ECS.h"
#include "../GameEngine/GameEngine.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/EntityLifeCycleComponent.h"

// ʵ����������ϵͳ�࣬����һϵ���߼��ӿڣ�������ʵ�����������
class EntityLifeCycleSystem : public System
{
public:
	inline static const std::string NAME = std::string("EntityLifeCycleSystem") + U8_TO_CHARPTR("ʵ����������ϵͳ"); // ϵͳ����

	EntityLifeCycleSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		RequireComponent<EntityLifeCycleComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief ����ʵ����������ϵͳ����������ʵ�岢�������ǵ���������
	*/
	void Update()
	{
		for (const auto& entity : GetEntities())
		{
			auto& entityLifeCycleComponent = entity.GetComponent<EntityLifeCycleComponent>();

			// ���ʵ���Ƿ񳬳���ͼ�߽磬���������outOfMapDelay�ӳ�ʱ�������ʵ�壬�ӳ�ʱ���ڷ��ص���ͼ��������
			if (IsEntityOutOfMap(entity))
			{
				// ���ʵ���һ�γ�����ͼ�߽磬������isOutOfMapΪtrue��������outOfMapTime
				if (entityLifeCycleComponent.isOutOfMap == false)
				{
					entityLifeCycleComponent.isOutOfMap = true;
					entityLifeCycleComponent.outOfMapTime = 0;
				}
				// ���ʵ���Ѿ�������ͼ�߽磬������outOfMapTime
				else
				{
					entityLifeCycleComponent.outOfMapTime += Timer::time.deltaTime;
					// ���������ͼ�߽��ʱ�䳬�����ӳ�ʱ�䣬������ʵ��
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

		// �о������ʱ��ʹ�þ�������Ĵ�С��Χ���ж��Ƿ񳬳���ͼ�߽�
		if (entity.HasComponent<SpriteComponent>())
		{
			const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			isOutOfMap = (position.x + spriteComponent.size.x * transformComponent.scale.x < 0 ||
				position.x > mapWidth ||
				position.y + spriteComponent.size.y * transformComponent.scale.y < 0 ||
				position.y > mapHeight);
		}
		// û�о������ʱ��ͨ�������ж��Ƿ񳬳���ͼ�߽�
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
