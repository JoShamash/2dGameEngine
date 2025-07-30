#ifndef HEALTHBARCOMPONENT_H
#define HEALTHBARCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>

#include "../Logger/Logger.h"

/**
* @struct HealthBarComponent
* @brief 生命条组件，用于显示实体的生命值条
*/
struct HealthBarComponent {
	inline const static std::string NAME = std::string("HealthBarComponent") + U8_TO_CHARPTR("生命值条组件"); // 组件名称

	std::string assetId;		// 字体资源ID
	glm::i64vec2 size;			// 生命值条大小
	SDL_Color borderColor;		// 边框颜色，默认为白色
	SDL_Color fillColor;		// 填充颜色&文本颜色，默认为白色
	SDL_Color backgroundColor;	// 背景颜色，默认为灰色半透明

	HealthBarComponent(std::string assetId = "", glm::i64vec2 size = {50, 5}, SDL_Color borderColor = {255, 255, 255, 255}, SDL_Color fillColor = {255, 255, 255, 255}, SDL_Color backgroundColor = {128, 128, 128, 128})
		: assetId(assetId), size(size), borderColor(borderColor), fillColor(fillColor), backgroundColor(backgroundColor) { }
};

#endif // !HEALTHBARCOMPONENT_H
