#ifndef PROJECTILELIFEACYCLESYSTEM_H
#define PROJECTILELIFEACYCLESYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/ProjectileComponent.h"
#include "../Time/Timer.h"

class ProjectileLifeCycleSystem : public System
{
public:
	inline static const std::string NAME = std::string("ProjectileLifeCycleSystem") + U8_TO_CHARPTR("�������������ڹ���ϵͳ"); // ϵͳ����


	ProjectileLifeCycleSystem()
	{
		RequireComponent<ProjectileComponent>();
	}

	void Update()
	{
		for (const auto& entity : GetEntities())
		{
			const auto& projectileComponent = entity.GetComponent<ProjectileComponent>();

			bool timeToKill = Timer::time.gameTime >= projectileComponent.createTime + projectileComponent.duration;
			if (timeToKill)
			{
				entity.KillSelf();
			}
		}
	}
};

#endif // !PROJECTILELIFEACYCLESYSTEM_H
