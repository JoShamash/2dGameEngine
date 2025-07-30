#ifndef PROJECTILEEMITSYSTEM_H
#define PROJECTILEEMITSYSTEM_H

#include <memory>
#include <cmath>

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CampComponent.h"
#include "../Components/EntityLifeCycleComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Time/Timer.h"

class ProjectileEmitSystem : public System
{
public:
	inline static const std::string NAME = std::string("ProjectileEmitSystem") + U8_TO_CHARPTR("发射器系统"); // 系统名称


	ProjectileEmitSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<ProjectileEmitterComponent>();
	}

	void SubscribeEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeEvent<KeyPressedEvent>(this, &ProjectileEmitSystem::OnKeyPressed);
	}

	void OnKeyPressed(KeyPressedEvent& event)
	{
		if (event.key == SDLK_SPACE)
		{
			for (const auto& entity : GetEntities())
			{
				if (entity.HasTag("player"))
				{
					auto& projectileEmitterComponent = entity.GetComponent<ProjectileEmitterComponent>();
					projectileEmitterComponent.isEmit = true;

					if (projectileEmitterComponent.isEmit == false || projectileEmitterComponent.projectileDuration == 0 || projectileEmitterComponent.repeatFrequency == 0)
					{
						return;
					}

					const auto& transformComponent = entity.GetComponent<TransformComponent>();
					const auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
					const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
					const auto& campComponent = entity.GetComponent<CampComponent>();
					const auto& position = transformComponent.position;
					const auto& size = spriteComponent.size;

					bool timeToEmit = projectileEmitterComponent.lastEmissionTime + projectileEmitterComponent.repeatFrequency <= Timer::time.gameTime;
					if (projectileEmitterComponent.lastEmissionTime == 0 || timeToEmit)
					{
						Entity projectile = entity.GetRegistry()->CreateEntity();
						projectile.AddGroup("projectile");
						TransformComponent transformComponent1({ position.x + size.x / 2, position.y + size.y / 2 });
						RigidBodyComponent rigidBodyComponent1(rigidBodyComponent.speed + projectileEmitterComponent.speed, rigidBodyComponent.velocity);
						SpriteComponent spriteComponent1("bullet", { 4, 4 }, RenderLayer::Projectiles);
						BoxColliderComponent boxColliderComponent1(spriteComponent1.size);
						ProjectileComponent projectileComponent1(projectileEmitterComponent.projectileDuration, projectileEmitterComponent.hitDamage);
						CampComponent campComponent1(campComponent.camp);

						projectile.AddComponent<TransformComponent>(transformComponent1);
						projectile.AddComponent<RigidBodyComponent>(rigidBodyComponent1);
						projectile.AddComponent<SpriteComponent>(spriteComponent1);
						projectile.AddComponent<BoxColliderComponent>(boxColliderComponent1);
						projectile.AddComponent<ProjectileComponent>(projectileComponent1);
						projectile.AddComponent<CampComponent>(campComponent1);
						projectile.AddComponent<EntityLifeCycleComponent>();

						projectileEmitterComponent.lastEmissionTime = Timer::time.gameTime;
					}

					projectileEmitterComponent.isEmit = false;
				}
			}
		}
	}

	void Update(std::unique_ptr<Registry>& registry)
	{
		for (const auto& entity : GetEntities())
		{
			auto& projectileEmitterComponent = entity.GetComponent<ProjectileEmitterComponent>();
			if (projectileEmitterComponent.isEmit == false || projectileEmitterComponent.projectileDuration == 0 || projectileEmitterComponent.repeatFrequency == 0)
			{
				continue;
			}

			const auto& transformComponent = entity.GetComponent<TransformComponent>();
			const auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
			const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			const auto& campComponent = entity.GetComponent<CampComponent>();
			const auto& position = transformComponent.position;
			const auto& size = spriteComponent.size;

			bool timeToEmit = projectileEmitterComponent.lastEmissionTime + projectileEmitterComponent.repeatFrequency <= Timer::time.gameTime;
			if (projectileEmitterComponent.lastEmissionTime == 0 || timeToEmit)
			{
				Entity projectile = registry->CreateEntity();
				projectile.AddGroup("projectile");

				TransformComponent transformComponent1({ position.x + size.x / 2, position.y + size.y / 2 });
				RigidBodyComponent rigidBodyComponent1(rigidBodyComponent.speed + projectileEmitterComponent.speed, rigidBodyComponent.velocity);
				SpriteComponent spriteComponent1("bullet", { 4, 4 }, RenderLayer::Projectiles);
				BoxColliderComponent boxColliderComponent1(spriteComponent1.size);
				ProjectileComponent projectileComponent1(projectileEmitterComponent.projectileDuration, projectileEmitterComponent.hitDamage);
				CampComponent campComponent1(campComponent.camp);

				projectile.AddComponent<TransformComponent>(transformComponent1);
				projectile.AddComponent<RigidBodyComponent>(rigidBodyComponent1);
				projectile.AddComponent<SpriteComponent>(spriteComponent1);
				projectile.AddComponent<BoxColliderComponent>(boxColliderComponent1);
				projectile.AddComponent<ProjectileComponent>(projectileComponent1);
				projectile.AddComponent<CampComponent>(campComponent1);
				projectile.AddComponent<EntityLifeCycleComponent>();

				projectileEmitterComponent.lastEmissionTime = Timer::time.gameTime;
			}
		}
	}
};

#endif // !PROJECTILEEMITSYSTEM_H
