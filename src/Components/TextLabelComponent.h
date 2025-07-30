#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct TextLabelComponent
* @brief �ı���ǩ�����������ʾ�ı���Ϣ
*/
struct TextLabelComponent{
	inline const static std::string NAME = std::string("TextLabelComponent") + U8_TO_CHARPTR("�ı���ǩ���"); // �������


	glm::i64vec2 position;		// �ı�λ��
	std::string text;			// �ı�����
	std::string assetId;		// ������ԴID
	SDL_Color color;			// �ı���ɫ
	bool isFixed;				// ��Ⱦ��ʽ���Ƿ�̶�

	/**
	 * @brief ���캯��
	 * @param position �ı�λ��
	 * @param text �ı�����
	 * @param assetId ������ԴID
	 * @param color �ı���ɫ
	 * @param isFixed ��Ⱦ��ʽ���Ƿ�̶�
	*/
	TextLabelComponent(glm::i64vec2 position = {0, 0}, std::string text = "", std::string assetId = "", SDL_Color color = {0, 0, 0, 0}, bool isFixed = false)
		: position(position), text(text), assetId(assetId), color(color), isFixed(isFixed) { }
};

#endif // !TEXTLABELCOMPONENT_H
