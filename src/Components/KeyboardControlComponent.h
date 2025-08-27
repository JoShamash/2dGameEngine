#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct KeyboardControlComponent
* @brief ���̿�����������ڴ�����̿���ʵ����ƶ��ٶȷ��򣬳�����RigidBodyComponent����ʹ�û�������ٶ�ʸ����
*/
struct KeyboardControlComponent {
	inline const static std::string NAME = std::string("KeyboardControlComponent") + U8_TO_CHARPTR("���̿������"); // �������

	glm::f64vec2 upVelocity = {0, -1};		// ���ϡ��ƶ����ٶȷ���
	glm::f64vec2 rightVelocity = {1, 0};	// ���ҡ��ƶ����ٶȷ���
	glm::f64vec2 downVelocity = {0, 1};		// ���¡��ƶ����ٶȷ���
	glm::f64vec2 leftVelocity = {-1, 0};	// ������ƶ����ٶȷ���

	bool isFire = false;					// �ո�����Ƿ񿪻�
	bool isMove = false;					// �Ƿ��ƶ�


	/**
	 * @brief ���캯��
	 * @param upVelocity	���ƶ��ٶȷ���	 
	 * @param rightVelocity ���ƶ��ٶȷ���
	 * @param downVelocity  ���ƶ��ٶȷ���
	 * @param leftVelocity  ���ƶ��ٶȷ���
	 * @param isFire		�ո�����Ƿ񿪻�
	 */
	KeyboardControlComponent() = default;
};

#endif // !KEYBOARDCONTROLCOMPONENT_H
