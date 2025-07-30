#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct BoxColliderComponent
* @brief ��ײ��������ڴ���ʵ�����ײ����λ�ø��¡�
*/
struct BoxColliderComponent {
	inline const static std::string NAME = std::string("BoxColliderComponent") + U8_TO_CHARPTR("������ײ���"); // �������
	                  
	
	glm::f64vec2 size;			// ��ײ�д�С��xΪ��ȣ�yΪ�߶�  
	glm::f64vec2 offset;		// ƫ����
	bool isCollision = false;	// �Ƿ�����ײ


	/**
	 * @brief ���캯��
	 * @param size ��ײ�д�С��Ĭ��Ϊ(0, 0)
	 * @param offset ƫ������Ĭ��Ϊ(0, 0)
	 */
	BoxColliderComponent(const glm::f64vec2& size = {0, 0}, const glm::f64vec2& offset = {0, 0})
		: size(size), offset(offset) { }
};

#endif // !BOXCOLLIDERCOMPONENT_H
