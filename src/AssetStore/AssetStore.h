#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>

#include <string>
#include <memory>
#include <unordered_map>

#include "../Logger/Logger.h"
#include "MapStyle.h"

class AssetStore
{
public:
    // 构造函数：初始化资源管理器，准备资源加载
    AssetStore();

    // 析构函数：清理资源并销毁资源管理器
    ~AssetStore();

    // 清理特定的图片资源
    void ClearImageAsset(const std::string& assetId);

    // 清理所有图片资源
    void ClearImageAssets();

    // 清理特定的音频资源
    void ClearAudioAsset(const std::string& assetId);

    // 清理所有音频资源
    void ClearAudioAssets();

    // 清理特定的音效资源
    void ClearSoundEffectAsset(const std::string& assetId);

    // 清理所有音效资源
    void ClearSoundEffectAssets();

    // 清理特定的字体资源
    void ClearFontAsset(const std::string& assetId);

    // 清理所有字体资源
    void ClearFontAssets();

	// 清理特定的地图样式资源
	void ClearMapStyleAsset(const std::string& assetId);

	// 清理所有地图样式资源
	void ClearMapStyleAssets();

    // 清理所有资源
    void ClearAllAssets();

    // 添加图片资源：根据路径加载图片并存储
    void AddImageAsset(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);

    // 添加音频资源：根据路径加载音频并存储
    void AddAudioAsset(const std::string& assetId, const std::string& path);

    // 添加音效资源：根据路径加载音效并存储
    void AddSoundEffectAsset(const std::string& assetId, const std::string& path);

    // 添加字体资源：根据路径加载字体并存储
    void AddFontAsset(const std::string& assetId, const std::string& path, size_t psize);

	// 添加地图样式资源：根据路径加载地图样式并存储
	void AddMapStyleAsset(const std::string& assetId, const std::string& path);

    // 修改图片资源：根据路径加载图片并替换
    void ChangeImageAsset(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);

    // 修改音频资源：根据路径加载音频并替换
    void ChangeAudioAsset(const std::string& assetId, const std::string& path);

    // 修改音效资源：根据路径加载音效并替换
    void ChangeSoundEffectAsset(const std::string& assetId, const std::string& path);

    // 修改字体资源：根据路径加载字体并替换
    void ChangeFontAsset(const std::string& assetId, const std::string& path, size_t psize = 24);

	// 修改地图样式资源：根据路径加载地图样式并替换
	void ChangeMapStyleAsset(const std::string& assetId, const std::string& path);

    // 获取图片资源：返回图片纹理
    SDL_Texture* GetImageAsset(const std::string& assetId);

    // 获取音频资源：返回音乐流
    Mix_Music* GetAudioAsset(const std::string& assetId);

    // 获取音效资源：返回音效
    Mix_Chunk* GetSoundEffectAsset(const std::string& assetId);

    // 获取字体资源：返回字体
    TTF_Font* GetFontAsset(const std::string& assetId);

	// 获取地图样式资源：返回地图样式
	MapStyle* GetMapStyleAsset(const std::string& assetId);

private:
    // 存储图片资源的容器，使用资源 ID 进行索引
    std::unordered_map<std::string, SDL_Texture*> imageAssets;

    // 存储音频资源的容器，使用资源 ID 进行索引
    std::unordered_map<std::string, Mix_Music*> audioAssets;

    // 存储音效资源的容器，使用资源 ID 进行索引
    std::unordered_map<std::string, Mix_Chunk*> soundEffectAssets;

    // 存储字体资源的容器，使用资源 ID 进行索引
    std::unordered_map<std::string, TTF_Font*> fontAssets;

	// 存储地图样式的容器，使用资源 ID 进行索引
	std::unordered_map<std::string, MapStyle*> mapStyleAssets;
};

#endif // ASSETSTORE_H
