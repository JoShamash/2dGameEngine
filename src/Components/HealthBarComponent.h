#ifndef HEALTHBARCOMPONENT_H
#define HEALTHBARCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>

#include "../Logger/Logger.h"

/**
* @struct HealthBarComponent
* @brief �����������������ʾʵ�������ֵ��
*/
struct HealthBarComponent {
	inline const static std::string NAME = std::string("HealthBarComponent") + U8_TO_CHARPTR("����ֵ�����"); // �������

	std::string assetId;		// ������ԴID
	glm::i64vec2 size;			// ����ֵ����С
	SDL_Color borderColor;		// �߿���ɫ��Ĭ��Ϊ��ɫ
	SDL_Color fillColor;		// �����ɫ&�ı���ɫ��Ĭ��Ϊ��ɫ
	SDL_Color backgroundColor;	// ������ɫ��Ĭ��Ϊ��ɫ��͸��

	HealthBarComponent(std::string assetId = "", glm::i64vec2 size = {50, 5}, SDL_Color borderColor = {255, 255, 255, 255}, SDL_Color fillColor = {255, 255, 255, 255}, SDL_Color backgroundColor = {128, 128, 128, 128})
		: assetId(assetId), size(size), borderColor(borderColor), fillColor(fillColor), backgroundColor(backgroundColor) { }
};

#endif // !HEALTHBARCOMPONENT_H
