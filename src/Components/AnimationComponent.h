#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

/**
* @struct AnimationComponent
* @brief ������������ڴ�����״̬���������
*/
struct AnimationComponent 
{
	inline const static std::string NAME = std::string("AnimationComponent") + U8_TO_CHARPTR("�������"); // �������

	
	Uint64 numFrames;       // ��֡��
	Uint64 currentFrame;    // ��ǰ֡����
	Uint64 frameSpeed;		// ֡���ʣ�ÿ�����֡��
	bool isLooping;         // �Ƿ�ѭ������
	Uint64 startTime;       // ������ʼʱ��
	
	
	/** @brief ���캯��
	* @param numFrames ��֡��
	* @param frameSpeed ֡���ʣ�ÿ�����֡��
	* @param isLooping �Ƿ�ѭ������
	*/
	AnimationComponent(Uint64 numFrames = 0, Uint64 frameSpeed = 0, bool isLooping = true)
		: numFrames(numFrames), currentFrame(0), frameSpeed(frameSpeed), isLooping(isLooping), startTime(SDL_GetTicks64()) { }
};






#endif // !ANIMATIONCOMPONENT_H
