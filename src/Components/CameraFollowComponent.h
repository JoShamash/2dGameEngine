#ifndef CAMERAFOLLOWCOMPONENT_H
#define CAMERAFOLLOWCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct CameraFollowComponent
* @brief ������������������������ӽǵĹ̶���
*/
struct CameraFollowComponent
{
	inline const static std::string NAME = std::string("CameraFollowComponent") + U8_TO_CHARPTR("������������"); // �������

	CameraFollowComponent() = default;
};

#endif // !CAMERAFOLLOWCOMPONENT_H
