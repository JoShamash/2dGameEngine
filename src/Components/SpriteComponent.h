#ifndef SPRITECOMPONENT_H  
#define SPRITECOMPONENT_H  

#include <glm/glm.hpp>  
#include <SDL2/SDL.h>
#include <string>  

#include "../Logger/Logger.h"

// ������Ⱦ��ö��
enum class RenderLayer : Uint8
{
    // ���� ������ ����  
    BackgroundFar = 0,    // ��Զ��������պС�Զ����
    BackgroundNear,       // ��������ɽ����Զ��������

    // ���� ����� ����  
    Terrain,              // ���Σ����桢ˮ��ȴ������̬�����
    StaticGeometry,       // ��̬���Σ��������ر꣩
    Decals,               // ���������ס�ˮ������ǵȣ�
    DynamicGeometry,      // ��̬���Σ����ƶ�ƽ̨���ŵȣ�

    // ���� ��ɫ������ ����  
    Characters,           // ��Ҫ��ɫ�� NPC
    Weapons,              // ����������
    Projectiles,          // �ӵ���������

    // ���� ��Ч ����  
    EffectsUnder,         // ��ɫ�·���Ч����Ӱ���䳾��
    EffectsOver,          // ��ɫ�Ϸ���Ч���⻷����Ч��
    Particles,            // ͨ������ϵͳ�����桢�����𻨣�

    // ���� UI ����  
    UIBackground,         // UI �����壨�Ի�������ͼ��
    UI,                   // ��Ҫ UI Ԫ�أ���ť���ı���
    UIForeground,         // UI ǰ�����ӣ����Ρ�������
    Overlay,              // ȫ�����ǣ�Ѫ�����˺���ʾ��

    // ���� ��������� ����  
    PostProcessing,       // ���ڴ������⡢ɫ��ӳ��ȣ�ȫ�� Quad��
    Debug,                // ���Ե��ӣ������߿���ײ�С�FPS ͳ�ƣ�

    Count                 // ����
};

// ���巽����ͼö�٣���ͬ����ѡ��ͬ��ͼ����ͼ��ʽΪ��������֡���������ϵ����Ƿ�����ͼ������������
enum class DirectionTexture : Uint8
{
    Up = 0,               // �����ƶ�����ͼ����
    Right,                // �����ƶ�����ͼ����
    Down,                 // �����ƶ�����ͼ����
    Left,                 // �����ƶ�����ͼ����

    Count                 // ��������
};

/**
* @struct SpriteComponent
* @brief ������������ڴ洢�������ԴID����С����Ⱦ���Դ��������
*/
struct SpriteComponent  
{  
	inline const static std::string NAME = std::string("SpriteComponent") + U8_TO_CHARPTR("�������"); // �������  


	std::string assetId; // ��ԴID 
	glm::u64vec2 size;   // �����С��xΪ��ȣ�yΪ�߶�  
    RenderLayer layer;   // ��Ⱦ��
    SDL_Rect srcRect;	 // Դ��������������Ⱦʱ�Ĳü�


    /**
	* @brief ���캯��
	* @param assetId ��ԴID��Ĭ��Ϊ���ַ���
	* @param size �����С��Ĭ��Ϊ(0, 0)
	* @param layer ��Ⱦ�㣬Ĭ��ΪRenderLayer::Characters
	* @param position Դ���ε����Ͻ�λ�ã�Ĭ��Ϊ(0, 0)
    */
	SpriteComponent(std::string assetId = "", glm::u64vec2 size = {0, 0}, RenderLayer layer = RenderLayer::Characters, glm::u64vec2 position = {0, 0})
		: size(size), assetId(assetId), layer(layer), srcRect(position.x, position.y, size.x, size.y) { }
};  

#endif // !SPRITECOMPONENT_H