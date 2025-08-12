#ifndef SPRITECOMPONENT_H  
#define SPRITECOMPONENT_H  

#include <glm/glm.hpp>  
#include <SDL2/SDL.h>
#include <string>  

#include "../Logger/Logger.h"

// 定义渲染层枚举
enum class RenderLayer : Uint8
{
    // —— 背景层 ——  
    BackgroundFar = 0,    // 最远背景（天空盒、远景）
    BackgroundNear,       // 近背景（山脉、远处建筑）

    // —— 世界层 ——  
    Terrain,              // 地形（地面、水面等大面积静态物件）
    StaticGeometry,       // 静态几何（建筑、地标、植被、障碍物）
    Decals,               // 贴花（弹孔、水迹、标记等）
    DynamicGeometry,      // 动态几何（可移动平台、门等）

    // —— 角色与物体 ——  
    Characters,           // 主要角色和 NPC
    Player,               // 玩家
    Weapons,              // 武器、工具
    Projectiles,          // 子弹、抛射物

    // —— 特效 ——  
    EffectsUnder,         // 角色下方特效（阴影、落尘）
    EffectsOver,          // 角色上方特效（光环、光效）
    Particles,            // 通用粒子系统（火焰、烟雾、火花）

    // —— UI ——  
    UIBackground,         // UI 背景板（对话框、面板底图）
    UI,                   // 主要 UI 元素（按钮、文本）
    UIForeground,         // UI 前景叠加（光晕、高亮框）
    Overlay,              // 全屏覆盖（血屏、伤害提示）

    // —— 后期与调试 ——  
    PostProcessing,       // 后期处理（泛光、色调映射等，全屏 Quad）
    Debug,                // 调试叠加（调试线框、碰撞盒、FPS 统计）

    Count                 // 层数
};

// 定义渲染层枚举对应的字符串映射·中文
inline const char* RenderLayerText(const RenderLayer& layer)
{
    switch (layer)
    {
        case RenderLayer::BackgroundFar: 
            return U8_TO_CHARPTR("最远背景");
        case RenderLayer::BackgroundNear: 
			return U8_TO_CHARPTR("近背景");
        case RenderLayer::Terrain: 
            return U8_TO_CHARPTR("地形");
        case RenderLayer::StaticGeometry: 
            return U8_TO_CHARPTR("静态几何");
        case RenderLayer::Decals: 
            return U8_TO_CHARPTR("贴花");
        case RenderLayer::DynamicGeometry: 
            return U8_TO_CHARPTR("动态几何");
        case RenderLayer::Characters: 
            return U8_TO_CHARPTR("角色与NPC");
        case RenderLayer::Player:
            return U8_TO_CHARPTR("玩家");
        case RenderLayer::Weapons: 
            return U8_TO_CHARPTR("武器与工具");
        case RenderLayer::Projectiles: 
            return U8_TO_CHARPTR("子弹与抛射物");
        case RenderLayer::EffectsUnder: 
            return U8_TO_CHARPTR("角色下方特效");
        case RenderLayer::EffectsOver: 
            return U8_TO_CHARPTR("角色上方特效");
        case RenderLayer::Particles: 
            return U8_TO_CHARPTR("通用粒子系统");
        case RenderLayer::UIBackground: 
            return U8_TO_CHARPTR("UI背景板");
        case RenderLayer::UI: 
            return U8_TO_CHARPTR("主要UI元素");
        case RenderLayer::UIForeground: 
            return U8_TO_CHARPTR("UI前景叠加");
        case RenderLayer::Overlay: 
            return U8_TO_CHARPTR("全屏覆盖");
        case RenderLayer::PostProcessing: 
            return U8_TO_CHARPTR("后期处理");
        case RenderLayer::Debug: 
            return U8_TO_CHARPTR("调试叠加");
        default:
			return U8_TO_CHARPTR("未知层级");
    }
}

// 从字符串解析渲染层枚举
inline RenderLayer RenderLayerFromString(const std::string& str)
{
    if (Registry::is_same_word(str, "BackgroundFar") || str == U8_TO_CHARPTR("最远背景"))
    {
        return RenderLayer::BackgroundFar;
    }
    else if (Registry::is_same_word(str, "BackgroundNear") || str == U8_TO_CHARPTR("近背景"))
    {
        return RenderLayer::BackgroundNear;
    }
    else if (Registry::is_same_word(str, "Terrain") || str == U8_TO_CHARPTR("地形"))
    {
        return RenderLayer::Terrain;
    }
    else if (Registry::is_same_word(str, "StaticGeometry") || str == U8_TO_CHARPTR("静态几何"))
    {
        return RenderLayer::StaticGeometry;
    }
    else if (Registry::is_same_word(str, "Decals") || str == U8_TO_CHARPTR("贴花"))
    {
        return RenderLayer::Decals;
    }
    else if (Registry::is_same_word(str, "DynamicGeometry") || str == U8_TO_CHARPTR("动态几何"))
    {
        return RenderLayer::DynamicGeometry;
    }
    else if (Registry::is_same_word(str, "Characters") || str == U8_TO_CHARPTR("角色与NPC"))
    {
        return RenderLayer::Characters;
    }
    else if (Registry::is_same_word(str, "Player") || str == U8_TO_CHARPTR("玩家"))
    {
        return RenderLayer::Player;
    }
    else if (Registry::is_same_word(str, "Weapons") || str == U8_TO_CHARPTR("武器与工具"))
    {
        return RenderLayer::Weapons;
    }
    else if (Registry::is_same_word(str, "Projectiles") || str == U8_TO_CHARPTR("子弹与抛射物"))
    {
        return RenderLayer::Projectiles;
    }
    else if (Registry::is_same_word(str, "EffectsUnder") || str == U8_TO_CHARPTR("角色下方特效"))
    {
        return RenderLayer::EffectsUnder;
    }
    else if (Registry::is_same_word(str, "EffectsOver") || str == U8_TO_CHARPTR("角色上方特效"))
    {
        return RenderLayer::EffectsOver;
    }
    else if (Registry::is_same_word(str, "Particles") || str == U8_TO_CHARPTR("通用粒子系统"))
    {
        return RenderLayer::Particles;
    }
    else if (Registry::is_same_word(str, "UIBackground") || str == U8_TO_CHARPTR("UI背景板"))
    {
        return RenderLayer::UIBackground;
    }
	else if (Registry::is_same_word(str, "UI") || str == U8_TO_CHARPTR("主要UI元素"))
    {
        return RenderLayer::UI;
    }
    else if (Registry::is_same_word(str, "UIForeground") || str == U8_TO_CHARPTR("UI前景叠加"))
    {
        return RenderLayer::UIForeground;
    }
    else if (Registry::is_same_word(str, "Overlay") || str == U8_TO_CHARPTR("全屏覆盖"))
    {
        return RenderLayer::Overlay;
    }
    else if (Registry::is_same_word(str, "PostProcessing") || str == U8_TO_CHARPTR("后期处理"))
    {
        return RenderLayer::PostProcessing;
    }
    else if (Registry::is_same_word(str, "Debug") || str == U8_TO_CHARPTR("调试叠加"))
    {
        return RenderLayer::Debug;
    }
    else
    {
		std::string message = U8_TO_CHARPTR("未知的渲染层字符串：") + str + U8_TO_CHARPTR("，默认设置为角色与NPC");
		Logger::Instance().Log(LogLevel::WARN, message);
		return RenderLayer::Characters; // 默认返回角色与NPC层
    }
}

// 定义方向贴图枚举，不同方向选择不同贴图，贴图格式为从左到右是帧动画，从上到下是方向贴图（↑→↓←）
enum class DirectionTexture : Uint8
{
    Up = 0,               // 向上移动的贴图索引
    Right,                // 向右移动的贴图索引
    Down,                 // 向下移动的贴图索引
    Left,                 // 向左移动的贴图索引

    Count                 // 索引个数
};

/**
* @struct SpriteComponent
* @brief 精灵组件，用于存储精灵的资源ID、大小、渲染层和源矩形区域
*/
struct SpriteComponent  
{  
	inline const static std::string NAME = std::string("SpriteComponent") + U8_TO_CHARPTR("精灵组件"); // 组件名称  


	std::string assetId; // 资源ID 
	glm::f64vec2 size;   // 精灵大小，x为宽度，y为高度  
    RenderLayer layer;   // 渲染层
    SDL_Rect srcRect;	 // 源矩形区域，用于渲染时的裁剪
	bool isDirSheet;     // 是否为方向贴图（上下左右四个方向）


    /**
	* @brief 构造函数
	* @param assetId 资源ID，默认为空字符串
	* @param size 精灵大小，默认为(0, 0)
	* @param layer 渲染层，默认为RenderLayer::Characters
	* @param position 源矩形的左上角位置，默认为(0, 0)
    */
    SpriteComponent(const std::string& assetId = "", glm::f64vec2 size = { 0, 0 }, RenderLayer layer = RenderLayer::Characters, bool isDirSheet = false, glm::f64vec2 position = { 0, 0 })
        : size(size), assetId(assetId), layer(layer), srcRect(position.x, position.y, size.x, size.y), isDirSheet(isDirSheet) { }
};  

#endif // !SPRITECOMPONENT_H