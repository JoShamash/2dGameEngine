#ifndef CAMPCOMPONENT_H
#define CAMPCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

// ������Ӫö��
enum class Camp : Uint8
{
	Friend = 0,		// �ѷ�
	Enemy,			// �з�
	Neutral,		// ������

	Count
};

// ������Ӫö�ٶ�Ӧ���ַ���ӳ�䡤����
inline const char* CampText(const Camp& camp)
{
	switch (camp)
	{
		case Camp::Friend: 
			return U8_TO_CHARPTR("�ѷ�");
		case Camp::Enemy: 
			return U8_TO_CHARPTR("�з�");
		case Camp::Neutral: 
			return U8_TO_CHARPTR("������");
		default:
			return U8_TO_CHARPTR("δ֪��Ӫ");
	}
}

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
