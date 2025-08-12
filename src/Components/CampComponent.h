#ifndef CAMPCOMPONENT_H
#define CAMPCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

// 定义阵营枚举
enum class Camp : Uint8
{
	Friend = 0,		// 友方
	Enemy,			// 敌方
	Neutral,		// 中立方
	None,			// 无阵营，通常为静态物

	Count
};

// 定义阵营枚举对应的字符串映射·中文
inline const char* CampText(const Camp& camp)
{
	switch (camp)
	{
		case Camp::Friend: 
			return U8_TO_CHARPTR("友方");
		case Camp::Enemy: 
			return U8_TO_CHARPTR("敌方");
		case Camp::Neutral: 
			return U8_TO_CHARPTR("中立方");
		case Camp::None:
			return U8_TO_CHARPTR("无阵营");
		default:
			return U8_TO_CHARPTR("未知阵营");
	}
}

inline Camp CampFromString(const std::string& str)
{
	if (Registry::is_same_word(str, "Friend") || str == U8_TO_CHARPTR("友方"))
	{
		return Camp::Friend;
	}
	else if (Registry::is_same_word(str, "Enemy") || str == U8_TO_CHARPTR("敌方"))
	{
		return Camp::Enemy;
	}
	else if (Registry::is_same_word(str, "Neutral") || str == U8_TO_CHARPTR("中立方"))
	{
		return Camp::Neutral;
	}
	else if (Registry::is_same_word(str, "None") || str == U8_TO_CHARPTR("无阵营"))
	{
		return Camp::None;
	}
	else
	{
		std::string message = U8_TO_CHARPTR("未知的阵营字符串：") + str + U8_TO_CHARPTR("，默认设置为友方");
		Logger::Instance().Log(LogLevel::WARN, message);
		return Camp::Friend;
	}
}

/**
* @struct CampComponent
* @brief 阵营组件，用于定义阵营属性
*/
struct CampComponent {
	inline const static std::string NAME = std::string("CampComponent") + U8_TO_CHARPTR("阵营组件"); // 组件名称


	Camp camp;	// 阵营

	/**
	 * @brief 构造函数
	 * @param Camp	阵营
	*/
	CampComponent(Camp camp = Camp::Friend)
		: camp(camp) { }

	CampComponent(const std::string& campStr)
		: camp(CampFromString(campStr)) { }
};

#endif // !CAMPCOMPONENT_H
