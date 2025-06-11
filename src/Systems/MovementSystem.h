#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

#include <iomanip>
#include <sstream> 

class MovementSystem : public System 
{
	inline static const std::string NAME = std::string("MovementSystem") + U8_TO_CHARPTR("运动系统"); // 系统名称

public:
	MovementSystem() 
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();
	}

	void Update(double deltaTime)
	{
		for (auto& entity : GetSystemEntities())
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
			
			transformComponent.position.x += rigidBodyComponent.volocity.x * deltaTime;
			transformComponent.position.y += rigidBodyComponent.volocity.y * deltaTime;

			std::ostringstream oss;
			oss << U8_TO_CHARPTR("更新，实体(id=") << entity.GetId()
				<< U8_TO_CHARPTR(")位置为")
				<< std::fixed << std::setprecision(2)  // 设置保留两位小数
				<< transformComponent.position.x << ", "
				<< transformComponent.position.y;
			
			std::string message = oss.str();
			Logger::Instance().Log(LogLevel::INFO, MovementSystem::NAME + message);
		}
	}
};

#endif // !MOVEMENTSYSTEM_H
