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
	None,			// ����Ӫ��ͨ��Ϊ��̬��

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
		case Camp::None:
			return U8_TO_CHARPTR("����Ӫ");
		default:
			return U8_TO_CHARPTR("δ֪��Ӫ");
	}
}

inline Camp CampFromString(const std::string& str)
{
	if (Registry::is_same_word(str, "Friend") || str == U8_TO_CHARPTR("�ѷ�"))
	{
		return Camp::Friend;
	}
	else if (Registry::is_same_word(str, "Enemy") || str == U8_TO_CHARPTR("�з�"))
	{
		return Camp::Enemy;
	}
	else if (Registry::is_same_word(str, "Neutral") || str == U8_TO_CHARPTR("������"))
	{
		return Camp::Neutral;
	}
	else if (Registry::is_same_word(str, "None") || str == U8_TO_CHARPTR("����Ӫ"))
	{
		return Camp::None;
	}
	else
	{
		std::string message = U8_TO_CHARPTR("δ֪����Ӫ�ַ�����") + str + U8_TO_CHARPTR("��Ĭ������Ϊ�ѷ�");
		Logger::Instance().Log(LogLevel::WARN, message);
		return Camp::Friend;
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

	CampComponent(const std::string& campStr)
		: camp(CampFromString(campStr)) { }
};

#endif // !CAMPCOMPONENT_H
