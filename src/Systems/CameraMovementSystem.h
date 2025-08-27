#ifndef  CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../Camera/Camera.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/CameraFollowComponent.h"

#include <algorithm>

class CameraMovementSystem : public System
{
public:
	inline static const std::string NAME = std::string("CameraMovementSystem") + U8_TO_CHARPTR("������ƶ�ϵͳ"); // ϵͳ����

	CameraMovementSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<CameraFollowComponent>();
	}

	void Update(Camera& camera)
	{
        auto& entities = GetEntities();
        auto entityNum = entities.size();

        if (entityNum == 1)
        {
            auto& entity = entities.front();
            const auto& transformComponent = entity.GetComponent<TransformComponent>();
            const auto& position = transformComponent.position;

            // ����Ŀ�������λ�ã�ʹʵ�����
            double targetX = position.x - (camera.w / 2.0f);
            double targetY = position.y - (camera.h / 2.0f);

            // ����Ŀ��λ���ڵ�ͼ��Χ��
            targetX = std::clamp(targetX, 0.0, GameEngine::mapWidth - camera.w);
            targetY = std::clamp(targetY, 0.0, GameEngine::mapHeight - camera.h);

            if(!camera.isSmoothEnabled())
            {
                // ���δ����ƽ����ֱ������λ��
                camera.x = targetX;
                camera.y = targetY;
                return;
			}
            else
            {
                // ʹ�����Բ�ֵƽ�����������λ��
				double smoothFactor = camera.getSmoothFactor();
                camera.x = Lerp(camera.x, targetX, smoothFactor);
                camera.y = Lerp(camera.y, targetY, smoothFactor);
            }
            
        }
        else if (entityNum > 1)
        {
            // ����������󶨣������߼�����������ʵ��İ�Χ��������ΪĿ��λ��
            double minX = std::numeric_limits<double>::max();
            double minY = std::numeric_limits<double>::max();
            double maxX = std::numeric_limits<double>::lowest();
            double maxY = std::numeric_limits<double>::lowest();

            for (const auto& entity : entities)
            {
                const auto& transformComponent = entity.GetComponent<TransformComponent>();
                const auto& position = transformComponent.position;

                if (position.x < minX) minX = position.x;
                if (position.y < minY) minY = position.y;
                if (position.x > maxX) maxX = position.x;
                if (position.y > maxY) maxY = position.y;
            }

            // �����Χ������
            double centerX = (minX + maxX) / 2.0;
            double centerY = (minY + maxY) / 2.0;

            // ����Ŀ�������λ�ã�ʹ��Χ�����ľ���
            double targetX = centerX - (camera.w / 2.0);
            double targetY = centerY - (camera.h / 2.0);

            // ����Ŀ��λ���ڵ�ͼ��Χ��
            targetX = std::clamp(targetX, 0.0, GameEngine::mapWidth - camera.w);
            targetY = std::clamp(targetY, 0.0, GameEngine::mapHeight - camera.h);

            if (!camera.isSmoothEnabled())
            {
                // ���δ����ƽ����ֱ������λ��
                camera.x = targetX;
                camera.y = targetY;
                return;
            }
            else
            {
                // ʹ�����Բ�ֵƽ�����������λ��
                double smoothFactor = camera.getSmoothFactor();
                camera.x = Lerp(camera.x, targetX, smoothFactor);
                camera.y = Lerp(camera.y, targetY, smoothFactor);
            }
        }
	}

private:
	// ���Բ�ֵ����
    template <typename T>
	T Lerp(T a, T b, float t)
	{
		return a + t * (b - a);
	}
};

#endif // ! CAMERAMOVEMENTSYSTEM_H
