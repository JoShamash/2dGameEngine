#ifndef RENDERCOLLIDERSYSTEM_H
#define RENDERCOLLIDERSYSTEM_H

// �����ɫ Uint32 �� Uint8 R����
#define COLOR_R(c)  ((Uint8)(((c) >> 24) & 0xFF))
// �����ɫ Uint32 �� Uint8 G����
#define COLOR_G(c)  ((Uint8)(((c) >> 16) & 0xFF))
// �����ɫ Uint32 �� Uint8 B����
#define COLOR_B(c)  ((Uint8)(((c) >>  8) & 0xFF))
// �����ɫ Uint32 �� Uint8 A����
#define COLOR_A(c)  ((Uint8)(((c) >>  0) & 0xFF))

#include "../ECS/ECS.h"
#include "../Camera/Camera.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

// ������Ⱦ������ײ״̬��ɫö��
enum class BoxColliderStateColor : Uint32
{
	// ��ɫ��δ��ײ (R=0, G=255, B=0, A=255)
	NoCollision = 0x00FF00FF,

	// ��ɫ������ײ (R=255, G=0, B=0, A=255)
	Collision = 0xFF0000FF
};

// ��Ⱦ������ײϵͳ�࣬����һϵ���߼��ӿڣ����ڵ���ʱ��Ⱦ������ײ
class RenderColliderSystem : public System
{
public:
	inline static const std::string NAME = std::string("RenderBoxColliderSystem") + U8_TO_CHARPTR("��Ⱦ������ײϵͳ"); // ϵͳ����

	RenderColliderSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief ��Ⱦ������ײϵͳ����������ʵ�岢��ȾAABB�������߽��
	* @param renderer SDL ��Ⱦ����������Ⱦͼ��
	*/
	void Update(SDL_Renderer* renderer, const Camera& camera)
	{
		SDL_Rect window = { 0, 0, camera.w, camera.h };

		for (const auto& entity : GetEntities())
		{
			const auto& transformComponent = entity.GetComponent<TransformComponent>();
			const auto& boxColliderComponent = entity.GetComponent<BoxColliderComponent>();

			auto x1 = transformComponent.position.x + boxColliderComponent.offset.x - camera.x;
			auto y1 = transformComponent.position.y + boxColliderComponent.offset.y - camera.y;
			auto x2 = boxColliderComponent.size.x * transformComponent.scale.x;
			auto y2 = boxColliderComponent.size.y * transformComponent.scale.y;
			SDL_Rect rect = { x1, y1, x2, y2 };

			if (!SDL_HasIntersection(&rect, &window))
			{
				continue;
			}

			auto color = static_cast<Uint32>(boxColliderComponent.isCollision ? BoxColliderStateColor::Collision : BoxColliderStateColor::NoCollision);
			SDL_SetRenderDrawColor(renderer, COLOR_R(color), COLOR_G(color), COLOR_B(color), COLOR_A(color));
			SDL_RenderDrawRect(renderer, &rect);
		}
	}
};

#endif // !RENDERCOLLIDERSYSTEM_H
