#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

// 运动系统类，定义一系列逻辑接口，负责处理实体的运动逻辑
class MovementSystem : public System 
{
public:
	inline static const std::string NAME = std::string("MovementSystem") + U8_TO_CHARPTR("运动系统"); // 系统名称

	MovementSystem() 
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief 更新运动状态，处理实体的运动逻辑
	* @param deltaTime 时间增量，表示自上次更新以来经过的时间
	*/
	void Update()
	{
		for (const auto& entity : GetEntities())
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
			
			transformComponent.position.x += rigidBodyComponent.velocity.x * rigidBodyComponent.speed * Timer::time.deltaTime / 1000.0;
			transformComponent.position.y += rigidBodyComponent.velocity.y * rigidBodyComponent.speed * Timer::time.deltaTime / 1000.0;
		}
	}
};

#endif // !MOVEMENTSYSTEM_H
