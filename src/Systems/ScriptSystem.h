#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include <optional>

#include "../ECS/ECS.h"
#include "../Time/Timer.h"
#include "../Camera/Camera.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/ScriptComponent.h"

class ScriptSystem : public System
{
public:
	inline static const std::string NAME = std::string("ScriptSystem") + U8_TO_CHARPTR("脚本系统"); // 系统名称
	 

	ScriptSystem()
	{
		RequireComponent<ScriptComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	void Update(std::unique_ptr<Registry>& registry, SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera)
	{
		for (const auto& entity : GetEntities())
		{
			const auto& scriptComponent = entity.GetComponent<ScriptComponent>();
			const auto& funcVec = scriptComponent.funcVec;
			
			for (const auto& func : funcVec)
			{
				auto funcResult = func(entity);
				if (!funcResult.valid())
				{
					sol::error err = funcResult;
					std::string message = U8_TO_CHARPTR("脚本执行错误，实体id=") + std::to_string(entity.GetId()) + U8_TO_CHARPTR("，错误信息：") + std::string(err.what());
					Logger::Instance().Log(LogLevel::ERROR, message);
				}
			}
		}
	}

	void LuaBindCpp(sol::state& luaState)
	{
		LuaAddBasicTypes(luaState);
		LuaAddTimerType(luaState);
		LuaAddEntityType(luaState);
		LuaAddAllComponentTypes(luaState);
		LuaAddAllFuncTypes(luaState);
	}

private:
	void LuaAddBasicTypes(sol::state& luaState)
	{
		luaState.new_usertype<glm::f64vec2>(
			"f64vec2",
			sol::constructors<glm::f64vec2(), glm::f64vec2(double, double)>(),
			"x", &glm::f64vec2::x,
			"y", &glm::f64vec2::y
		);

		luaState.new_usertype<glm::i64vec2>(
			"i64vec2",
			sol::constructors<glm::i64vec2(), glm::i64vec2(int64_t, int64_t)>(),
			"x", &glm::i64vec2::x,
			"y", &glm::i64vec2::y
		);

		luaState.new_usertype<SDL_Color>(
			"rgba_color",
			sol::constructors<SDL_Color()>(),
			"r", &SDL_Color::r,
			"g", &SDL_Color::g,
			"b", &SDL_Color::b,
			"a", &SDL_Color::a
		);

		luaState.new_enum<Camp>("Camp",
			{
				{"Friend", Camp::Friend},
				{"Enemy", Camp::Enemy},
				{"Neutral", Camp::Neutral},
				{"None", Camp::None},
				{"Count", Camp::Count}
			}
		);

		luaState.new_enum<RenderLayer>("RenderLayer", 
			{
				{"BackgroundFar", RenderLayer::BackgroundFar},
				{"BackgroundNear", RenderLayer::BackgroundNear},
				{"Terrain", RenderLayer::Terrain},
				{"StaticGeometry", RenderLayer::StaticGeometry},
				{"Decals", RenderLayer::Decals},
				{"DynamicGeometry", RenderLayer::DynamicGeometry},
				{"Characters", RenderLayer::Characters},
				{"Player", RenderLayer::Player},
				{"Weapons", RenderLayer::Weapons},
				{"Projectiles", RenderLayer::Projectiles},
				{"EffectsUnder", RenderLayer::EffectsUnder},
				{"EffectsOver", RenderLayer::EffectsOver},
				{"Particles", RenderLayer::Particles},
				{"UIBackground", RenderLayer::UIBackground},
				{"UI", RenderLayer::UI},
				{"UIForeground", RenderLayer::UIForeground},
				{"Overlay", RenderLayer::Overlay},
				{"PostProcessing", RenderLayer::PostProcessing},
				{"Debug", RenderLayer::Debug},
				{"Count", RenderLayer::Count}
			}
		);
	}

	void LuaAddTimerType(sol::state& luaState)
	{
		luaState.new_usertype<Time>(
			"time",

			// 构造函数
			sol::constructors<Time()>(),

			// 成员变量绑定
			"gameTime", &Time::gameTime,
			"deltaTime", &Time::deltaTime,
			"timeScale", &Time::timeScale,
			"paused", &Time::paused,
			"lastRealTime", &Time::lastRealTime,

			// 成员函数绑定
			"reset", &Time::reset
		);

		luaState.new_usertype<Timer>(
			"timer",

			// 构造函数
			sol::constructors<Timer()>(),

			// 静态成员变量通过函数绑定
			"get_time", []() -> Time& { return Timer::time; },
			"get_targetFrameTime", []() -> Uint64 { return Timer::targetFrameTime; },

			// 成员函数绑定
			"pause", &Timer::pause,
			"resume", &Timer::resume,
			"update", &Timer::update
		);
	}

	void LuaAddEntityType(sol::state& luaState)
	{
		luaState.new_usertype<Entity>(
			"Entity",
			sol::constructors<Entity(glm::uint64 id), Entity(const Entity & entity)>(),
			"get_id", &Entity::GetId,

			"get_tag", &Entity::GetTag,
			"add_tag", &Entity::AddTag,
			"remove_tag", &Entity::RemoveTag,
			"has_tag", &Entity::HasTag,
			"change_tag", &Entity::ChangeTag,

			"get_group", &Entity::GetGroup,
			"add_group", &Entity::AddGroup,
			"remove_group", &Entity::RemoveGroup,
			"has_group", &Entity::HasGroup,
			"change_group", &Entity::ChangeGroup,

			"killself", &Entity::KillSelf,

			"add_component", sol::overload(
				[](Entity& entity, TransformComponent component) { entity.AddComponent<TransformComponent>(component); },
				[](Entity& entity, RigidBodyComponent component) { entity.AddComponent<RigidBodyComponent>(component); },
				[](Entity& entity, CampComponent component) { entity.AddComponent<CampComponent>(component); },
				[](Entity& entity, SpriteComponent component) { entity.AddComponent<SpriteComponent>(component); },
				[](Entity& entity, AnimationComponent component) { entity.AddComponent<AnimationComponent>(component); },
				[](Entity& entity, BoxColliderComponent component) { entity.AddComponent<BoxColliderComponent>(component); },
				[](Entity& entity, KeyboardControlComponent component) { entity.AddComponent<KeyboardControlComponent>(component); },
				[](Entity& entity, CameraFollowComponent component) { entity.AddComponent<CameraFollowComponent>(component); },
				[](Entity& entity, ProjectileEmitterComponent component) { entity.AddComponent<ProjectileEmitterComponent>(component); },
				[](Entity& entity, ProjectileComponent component) { entity.AddComponent<ProjectileComponent>(component); },
				[](Entity& entity, HealthComponent component) { entity.AddComponent<HealthComponent>(component); },
				[](Entity& entity, TextLabelComponent component) { entity.AddComponent<TextLabelComponent>(component); },
				[](Entity& entity, HealthBarComponent component) { entity.AddComponent<HealthBarComponent>(component); },
				[](Entity& entity, EntityLifeCycleComponent component) { entity.AddComponent<EntityLifeCycleComponent>(component); },
				[](Entity& entity, ScriptComponent component) { entity.AddComponent<ScriptComponent>(component); }
			),
			"remove_component", [](Entity& entity, const std::string& component)
			{
				if (component == "TransformComponent")
					entity.RemoveComponent<TransformComponent>();
				else if (component == "RigidBodyComponent")
					entity.RemoveComponent<RigidBodyComponent>();
				else if (component == "CampComponent")
					entity.RemoveComponent<CampComponent>();
				else if (component == "SpriteComponent")
					entity.RemoveComponent<SpriteComponent>();
				else if (component == "AnimationComponent")
					entity.RemoveComponent<AnimationComponent>();
				else if (component == "BoxColliderComponent")
					entity.RemoveComponent<BoxColliderComponent>();
				else if (component == "KeyboardControlComponent")
					entity.RemoveComponent<KeyboardControlComponent>();
				else if (component == "CameraFollowComponent")
					entity.RemoveComponent<CameraFollowComponent>();
				else if (component == "ProjectileEmitterComponent")
					entity.RemoveComponent<ProjectileEmitterComponent>();
				else if (component == "ProjectileComponent")
					entity.RemoveComponent<ProjectileComponent>();
				else if (component == "HealthComponent")
					entity.RemoveComponent<HealthComponent>();
				else if (component == "TextLabelComponent")
					entity.RemoveComponent<TextLabelComponent>();
				else if (component == "HealthBarComponent")
					entity.RemoveComponent<HealthBarComponent>();
				else if (component == "EntityLifeCycleComponent")
					entity.RemoveComponent<EntityLifeCycleComponent>();
				else if (component == "ScriptComponent")
					entity.RemoveComponent<ScriptComponent>();
			},
			"has_component", [](Entity& entity, const std::string& component)
			{
				if (component == "TransformComponent")
					return entity.HasComponent<TransformComponent>();
				else if (component == "RigidBodyComponent")
					return entity.HasComponent<RigidBodyComponent>();
				else if (component == "CampComponent")
					return entity.HasComponent<CampComponent>();
				else if (component == "SpriteComponent")
					return entity.HasComponent<SpriteComponent>();
				else if (component == "AnimationComponent")
					return entity.HasComponent<AnimationComponent>();
				else if (component == "BoxColliderComponent")
					return entity.HasComponent<BoxColliderComponent>();
				else if (component == "KeyboardControlComponent")
					return entity.HasComponent<KeyboardControlComponent>();
				else if (component == "CameraFollowComponent")
					return entity.HasComponent<CameraFollowComponent>();
				else if (component == "ProjectileEmitterComponent")
					return entity.HasComponent<ProjectileEmitterComponent>();
				else if (component == "ProjectileComponent")
					return entity.HasComponent<ProjectileComponent>();
				else if (component == "HealthComponent")
					return entity.HasComponent<HealthComponent>();
				else if (component == "TextLabelComponent")
					return entity.HasComponent<TextLabelComponent>();
				else if (component == "HealthBarComponent")
					return entity.HasComponent<HealthBarComponent>();
				else if (component == "EntityLifeCycleComponent")
					return entity.HasComponent<EntityLifeCycleComponent>();
				else if (component == "ScriptComponent")
					return entity.HasComponent<ScriptComponent>();
				return false;
			},
				"get_transformComponent", [](Entity& entity) -> TransformComponent*
				{
					if (entity.HasComponent<TransformComponent>())
					{
						return &entity.GetComponent<TransformComponent>();
					}
					return nullptr;
				},
				"get_rigidBodyComponent", [](Entity& entity) -> RigidBodyComponent*
				{
					if (entity.HasComponent<RigidBodyComponent>())
					{
						return &entity.GetComponent<RigidBodyComponent>();
					}
					return nullptr;
				},
				"get_campComponent", [](Entity& entity) -> CampComponent*
				{
					if (entity.HasComponent<CampComponent>())
					{
						return &entity.GetComponent<CampComponent>();
					}
					return nullptr;
				},
				"get_spriteComponent", [](Entity& entity) -> SpriteComponent*
				{
					if (entity.HasComponent<SpriteComponent>())
					{
						return &entity.GetComponent<SpriteComponent>();
					}
					return nullptr;
				},
				"get_animationComponent", [](Entity& entity) -> AnimationComponent*
				{
					if (entity.HasComponent<AnimationComponent>())
					{
						return &entity.GetComponent<AnimationComponent>();
					}
					return nullptr;
				},
				"get_boxColliderComponent", [](Entity& entity) -> BoxColliderComponent*
				{
					if (entity.HasComponent<BoxColliderComponent>())
					{
						return &entity.GetComponent<BoxColliderComponent>();
					}
					return nullptr;
				},
				"get_keyboardControlComponent", [](Entity& entity) -> KeyboardControlComponent*
				{
					if (entity.HasComponent<KeyboardControlComponent>())
					{
						return &entity.GetComponent<KeyboardControlComponent>();
					}
					return nullptr;
				},
				"get_cameraFollowComponent", [](Entity& entity) -> CameraFollowComponent*
				{
					if (entity.HasComponent<CameraFollowComponent>())
					{
						return &entity.GetComponent<CameraFollowComponent>();
					}
					return nullptr;
				},
				"get_projectileEmitterComponent", [](Entity& entity) -> ProjectileEmitterComponent*
				{
					if (entity.HasComponent<ProjectileEmitterComponent>())
					{
						return &entity.GetComponent<ProjectileEmitterComponent>();
					}
					return nullptr;
				},
				"get_projectileComponent", [](Entity& entity) -> ProjectileComponent*
				{
					if (entity.HasComponent<ProjectileComponent>())
					{
						return &entity.GetComponent<ProjectileComponent>();
					}
					return nullptr;
				},
				"get_healthComponent", [](Entity& entity) -> HealthComponent*
				{
					if (entity.HasComponent<HealthComponent>())
					{
						return &entity.GetComponent<HealthComponent>();
					}
					return nullptr;
				},
				"get_textLabelComponent", [](Entity& entity) -> TextLabelComponent*
				{
					if (entity.HasComponent<TextLabelComponent>())
					{
						return &entity.GetComponent<TextLabelComponent>();
					}
					return nullptr;
				},
				"get_healthBarComponent", [](Entity& entity) -> HealthBarComponent*
				{
					if (entity.HasComponent<HealthBarComponent>())
					{
						return &entity.GetComponent<HealthBarComponent>();
					}
					return nullptr;
				},
				"get_entityLifeCycleComponent", [](Entity& entity) -> EntityLifeCycleComponent*
				{
					if (entity.HasComponent<EntityLifeCycleComponent>())
					{
						return &entity.GetComponent<EntityLifeCycleComponent>();
					}
					return nullptr;
				},
				"get_scriptComponent", [](Entity& entity) -> ScriptComponent*
				{
					if (entity.HasComponent<ScriptComponent>())
					{
						return &entity.GetComponent<ScriptComponent>();
					}
					return nullptr;
				}
		);
	}

	void LuaAddAllComponentTypes(sol::state& luaState)
	{
		LuaAddTransformComponentType(luaState);
		LuaAddRigidBodyComponentType(luaState);
		LuaAddCampComponentType(luaState);
		LuaAddSpriteComponentType(luaState);
		LuaAddAnimationComponentType(luaState);
		LuaAddBoxColliderComponentType(luaState);
		LuaAddKeyboardControlComponentType(luaState);
		LuaAddCameraFollowComponentType(luaState);
		LuaAddProjectileEmitterComponentType(luaState);
		LuaAddProjectileComponentType(luaState);
		LuaAddHealthComponentType(luaState);
		LuaAddTextLabelComponentType(luaState);
		LuaAddHealthBarComponentType(luaState);
		LuaAddEntityLifeCycleComponentType(luaState);
		LuaAddScriptComponentType(luaState);
	}

	void LuaAddAllFuncTypes(sol::state& luaState)
	{
		LuaAddGetTimer(luaState);
	}

private:
	void LuaAddTransformComponentType(sol::state& luaState)
	{
		luaState.new_usertype<TransformComponent>(
			"transformComponent",
			sol::constructors<TransformComponent(glm::f64vec2 position, double rotation, glm::f64vec2 scale)>(),
			"position", &TransformComponent::position,
			"rotation", &TransformComponent::rotation,
			"scale", & TransformComponent::scale,
			"print_addr", [](TransformComponent& cp)
			{
				std::ostringstream oss;
				oss << &cp;
				std::string addr_str = oss.str();
				Logger::Instance().Log(LogLevel::TRACE, addr_str);
			}
		);
	}

	void LuaAddRigidBodyComponentType(sol::state& luaState)
	{
		luaState.new_usertype<RigidBodyComponent>(
			"rigidBodyComponent",
			sol::constructors<RigidBodyComponent(double speed, glm::f64vec2 velocity)>(),
			"speed", &RigidBodyComponent::speed,
			"maxSpeed", &RigidBodyComponent::maxSpeed,
			"velocity", &RigidBodyComponent::velocity
		);
	}

	void LuaAddCampComponentType(sol::state& luaState)
	{
		luaState.new_usertype<CampComponent>(
			"campComponent",
			sol::constructors<CampComponent(Camp camp), CampComponent(const std::string& campStr)>(),
			"camp", &CampComponent::camp
		);
	}

	void LuaAddSpriteComponentType(sol::state& luaState)
	{
		luaState.new_usertype<SpriteComponent>(
			"spriteComponent",
			sol::constructors<SpriteComponent(std::string assetId, glm::f64vec2 size, RenderLayer layer, bool isDirSheet, glm::f64vec2 position)>(),
			"assetId", &SpriteComponent::assetId,
			"size", &SpriteComponent::size,
			"layer", &SpriteComponent::layer,
			"isDirSheet", &SpriteComponent::isDirSheet
		);
	}

	void LuaAddAnimationComponentType(sol::state& luaState)
	{
		luaState.new_usertype<AnimationComponent>(
			"animationComponent",
			sol::constructors<AnimationComponent(Uint64 numFrames, Uint64 frameSpeed, bool isLooping)>(),
			"numFrames", &AnimationComponent::numFrames,
			"currentFrame", &AnimationComponent::currentFrame,
			"frameSpeed", &AnimationComponent::frameSpeed,
			"isLooping", &AnimationComponent::isLooping,
			"startTime", &AnimationComponent::startTime
		);
	}

	void LuaAddBoxColliderComponentType(sol::state& luaState)
	{
		luaState.new_usertype<BoxColliderComponent>(
			"boxColliderComponent",
			sol::constructors< BoxColliderComponent(const glm::f64vec2& size , const glm::f64vec2& offset) >(),
			"size", &BoxColliderComponent::size,
			"offset", &BoxColliderComponent::offset,
			"frameSpeed", &BoxColliderComponent::isCollision
		);
	}

	void LuaAddKeyboardControlComponentType(sol::state& luaState)
	{
		luaState.new_usertype<KeyboardControlComponent>(
			"keyboardControlComponent",
			sol::constructors<KeyboardControlComponent()>(),
			"isFire", &KeyboardControlComponent::isFire,
			"isMove", &KeyboardControlComponent::isMove,
			"upVelocity", &KeyboardControlComponent::upVelocity,
			"rightVelocity", &KeyboardControlComponent::rightVelocity,
			"downVelocity", &KeyboardControlComponent::downVelocity,
			"leftVelocity", &KeyboardControlComponent::leftVelocity
		);
	}

	void LuaAddCameraFollowComponentType(sol::state& luaState)
	{
		luaState.new_usertype<CameraFollowComponent>(
			"cameraFollowComponent",
			sol::constructors<CameraFollowComponent()>()
		);
	}

	void LuaAddProjectileEmitterComponentType(sol::state& luaState)
	{
		luaState.new_usertype<ProjectileEmitterComponent>(
			"projectileEmitterComponent",
			sol::constructors<ProjectileEmitterComponent(bool isEmit, int repeatFrequency, int projectileDuration, double hitDamage, double speed, Uint64 lastEmissionTime)>(),
			"isEmit", &ProjectileEmitterComponent::isEmit,
			"repeatFrequency", &ProjectileEmitterComponent::repeatFrequency,
			"projectileDuration", &ProjectileEmitterComponent::projectileDuration,
			"hitDamage", &ProjectileEmitterComponent::hitDamage,
			"speed", &ProjectileEmitterComponent::speed,
			"lastEmissionTime", &ProjectileEmitterComponent::lastEmissionTime
		);
	}

	void LuaAddProjectileComponentType(sol::state& luaState)
	{
		luaState.new_usertype<ProjectileComponent>(
			"projectileComponent",
			sol::constructors<ProjectileComponent(Uint64 duration, Uint64 hitDamage)>(),
			"hitDamage", &ProjectileComponent::hitDamage,
			"duration", &ProjectileComponent::duration,
			"createTime", &ProjectileComponent::createTime
		);
	}

	void LuaAddHealthComponentType(sol::state& luaState)
	{
		luaState.new_usertype<HealthComponent>(
			"projectileComponent",
			sol::constructors<HealthComponent(double maxHp)>(),
			"hp", &HealthComponent::hp,
			"maxHp", &HealthComponent::maxHp
		);
	}

	void LuaAddTextLabelComponentType(sol::state& luaState)
	{
		luaState.new_usertype<TextLabelComponent>(
			"textLabelComponent",
			sol::constructors<TextLabelComponent(glm::i64vec2 position, std::string text, std::string assetId, SDL_Color color, bool isFixed)>(),
			"position", &TextLabelComponent::position,
			"text", &TextLabelComponent::text,
			"assetId", &TextLabelComponent::assetId,
			"color", &TextLabelComponent::color,
			"isFixed", &TextLabelComponent::isFixed
		);
	}

	void LuaAddHealthBarComponentType(sol::state& luaState)
	{
		luaState.new_usertype<HealthBarComponent>(
			"healthBarComponent",
			sol::constructors<HealthBarComponent(std::string assetId, glm::i64vec2 size, SDL_Color borderColor, SDL_Color fillColor, SDL_Color backgroundColor)>(),
			"assetId", &HealthBarComponent::assetId,
			"size", &HealthBarComponent::size,
			"borderColor", &HealthBarComponent::borderColor,
			"fillColor", &HealthBarComponent::fillColor,
			"backgroundColor", &HealthBarComponent::backgroundColor
		);
	}

	void LuaAddEntityLifeCycleComponentType(sol::state& luaState)
	{
		luaState.new_usertype<EntityLifeCycleComponent>(
			"entityLifeCycleComponent",
			sol::constructors<EntityLifeCycleComponent()>(),
			"isDead", &EntityLifeCycleComponent::isDead,
			"isDestroyed", &EntityLifeCycleComponent::isDestroyed,
			"isOutOfMap", &EntityLifeCycleComponent::isOutOfMap,
			"deathTime", &EntityLifeCycleComponent::deathTime,
			"outOfMapTime", &EntityLifeCycleComponent::outOfMapTime,
			"destroyTime", &EntityLifeCycleComponent::destroyTime,
			"destroyDelay", &EntityLifeCycleComponent::destroyDelay,
			"outOfMapDelay", &EntityLifeCycleComponent::outOfMapDelay
		);
	}

	void LuaAddScriptComponentType(sol::state& luaState)
	{
		luaState.new_usertype<ScriptComponent>(
			"scriptComponent",
			sol::constructors<ScriptComponent()>()
		);
	}

private:
	void LuaAddGetTimer(sol::state& luaState)
	{
		luaState.set_function("get_timer", []() { return GameEngine::timer; });
	}
};

#endif // !SCRIPTSYSTEM_H
