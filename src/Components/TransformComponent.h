#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/glm.hpp>

#include <string>

#include "../Logger/Logger.h"

/**
* @struct TransformComponent
* @brief ת��������������ꡢ��С���Ƕ�
*/
struct TransformComponent 
{
	inline const static std::string NAME = std::string("TransformComponent") + U8_TO_CHARPTR("ת�����"); // �������

	glm::f64vec2 position; // ����λ�ã�x, y��
	double rotation;	   // �Ƕ�
	glm::f64vec2 scale;    // ���ű�����x, y��
	

	TransformComponent(glm::f64vec2 position = glm::f64vec2(0, 0), double rotation = 0, glm::f64vec2 scale = glm::f64vec2(1, 1))
		: position(position), rotation(rotation), scale(scale) { }

};

#endif // !TRANSFORMCOMPONENT_H
