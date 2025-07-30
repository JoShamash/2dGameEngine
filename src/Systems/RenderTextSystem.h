#ifndef RENDERTEXTSYSTEM_H
#define RENDERTEXTSYSTEM_H

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../Camera/Camera.h"
#include "../Components/TextLabelComponent.h"

// ��Ⱦ�ı�ϵͳ�࣬����һϵ���߼��ӿڣ�������Ⱦ�ı���ǩ����Ļ��
class RenderTextSystem : public System
{
public:
	inline static const std::string NAME = std::string("RenderTextSystem") + U8_TO_CHARPTR("��Ⱦ�ı�ϵͳ"); // ϵͳ����

	RenderTextSystem()
	{
		RequireComponent<TextLabelComponent>();

		std::string message = U8_TO_CHARPTR("���ϵͳ��") + NAME;
		Logger::Instance().Log(LogLevel::INFO, message);
	}

	/**
	* @brief ������Ⱦ�ı�ϵͳ����Ⱦ�����ı���ǩ
	* @param renderer SDL ��Ⱦ����������Ⱦ�ı�
	* @param assetStore ��Դ�洢�����ڻ�ȡ������Դ
	*/
	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, const Camera& camera)
	{
		for (const auto& entity : GetEntities())
		{
			auto& textLabelComponent = entity.GetComponent<TextLabelComponent>();
			auto fontAsset = assetStore->GetFontAsset(textLabelComponent.assetId);
			if (!fontAsset)
			{
				std::string message = U8_TO_CHARPTR("������Դδ�ҵ���ʵ��id=") + std::to_string(entity.GetId());
				Logger::Instance().Log(LogLevel::ERROR, message);
				continue;
			}

			SDL_Surface* surface = TTF_RenderUTF8_Blended(fontAsset, textLabelComponent.text.c_str(), textLabelComponent.color);
			if (!surface)
			{
				std::string message = U8_TO_CHARPTR("��Ⱦ�ı�ʧ�ܣ�ʵ��id=") + std::to_string(entity.GetId());
				Logger::Instance().Log(LogLevel::ERROR, message);
				continue;
			}

			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);
			if (!texture)
			{
				std::string message = U8_TO_CHARPTR("��������ʧ�ܣ�ʵ��id=") + std::to_string(entity.GetId());
				Logger::Instance().Log(LogLevel::ERROR, message);
				continue;
			}

			// ������Ⱦλ�ã�����ı���ǩ���ǹ̶�λ�ã�����Ҫ��ȥ�������ƫ��
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
