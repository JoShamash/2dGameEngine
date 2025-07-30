#ifndef RENDERTEXTSYSTEM_H
#define RENDERTEXTSYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Camera/Camera.h"
#include "../Components/TextLabelComponent.h"

// 渲染文本系统类，定义一系列逻辑接口，负责渲染文本标签到屏幕上
class RenderTextSystem : public System
{
public:
	inline static const std::string NAME = std::string("RenderTextSystem") + U8_TO_CHARPTR("渲染文本系统"); // 系统名称

	RenderTextSystem()
	{
		RequireComponent<TextLabelComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief 更新渲染文本系统，渲染所有文本标签
	* @param renderer SDL 渲染器，用于渲染文本
	* @param assetStore 资源存储，用于获取字体资源
	*/
	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera)
	{
		for (const auto& entity : GetEntities())
		{
			auto& textLabelComponent = entity.GetComponent<TextLabelComponent>();
			auto fontAsset = assetStore->GetFontAsset(textLabelComponent.assetId);
			if (!fontAsset)
			{
				std::string message = U8_TO_CHARPTR("字体资源未找到，实体id=") + std::to_string(entity.GetId());
				Logger::Instance().Log(LogLevel::ERROR, message);
				continue;
			}

			SDL_Surface* surface = TTF_RenderUTF8_Blended(fontAsset, textLabelComponent.text.c_str(), textLabelComponent.color);
			if (!surface)
			{
				std::string message = U8_TO_CHARPTR("渲染文本失败，实体id=") + std::to_string(entity.GetId());
				Logger::Instance().Log(LogLevel::ERROR, message);
				continue;
			}

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			if (!texture)
			{
				std::string message = U8_TO_CHARPTR("创建纹理失败，实体id=") + std::to_string(entity.GetId());
				Logger::Instance().Log(LogLevel::ERROR, message);
				continue;
			}

			// 设置渲染位置，如果文本标签不是固定位置，则需要减去摄像机的偏移
			if (!textLabelComponent.isFixed)
			{
				textLabelComponent.position.x -= camera.x;
				textLabelComponent.position.y -= camera.y;
			}

			int labelWidth, labelHeight;
			SDL_QueryTexture(texture, nullptr, nullptr, &labelWidth, &labelHeight);

			SDL_Rect dstRect = {
				static_cast<int>(textLabelComponent.position.x),
				static_cast<int>(textLabelComponent.position.y),
				labelWidth,
				labelHeight
			};
			SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
			SDL_DestroyTexture(texture);
		}
	}
};

#endif // !RENDERTEXTSYSTEM_H
