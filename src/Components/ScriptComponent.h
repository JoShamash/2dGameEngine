#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include <sol/sol.hpp>
#include <vector>

#include "../Logger/Logger.h"

/**
* @struct ScriptComponent
* @brief 脚本（Lua）组件，用于绑定Lua脚本到实体上
*/
struct ScriptComponent {
	inline const static std::string NAME = std::string("ScriptComponent") + U8_TO_CHARPTR("脚本组件"); // 组件名称


	std::vector<sol::safe_function> funcVec;

	/**
	 * @brief 构造函数
	*/
	ScriptComponent() = default;
};

#endif // !SCRIPTCOMPONENT_H
