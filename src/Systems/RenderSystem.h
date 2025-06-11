#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL2_image/SDL_image.h>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

class RenderSystem : public System
{
	inline static const std::string NAME = std::string("RenderSystem") + U8_TO_CHARPTR("渲染系统"); // 系统名称

public:
	RenderSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore)
	{
		for (auto& entity : GetSystemEntities())
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

			std::string message = U8_TO_CHARPTR("渲染，实体(id=") + std::to_string(entity.GetId()) + ")";
			Logger::Instance().Log(LogLevel::INFO, RenderSystem::NAME + message);
		}
	}
};

#endif // !RENDERSYSTEM_H
