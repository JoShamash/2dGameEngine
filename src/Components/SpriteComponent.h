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

// ������Ⱦ��ö�ٶ�Ӧ���ַ���ӳ�䡤����
inline const char* RenderLayerText(const RenderLayer& layer)
{
    switch (layer)
    {
        case RenderLayer::BackgroundFar: 
            return U8_TO_CHARPTR("��Զ����");
        case RenderLayer::BackgroundNear: 
			return U8_TO_CHARPTR("������");
        case RenderLayer::Terrain: 
            return U8_TO_CHARPTR("����");
        case RenderLayer::StaticGeometry: 
            return U8_TO_CHARPTR("��̬����");
        case RenderLayer::Decals: 
            return U8_TO_CHARPTR("����");
        case RenderLayer::DynamicGeometry: 
            return U8_TO_CHARPTR("��̬����");
        case RenderLayer::Characters: 
            return U8_TO_CHARPTR("��ɫ��NPC");
        case RenderLayer::Weapons: 
            return U8_TO_CHARPTR("�����빤��");
        case RenderLayer::Projectiles: 
            return U8_TO_CHARPTR("�ӵ���������");
        case RenderLayer::EffectsUnder: 
            return U8_TO_CHARPTR("��ɫ�·���Ч");
        case RenderLayer::EffectsOver: 
            return U8_TO_CHARPTR("��ɫ�Ϸ���Ч");
        case RenderLayer::Particles: 
            return U8_TO_CHARPTR("ͨ������ϵͳ");
        case RenderLayer::UIBackground: 
            return U8_TO_CHARPTR("UI������");
        case RenderLayer::UI: 
            return U8_TO_CHARPTR("��ҪUIԪ��");
        case RenderLayer::UIForeground: 
            return U8_TO_CHARPTR("UIǰ������");
        case RenderLayer::Overlay: 
            return U8_TO_CHARPTR("ȫ������");
        case RenderLayer::PostProcessing: 
            return U8_TO_CHARPTR("���ڴ���");
        case RenderLayer::Debug: 
            return U8_TO_CHARPTR("���Ե���");
        default:
			return U8_TO_CHARPTR("δ֪�㼶");
    }
}


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