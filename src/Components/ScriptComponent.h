#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include <sol/sol.hpp>
#include <vector>

#include "../Logger/Logger.h"

/**
* @struct ScriptComponent
* @brief �ű���Lua����������ڰ�Lua�ű���ʵ����
*/
struct ScriptComponent {
	inline const static std::string NAME = std::string("ScriptComponent") + U8_TO_CHARPTR("�ű����"); // �������


	std::vector<sol::safe_function> funcVec;

	/**
	 * @brief ���캯��
	*/
	ScriptComponent() = default;
};

#endif // !SCRIPTCOMPONENT_H
