#ifndef RENDERHEALTHBARSYSTEM_H
#define RENDERHEALTHBARSYSTEM_H

#include "../ECS/ECS.h"
#include "../Camera/Camera.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/HealthBarComponent.h"

#include <cmath>

class RenderHealthBarSystem : public System
{
public:
	inline static const std::string NAME = std::string("RenderHealthBarSystem") + U8_TO_CHARPTR("渲染生命值条系统"); // 系统名称

	RenderHealthBarSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<HealthComponent>();
		RequireComponent<HealthBarComponent>();

		std::string message = U8_TO_CHARPTR("添加系统：") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	 * @brief 更新渲染生命值条系统，渲染所有实体的生命值条
	 * @param renderer SDL 渲染器，用于渲染生命值条
	 */
	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera)
	{
		for (const auto& entity : GetEntities())
		{
			const auto& transformComponent = entity.GetComponent<TransformComponent>();
			const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			const auto& healthComponent = entity.GetComponent<HealthComponent>();
			auto& healthBarComponent = entity.GetComponent<HealthBarComponent>();

			// 计算贴图实际大小
			const glm::i64vec2 spriteSize = {
				spriteComponent.size.x * transformComponent.scale.x,
				spriteComponent.size.y * transformComponent.scale.y
			};

			// 计算生命值条的有效长度
			double healthRatio = static_cast<double>(healthComponent.hp) / healthComponent.maxHp;
			healthBarComponent.size.x = spriteSize.x * 1.2; // 生命值条宽度为精灵宽度的120%
			int barWidth = healthBarComponent.size.x * healthRatio;

			// 如果生命值条宽度小于等于0或者生命值小于等于0，则不绘制
			if (barWidth <= 0 || healthComponent.hp <= 0)
			{
				continue;
			}

			// 计算生命值等级和生命值条颜色
			HealthLevel healthLevel;
			auto& healthBarColor = healthBarComponent.fillColor;
			GetHealthLevelAndColor(healthRatio, healthLevel, healthBarColor);

			// 计算生命值条位置
			glm::i64vec2 healthBarPosition = {
				transformComponent.position.x + spriteSize.x / 2.0 - healthBarComponent.size.x / 2.0,
				transformComponent.position.y - healthBarComponent.size.y - 10 // 生命值条在实体上方的位置，距离实体顶部自身10像素的高度距离
			};

			// 绘制生命值条背景
			SDL_Rect backgroundRect = {
				healthBarPosition.x,
				healthBarPosition.y,
				healthBarComponent.size.x,
				healthBarComponent.size.y
			};
			backgroundRect.x -= camera.x; // 减去摄像机偏移X
			backgroundRect.y -= camera.y; // 减去摄像机偏移Y
			SDL_SetRenderDrawColor(renderer, healthBarComponent.backgroundColor.r, healthBarComponent.backgroundColor.g,
				healthBarComponent.backgroundColor.b, healthBarComponent.backgroundColor.a);
			SDL_RenderFillRect(renderer, &backgroundRect);
				
			// 绘制生命值条
			SDL_Rect healthBarRect = {
				healthBarPosition.x,
				healthBarPosition.y,
				barWidth,
				healthBarComponent.size.y
			};
			healthBarRect.x -= camera.x; // 减去摄像机偏移X
			healthBarRect.y -= camera.y; // 减去摄像机偏移Y
			SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, healthBarColor.a);
			SDL_RenderFillRect(renderer, &healthBarRect);

			// 绘制边框
			SDL_SetRenderDrawColor(renderer, healthBarComponent.borderColor.r, healthBarComponent.borderColor.g,
				healthBarComponent.borderColor.b, healthBarComponent.borderColor.a);
			SDL_RenderDrawRect(renderer, &healthBarRect);

			// 绘制生命值文本，百分比
			std::string healthText = std::to_string(static_cast<int>(healthRatio * 100)) + "%";
			const auto& healthBarAssetId = healthBarComponent.assetId;
			auto healthBarRightPosition = glm::i64vec2{	// 计算生命值条右侧垂直居中位置
				healthBarPosition.x + healthBarComponent.size.x,
				healthBarPosition.y + healthBarComponent.size.y / 2
			};
			RenderHealthText(renderer, assetStore, camera, healthBarAssetId, healthBarRightPosition, healthText, healthBarColor);
		}
	}

private:
	void GetHealthLevelAndColor(double healthRatio, HealthLevel& healthLevel, SDL_Color& healthBarColor)
	{
		if (healthRatio <= 0.2)
		{
			healthLevel = HealthLevel::Danger;
			healthBarColor = { 255, 0, 0, 255 }; // 红色
		}
		else if (healthRatio <= 0.5)
		{
			healthLevel = HealthLevel::Low;
			healthBarColor = { 255, 165, 0, 255 }; // 橙色
		}
		else if (healthRatio <= 0.8)
		{
			healthLevel = HealthLevel::Good;
			healthBarColor = { 255, 255, 0, 255 }; // 黄色
		}
		else
		{
			healthLevel = HealthLevel::Healthy;
			healthBarColor = { 0, 255, 0, 255 }; // 绿色
		}
	}

	void RenderHealthText(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera, const std::string& assetId, const glm::i64vec2& healthBarRightPosition, const std::string& text, const SDL_Color& color)
	{
		// 获取字体资源
		auto fontAsset = assetStore->GetFontAsset(assetId);
		if (!fontAsset)
		{
			std::string message = U8_TO_CHARPTR("字体资源未找到，资源id=") + assetId;
			Logger::Instance().Log(LogLevel::ERROR, message);
			return;
		}

		SDL_Surface* surface = TTF_RenderUTF8_Blended(fontAsset, text.c_str(), color);
		if (!surface)
		{
			std::string message = U8_TO_CHARPTR("渲染文本失败，资源id=") + assetId;
			Logger::Instance().Log(LogLevel::ERROR, message);
			return;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (!texture)
		{
			std::string message = U8_TO_CHARPTR("创建纹理失败，资源id=") + assetId;
			Logger::Instance().Log(LogLevel::ERROR, message);
			return;
		}

		int labelWidth, labelHeight;
		SDL_QueryTexture(texture, nullptr, nullptr, &labelWidth, &labelHeight);

		// 设置渲染位置在生命值条右侧，减去摄像机的偏移
		glm::i64vec2 textPosition = {
			healthBarRightPosition.x + 5,				// 右侧偏移5像素
			healthBarRightPosition.y - labelHeight / 2	// 垂直居中
		};
		SDL_Rect dstRect = {
			textPosition.x - camera.x, // 减去摄像机偏移X
			textPosition.y - camera.y, // 减去摄像机偏移Y
			labelWidth,
			labelHeight
		};

		SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
		SDL_DestroyTexture(texture);
	}
};


#endif // !RENDERHEALTHBARSYSTEM_H