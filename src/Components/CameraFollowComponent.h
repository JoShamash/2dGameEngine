#ifndef CAMERAFOLLOWCOMPONENT_H
#define CAMERAFOLLOWCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct CameraFollowComponent
* @brief 摄像机跟随组件，用于摄像机视角的固定。
*/
struct CameraFollowComponent
{
	inline const static std::string NAME = std::string("CameraFollowComponent") + U8_TO_CHARPTR("摄像机跟随组件"); // 组件名称

	CameraFollowComponent() = default;
};

#endif // !CAMERAFOLLOWCOMPONENT_H
