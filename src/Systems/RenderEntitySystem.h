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

// 渲染实体系统类，定义一系列逻辑接口，负责渲染实体的图像到屏幕上
class RenderEntitySystem : public System
{
public:
	inline static const std::string NAME = std::string("RenderEntitySystem") + U8_TO_CHARPTR("渲染实体系统"); // 系统名称

	RenderEntitySystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief 更新渲染实系统，渲染所有实体的图像
	* @param renderer SDL 渲染器，用于渲染图像
	* @param assetStore 资源存储，用于获取图像资源
	*/
	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera)
	{
		std::multimap<RenderLayer, Entity> entities; // 存储渲染系统中的实体，按渲染层分类

		SDL_Rect cameraView = {0, 0, camera.w, camera.h};

		for (const auto& entity : GetEntities())
		{
			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			entities.insert({spriteComponent.layer, entity});
		}

		for (const auto& [layer, entity] : entities)
		{
			auto& transformComponent = entity.GetComponent<TransformComponent>();
			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			auto& layer = spriteComponent.layer;
			SDL_Rect srcRect = spriteComponent.srcRect;

			if (entity.HasComponent<RigidBodyComponent>() && entity.HasComponent<AnimationComponent>())
			{
				const auto& size = spriteComponent.size;
				// 根据运动方向设置源矩形的y坐标
				
				const auto& rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();
				const auto& velocity = rigidBodyComponent.velocity;
				
				if(fabs(velocity.x) > fabs(velocity.y) && velocity.x > 0)
				{
					srcRect.y = size.y * static_cast<int>(DirectionTexture::Right);
				}
				else if(fabs(velocity.x) > fabs(velocity.y) && velocity.x < 0)
				{
					srcRect.y = size.y * static_cast<int>(DirectionTexture::Left);
				}
				else if(fabs(velocity.x) < fabs(velocity.y) && velocity.y > 0)
				{
					srcRect.y = size.y * static_cast<int>(DirectionTexture::Down);
				}
				else if(fabs(velocity.x) < fabs(velocity.y) && velocity.y < 0)
				{
					srcRect.y = size.y * static_cast<int>(DirectionTexture::Up);
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

			// 检查目标绘制矩形是否在相机视图内部
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
			//std::string message = U8_TO_CHARPTR("渲染，实体(id=") + std::to_string(entity.GetId()) + ")";
			//Logger::Instance().Log(LogLevel::INFO, RenderEntitySystem::NAME + message);
		}
	}
};

#endif // !RENDERENTITYSYSTEM_H
