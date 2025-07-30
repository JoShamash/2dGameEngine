#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct TextLabelComponent
* @brief 文本标签组件，用于显示文本信息
*/
struct TextLabelComponent{
	inline const static std::string NAME = std::string("TextLabelComponent") + U8_TO_CHARPTR("文本标签组件"); // 组件名称


	glm::i64vec2 position;		// 文本位置
	std::string text;			// 文本内容
	std::string assetId;		// 字体资源ID
	SDL_Color color;			// 文本颜色
	bool isFixed;				// 渲染方式，是否固定

	/**
	 * @brief 构造函数
	 * @param position 文本位置
	 * @param text 文本内容
	 * @param assetId 字体资源ID
	 * @param color 文本颜色
	 * @param isFixed 渲染方式，是否固定
	*/
	TextLabelComponent(glm::i64vec2 position = {0, 0}, std::string text = "", std::string assetId = "", SDL_Color color = {0, 0, 0, 0}, bool isFixed = false)
		: position(position), text(text), assetId(assetId), color(color), isFixed(isFixed) { }
};

#endif // !TEXTLABELCOMPONENT_H
