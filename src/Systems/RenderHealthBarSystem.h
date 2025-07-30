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
	inline static const std::string NAME = std::string("RenderHealthBarSystem") + U8_TO_CHARPTR("��Ⱦ����ֵ��ϵͳ"); // ϵͳ����

	RenderHealthBarSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<HealthComponent>();
		RequireComponent<HealthBarComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	 * @brief ������Ⱦ����ֵ��ϵͳ����Ⱦ����ʵ�������ֵ��
	 * @param renderer SDL ��Ⱦ����������Ⱦ����ֵ��
	 */
	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera)
	{
		for (const auto& entity : GetEntities())
		{
			const auto& transformComponent = entity.GetComponent<TransformComponent>();
			const auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			const auto& healthComponent = entity.GetComponent<HealthComponent>();
			auto& healthBarComponent = entity.GetComponent<HealthBarComponent>();

			// ������ͼʵ�ʴ�С
			const glm::i64vec2 spriteSize = {
				spriteComponent.size.x * transformComponent.scale.x,
				spriteComponent.size.y * transformComponent.scale.y
			};

			// ��������ֵ������Ч����
			double healthRatio = static_cast<double>(healthComponent.hp) / healthComponent.maxHp;
			healthBarComponent.size.x = spriteSize.x * 1.2; // ����ֵ�����Ϊ�����ȵ�120%
			int barWidth = healthBarComponent.size.x * healthRatio;

			// �������ֵ�����С�ڵ���0��������ֵС�ڵ���0���򲻻���
			if (barWidth <= 0 || healthComponent.hp <= 0)
			{
				continue;
			}

			// ��������ֵ�ȼ�������ֵ����ɫ
			HealthLevel healthLevel;
			auto& healthBarColor = healthBarComponent.fillColor;
			GetHealthLevelAndColor(healthRatio, healthLevel, healthBarColor);

			// ��������ֵ��λ��
			glm::i64vec2 healthBarPosition = {
				transformComponent.position.x + spriteSize.x / 2.0 - healthBarComponent.size.x / 2.0,
				transformComponent.position.y - healthBarComponent.size.y - 10 // ����ֵ����ʵ���Ϸ���λ�ã�����ʵ�嶥������10���صĸ߶Ⱦ���
			};

			// ��������ֵ������
			SDL_Rect backgroundRect = {
				healthBarPosition.x,
				healthBarPosition.y,
				healthBarComponent.size.x,
				healthBarComponent.size.y
			};
			backgroundRect.x -= camera.x; // ��ȥ�����ƫ��X
			backgroundRect.y -= camera.y; // ��ȥ�����ƫ��Y
			SDL_SetRenderDrawColor(renderer, healthBarComponent.backgroundColor.r, healthBarComponent.backgroundColor.g,
				healthBarComponent.backgroundColor.b, healthBarComponent.backgroundColor.a);
			SDL_RenderFillRect(renderer, &backgroundRect);
				
			// ��������ֵ��
			SDL_Rect healthBarRect = {
				healthBarPosition.x,
				healthBarPosition.y,
				barWidth,
				healthBarComponent.size.y
			};
			healthBarRect.x -= camera.x; // ��ȥ�����ƫ��X
			healthBarRect.y -= camera.y; // ��ȥ�����ƫ��Y
			SDL_SetRenderDrawColor(renderer, healthBarColor.r, healthBarColor.g, healthBarColor.b, healthBarColor.a);
			SDL_RenderFillRect(renderer, &healthBarRect);

			// ���Ʊ߿�
			SDL_SetRenderDrawColor(renderer, healthBarComponent.borderColor.r, healthBarComponent.borderColor.g,
				healthBarComponent.borderColor.b, healthBarComponent.borderColor.a);
			SDL_RenderDrawRect(renderer, &healthBarRect);

			// ��������ֵ�ı����ٷֱ�
			std::string healthText = std::to_string(static_cast<int>(healthRatio * 100)) + "%";
			const auto& healthBarAssetId = healthBarComponent.assetId;
			auto healthBarRightPosition = glm::i64vec2{	// ��������ֵ���Ҳഹֱ����λ��
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
			healthBarColor = { 255, 0, 0, 255 }; // ��ɫ
		}
		else if (healthRatio <= 0.5)
		{
			healthLevel = HealthLevel::Low;
			healthBarColor = { 255, 165, 0, 255 }; // ��ɫ
		}
		else if (healthRatio <= 0.8)
		{
			healthLevel = HealthLevel::Good;
			healthBarColor = { 255, 255, 0, 255 }; // ��ɫ
		}
		else
		{
			healthLevel = HealthLevel::Healthy;
			healthBarColor = { 0, 255, 0, 255 }; // ��ɫ
		}
	}

	void RenderHealthText(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera, const std::string& assetId, const glm::i64vec2& healthBarRightPosition, const std::string& text, const SDL_Color& color)
	{
		// ��ȡ������Դ
		auto fontAsset = assetStore->GetFontAsset(assetId);
		if (!fontAsset)
		{
			std::string message = U8_TO_CHARPTR("������Դδ�ҵ�����Դid=") + assetId;
			Logger::Instance().Log(LogLevel::ERROR, message);
			return;
		}

		SDL_Surface* surface = TTF_RenderUTF8_Blended(fontAsset, text.c_str(), color);
		if (!surface)
		{
			std::string message = U8_TO_CHARPTR("��Ⱦ�ı�ʧ�ܣ���Դid=") + assetId;
			Logger::Instance().Log(LogLevel::ERROR, message);
			return;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (!texture)
		{
			std::string message = U8_TO_CHARPTR("��������ʧ�ܣ���Դid=") + assetId;
			Logger::Instance().Log(LogLevel::ERROR, message);
			return;
		}

		int labelWidth, labelHeight;
		SDL_QueryTexture(texture, nullptr, nullptr, &labelWidth, &labelHeight);

		// ������Ⱦλ��������ֵ���Ҳ࣬��ȥ�������ƫ��
		glm::i64vec2 textPosition = {
			healthBarRightPosition.x + 5,				// �Ҳ�ƫ��5����
			healthBarRightPosition.y - labelHeight / 2	// ��ֱ����
		};
		SDL_Rect dstRect = {
			textPosition.x - camera.x, // ��ȥ�����ƫ��X
			textPosition.y - camera.y, // ��ȥ�����ƫ��Y
			labelWidth,
			labelHeight
		};

		SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
		SDL_DestroyTexture(texture);
	}
};


#endif // !RENDERHEALTHBARSYSTEM_H