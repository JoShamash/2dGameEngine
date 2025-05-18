#ifndef SPRITECOMPONENT_H  
#define SPRITECOMPONENT_H  

#include <glm/glm.hpp>  
#include <SDL2/SDL.h>
#include <string>  

class SpriteComponent  
{  
public:  
	inline const static std::string NAME = std::string("SpriteComponent") + reinterpret_cast<const char*>(u8"�������"); // �������  

	std::string assetId; // ��ԴID 
	glm::f64vec2 size;   // �����С��xΪ��ȣ�yΪ�߶�  
	SDL_Rect srcRect;	 // Դ��������

   SpriteComponent(std::string assetId = "", glm::f64vec2 size = glm::f64vec2(0, 0), glm::f64vec2 position = glm::f64vec2(0, 0))
	   : size(size), assetId(assetId), srcRect(position.x, position.y, size.x, size.y) { }
};  

#endif // !SPRITECOMPONENT_H