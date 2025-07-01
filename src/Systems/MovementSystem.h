#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

#include <iomanip>
#include <sstream> 

// �˶�ϵͳ�࣬����һϵ���߼��ӿڣ�������ʵ����˶��߼�
class MovementSystem : public System 
{
public:
	inline static const std::string NAME = std::string("MovementSystem") + U8_TO_CHARPTR("�˶�ϵͳ"); // ϵͳ����

	MovementSystem() 
	{
		RequireComponent<TransformComponent>();
		RequireComponent<RigidBodyComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief �����˶�״̬������ʵ����˶��߼�
	* @param deltaTime ʱ����������ʾ���ϴθ�������������ʱ��
	*/
	void Update()
	{
		for (const auto& entity : GetEntities())
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
			
			transformComponent.position.x += rigidBodyComponent.velocity.x * rigidBodyComponent.speed * Timer::time.deltaTime / 1000.0;
			transformComponent.position.y += rigidBodyComponent.velocity.y * rigidBodyComponent.speed * Timer::time.deltaTime / 1000.0;

			//std::ostringstream oss;
			//oss << U8_TO_CHARPTR("���£�ʵ��(id=") << entity.GetId()
			//	<< U8_TO_CHARPTR(")λ��Ϊ")
			//	<< std::fixed << std::setprecision(2)  // ���ñ�����λС��
			//	<< transformComponent.position.x << ", "
			//	<< transformComponent.position.y;
			//
			//std::string message = oss.str();
			//Logger::Instance().Log(LogLevel::INFO, MovementSystem::NAME + message);
		}
	}
};

#endif // !MOVEMENTSYSTEM_H
