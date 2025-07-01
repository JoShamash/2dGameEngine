#ifndef HEALTHSYSTEM_H
#define HEALTHSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/HealthComponent.h"

class HealthSystem : public System
{
public:
	inline static const std::string NAME = std::string("HealthSystem") + U8_TO_CHARPTR("生命值系统"); // 系统名称


	HealthSystem()
	{
		RequireComponent<HealthComponent>();
	}

	void Update()
	{
		for (const auto& entity : GetEntities())
		{
			const auto& healthComponent = entity.GetComponent<HealthComponent>();

			if (healthComponent.hp <= 0)
			{
				entity.KillSelf();
			}
		}
	}
};

#endif // !HEALTHSYSTEM_H
