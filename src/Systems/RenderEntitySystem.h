#ifndef RENDERENTITYSYSTEM_H
#define RENDERENTITYSYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Camera/Camera.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/AnimationComponent.h"

#include <map>
#include <ranges>

// ��Ⱦʵ��ϵͳ�࣬����һϵ���߼��ӿڣ�������Ⱦʵ���ͼ����Ļ��
class RenderEntitySystem : public System
{
public:
	inline static const std::string NAME = std::string("RenderEntitySystem") + U8_TO_CHARPTR("��Ⱦʵ��ϵͳ"); // ϵͳ����

	RenderEntitySystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief ������Ⱦʵϵͳ����Ⱦ����ʵ���ͼ��
	* @param renderer SDL ��Ⱦ����������Ⱦͼ��
	* @param assetStore ��Դ�洢�����ڻ�ȡͼ����Դ
	*/
	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera)
	{
		std::unordered_multimap<RenderLayer, Entity> entities; // �洢��Ⱦϵͳ�е�ʵ�壬����Ⱦ�����

		SDL_Rect cameraView = {0, 0, camera.w, camera.h};

		for (const auto& entity : GetEntities())
		{
			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			entities.emplace(spriteComponent.layer, entity);
		}

		for (int renderLayer = 0; renderLayer < static_cast<int>(RenderLayer::Count); renderLayer++)
		{
			auto [first, last] = entities.equal_range(static_cast<RenderLayer>(renderLayer));
			auto range = std::ranges::subrange(first, last);

			for (const auto& [layer, entity] : range)
			{
				auto& transformComponent = entity.GetComponent<TransformComponent>();
				auto& spriteComponent = entity.GetComponent<SpriteComponent>();
				auto& layer = spriteComponent.layer;
				SDL_Rect srcRect = spriteComponent.srcRect;

				if (entity.HasComponent<RigidBodyComponent>() && entity.HasComponent<AnimationComponent>())
				{
					const auto& size = spriteComponent.size;
					// �����˶���������Դ���ε�y����

					const auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
					const auto& velocity = rigidBodyComponent.velocity;

					if (spriteComponent.isDirSheet)
					{
						if (fabs(velocity.x) >= fabs(velocity.y) && velocity.x > 0)
						{
							srcRect.y = size.y * static_cast<int>(DirectionTexture::Right);
						}
						else if (fabs(velocity.x) >= fabs(velocity.y) && velocity.x < 0)
						{
							srcRect.y = size.y * static_cast<int>(DirectionTexture::Left);
						}
						else if (fabs(velocity.x) < fabs(velocity.y) && velocity.y > 0)
						{
							srcRect.y = size.y * static_cast<int>(DirectionTexture::Down);
						}
						else if (fabs(velocity.x) < fabs(velocity.y) && velocity.y < 0)
						{
							srcRect.y = size.y * static_cast<int>(DirectionTexture::Up);
						}
					}
				}

				SDL_Rect dstRect = {
					static_cast<int>(transformComponent.position.x),
					static_cast<int>(transformComponent.position.y),
					static_cast<int>(spriteComponent.size.x * transformComponent.scale.x),
					static_cast<int>(spriteComponent.size.y * transformComponent.scale.y)
				};

				if (layer < RenderLayer::UIBackground)
				{
					dstRect.x -= camera.x;
					dstRect.y -= camera.y;
				}

				// ���Ŀ����ƾ����Ƿ��������ͼ�ڲ�
				if (!SDL_HasIntersection(&dstRect, &cameraView))
				{
					continue;
				}

				SDL_RenderCopyEx(
					renderer,
					assetStore->GetImageAsset(spriteComponent.assetId),
					&srcRect,
					&dstRect,
					transformComponent.rotation,
					NULL,
					SDL_FLIP_NONE
				);
			}
		}
	}
};

#endif // !RENDERENTITYSYSTEM_H
