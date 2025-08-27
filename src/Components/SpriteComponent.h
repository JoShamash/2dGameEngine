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
    StaticGeometry,       // ��̬���Σ��������رꡢֲ�����ϰ��
    Decals,               // ���������ס�ˮ������ǵȣ�
    DynamicGeometry,      // ��̬���Σ����ƶ�ƽ̨���ŵȣ�

    // ���� ��ɫ������ ����  
    Characters,           // ��Ҫ��ɫ�� NPC
    Player,               // ���
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
        case RenderLayer::Player:
            return U8_TO_CHARPTR("���");
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

// ���ַ���������Ⱦ��ö��
inline RenderLayer RenderLayerFromString(const std::string& str)
{
    if (Registry::is_same_word(str, "BackgroundFar") || str == U8_TO_CHARPTR("��Զ����"))
    {
        return RenderLayer::BackgroundFar;
    }
    else if (Registry::is_same_word(str, "BackgroundNear") || str == U8_TO_CHARPTR("������"))
    {
        return RenderLayer::BackgroundNear;
    }
    else if (Registry::is_same_word(str, "Terrain") || str == U8_TO_CHARPTR("����"))
    {
        return RenderLayer::Terrain;
    }
    else if (Registry::is_same_word(str, "StaticGeometry") || str == U8_TO_CHARPTR("��̬����"))
    {
        return RenderLayer::StaticGeometry;
    }
    else if (Registry::is_same_word(str, "Decals") || str == U8_TO_CHARPTR("����"))
    {
        return RenderLayer::Decals;
    }
    else if (Registry::is_same_word(str, "DynamicGeometry") || str == U8_TO_CHARPTR("��̬����"))
    {
        return RenderLayer::DynamicGeometry;
    }
    else if (Registry::is_same_word(str, "Characters") || str == U8_TO_CHARPTR("��ɫ��NPC"))
    {
        return RenderLayer::Characters;
    }
    else if (Registry::is_same_word(str, "Player") || str == U8_TO_CHARPTR("���"))
    {
        return RenderLayer::Player;
    }
    else if (Registry::is_same_word(str, "Weapons") || str == U8_TO_CHARPTR("�����빤��"))
    {
        return RenderLayer::Weapons;
    }
    else if (Registry::is_same_word(str, "Projectiles") || str == U8_TO_CHARPTR("�ӵ���������"))
    {
        return RenderLayer::Projectiles;
    }
    else if (Registry::is_same_word(str, "EffectsUnder") || str == U8_TO_CHARPTR("��ɫ�·���Ч"))
    {
        return RenderLayer::EffectsUnder;
    }
    else if (Registry::is_same_word(str, "EffectsOver") || str == U8_TO_CHARPTR("��ɫ�Ϸ���Ч"))
    {
        return RenderLayer::EffectsOver;
    }
    else if (Registry::is_same_word(str, "Particles") || str == U8_TO_CHARPTR("ͨ������ϵͳ"))
    {
        return RenderLayer::Particles;
    }
    else if (Registry::is_same_word(str, "UIBackground") || str == U8_TO_CHARPTR("UI������"))
    {
        return RenderLayer::UIBackground;
    }
	else if (Registry::is_same_word(str, "UI") || str == U8_TO_CHARPTR("��ҪUIԪ��"))
    {
        return RenderLayer::UI;
    }
    else if (Registry::is_same_word(str, "UIForeground") || str == U8_TO_CHARPTR("UIǰ������"))
    {
        return RenderLayer::UIForeground;
    }
    else if (Registry::is_same_word(str, "Overlay") || str == U8_TO_CHARPTR("ȫ������"))
    {
        return RenderLayer::Overlay;
    }
    else if (Registry::is_same_word(str, "PostProcessing") || str == U8_TO_CHARPTR("���ڴ���"))
    {
        return RenderLayer::PostProcessing;
    }
    else if (Registry::is_same_word(str, "Debug") || str == U8_TO_CHARPTR("���Ե���"))
    {
        return RenderLayer::Debug;
    }
    else
    {
		std::string message = U8_TO_CHARPTR("δ֪����Ⱦ���ַ�����") + str + U8_TO_CHARPTR("��Ĭ������Ϊ��ɫ��NPC");
		Logger::Instance().Log(LogLevel::WARN, message);
		return RenderLayer::Characters; // Ĭ�Ϸ��ؽ�ɫ��NPC��
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
	glm::f64vec2 size;   // �����С��xΪ��ȣ�yΪ�߶�  
    RenderLayer layer;   // ��Ⱦ��
    SDL_Rect srcRect;	 // Դ��������������Ⱦʱ�Ĳü�
	bool isDirSheet;     // �Ƿ�Ϊ������ͼ�����������ĸ�����


    /**
	* @brief ���캯��
	* @param assetId ��ԴID��Ĭ��Ϊ���ַ���
	* @param size �����С��Ĭ��Ϊ(0, 0)
	* @param layer ��Ⱦ�㣬Ĭ��ΪRenderLayer::Characters
	* @param position Դ���ε����Ͻ�λ�ã�Ĭ��Ϊ(0, 0)
    */
    SpriteComponent(const std::string& assetId = "", glm::f64vec2 size = { 0, 0 }, RenderLayer layer = RenderLayer::Characters, bool isDirSheet = false, glm::f64vec2 position = { 0, 0 })
        : size(size), assetId(assetId), layer(layer), srcRect(position.x, position.y, size.x, size.y), isDirSheet(isDirSheet) { }
};  

#endif // !SPRITECOMPONENT_H