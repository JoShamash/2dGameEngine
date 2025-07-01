#ifndef CAMPCOMPONENT_H
#define CAMPCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

enum class Camp : Uint8
{
	Friend = 0,		// �ѷ�
	Enemy,			// �з�
	Neutral,		// ������

	Count
};

/**
* @struct CampComponent
* @brief ��Ӫ��������ڶ�����Ӫ����
*/
struct CampComponent {
	inline const static std::string NAME = std::string("CampComponent") + U8_TO_CHARPTR("��Ӫ���"); // �������


	Camp camp;	// ��Ӫ

	/**
	 * @brief ���캯��
	 * @param Camp	��Ӫ
	*/
	CampComponent(Camp camp = Camp::Friend)
		: camp(camp) { }
};

#endif // !CAMPCOMPONENT_H
