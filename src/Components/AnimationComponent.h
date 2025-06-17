#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct AnimationComponent
* @brief 动画组件，用于处理动画状态和相关数据
*/
struct AnimationComponent 
{
	inline const static std::string NAME = std::string("AnimationComponent") + U8_TO_CHARPTR("动画组件"); // 组件名称

	
	Uint64 numFrames;       // 总帧数
	Uint64 currentFrame;    // 当前帧索引
	Uint64 frameSpeed;		// 帧速率（每秒多少帧）
	bool isLooping;         // 是否循环播放
	Uint64 startTime;       // 动画开始时间
	
	
	/** @brief 构造函数
	* @param numFrames 总帧数
	* @param frameSpeed 帧速率（每秒多少帧）
	* @param isLooping 是否循环播放
	*/
	AnimationComponent(Uint64 numFrames = 0, Uint64 frameSpeed = 0, bool isLooping = true)
		: numFrames(numFrames), currentFrame(0), frameSpeed(frameSpeed), isLooping(isLooping), startTime(SDL_GetTicks64()) { }
};






#endif // !ANIMATIONCOMPONENT_H
