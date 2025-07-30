#ifndef RENDERCOLLIDERSYSTEM_H
#define RENDERCOLLIDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Camera/Camera.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

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
			SDL_Rect rect = {x1, y1, x2, y2};

			if (!SDL_HasIntersection(&rect, &window))
			{
				continue;
			}

			auto color = GetColorFromState(boxColliderComponent.isCollision);
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderDrawRect(renderer, &rect);
		}
	}

private:
	SDL_Color GetColorFromState(bool isCollision)
	{
		if (isCollision)
		{
			return { 255, 0, 0, 255 }; // ��ɫ������ײ (R=255, G=0, B=0, A=255)
		}
		else
		{
			return { 0, 255, 0, 255 }; // ��ɫ��δ��ײ (R=0, G=255, B=0, A=255)
		}
	}
};

#endif // !RENDERCOLLIDERSYSTEM_H
