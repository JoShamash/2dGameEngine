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
    StaticGeometry,       // 静态几何（建筑、地标）
    Decals,               // 贴花（弹孔、水迹、标记等）
    DynamicGeometry,      // 动态几何（可移动平台、门等）

    // —— 角色与物体 ——  
    Characters,           // 主要角色和 NPC
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


    /**
	* @brief 构造函数
	* @param assetId 资源ID，默认为空字符串
	* @param size 精灵大小，默认为(0, 0)
	* @param layer 渲染层，默认为RenderLayer::Characters
	* @param position 源矩形的左上角位置，默认为(0, 0)
    */
	SpriteComponent(std::string assetId = "", glm::f64vec2 size = {0, 0}, RenderLayer layer = RenderLayer::Characters, glm::f64vec2 position = {0, 0})
		: size(size), assetId(assetId), layer(layer), srcRect(position.x, position.y, size.x, size.y) { }
};  

#endif // !SPRITECOMPONENT_H