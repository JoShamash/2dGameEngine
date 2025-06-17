#ifndef RENDERENTITYSYSTEM_H
#define RENDERENTITYSYSTEM_H

#include <SDL2_image/SDL_image.h>
#include <map>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

// ��Ⱦʵ��ϵͳ�࣬������Ⱦʵ���ͼ����Ļ��
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
	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore)
	{
		std::multimap<RenderLayer, Entity> entities; // �洢��Ⱦϵͳ�е�ʵ�壬����Ⱦ�����

		for (const auto& entity : GetEntities())
		{
			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			entities.insert({spriteComponent.layer, entity});
		}

		for (const auto& [layer, entity] : entities)
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			
			SDL_Rect srcRect = spriteComponent.srcRect;

			SDL_Rect dstRect = {
				static_cast<int>(transformComponent.position.x),
				static_cast<int>(transformComponent.position.y),
				static_cast<int>(spriteComponent.size.x * transformComponent.scale.x),
				static_cast<int>(spriteComponent.size.y * transformComponent.scale.y)
			};

			SDL_RenderCopyEx(
				renderer, 
				assetStore->GetImageAsset(spriteComponent.assetId), 
				&srcRect, 
				&dstRect, 
				transformComponent.rotation,
				NULL,
				SDL_FLIP_NONE
			);
			//std::string message = U8_TO_CHARPTR("��Ⱦ��ʵ��(id=") + std::to_string(entity.GetId()) + ")";
			//Logger::Instance().Log(LogLevel::INFO, RenderEntitySystem::NAME + message);
		}
	}
};

#endif // !RENDERENTITYSYSTEM_H
