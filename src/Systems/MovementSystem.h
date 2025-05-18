#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

#include <iomanip>
#include <sstream> 

class MovementSystem : public System 
{
	inline static const std::string NAME = std::string("MovementSystem") + reinterpret_cast<const char*>(u8"�˶�ϵͳ"); // ϵͳ����

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
			oss << reinterpret_cast<const char*>(u8"���£�ʵ��(id=") << entity.GetId()
				<< reinterpret_cast<const char*>(u8")λ��Ϊ")
				<< std::fixed << std::setprecision(2)  // ���ñ�����λС��
				<< transformComponent.position.x << ", "
				<< transformComponent.position.y;
			
			std::string message = oss.str();
			Logger::Instance().Log(LogLevel::INFO, MovementSystem::NAME + message);
		}
	}
};

#endif // !MOVEMENTSYSTEM_H
