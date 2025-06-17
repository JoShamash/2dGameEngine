#include "AssetStore.h"

// 构造函数：初始化资源管理器，并进行日志记录
AssetStore::AssetStore()
{
    // 创建资源管理器时记录日志
    std::string message = U8_TO_CHARPTR("创建资源管理器");
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 析构函数：销毁资源管理器，清理所有资源并记录日志
AssetStore::~AssetStore()
{
    // 清理所有资源
    ClearAllAssets();

    // 销毁资源管理器时记录日志
    std::string message = U8_TO_CHARPTR("资源管理器已销毁");
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 清理特定图片资源
void AssetStore::ClearImageAsset(const std::string& assetId)
{
    auto imageIt = imageAssets.find(assetId); // 查找指定的资源 ID
    if (imageIt != imageAssets.end()) {
        // 释放资源
        SDL_DestroyTexture(imageIt->second);
        imageAssets.erase(imageIt);  // 从容器中移除该资源

		std::string message = U8_TO_CHARPTR("图片资源 ") + assetId + U8_TO_CHARPTR(" 已清理");
        Logger::Instance().Log(LogLevel::INFO, message);
	}
    else
    {
        std::string message = U8_TO_CHARPTR("图片资源 ") + assetId + U8_TO_CHARPTR(" 不存在");
        Logger::Instance().Log(LogLevel::WARN, message);
    }
}

// 清理所有图片资源
void AssetStore::ClearImageAssets()
{
    for (auto& [assetId, asset] : imageAssets) {
        SDL_DestroyTexture(asset);  // 释放每个图片资源
    }
    imageAssets.clear(); // 清空容器

	std::string message = U8_TO_CHARPTR("已清理所有图片资源");
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 清理特定音频资源
void AssetStore::ClearAudioAsset(const std::string& assetId)
{
    auto audioIt = audioAssets.find(assetId); // 查找指定的资源 ID
    if (audioIt != audioAssets.end()) {
        // 释放音频资源
        Mix_FreeMusic(audioIt->second);
        audioAssets.erase(audioIt);  // 从容器中移除该资源

		std::string message = U8_TO_CHARPTR("音频资源 ") + assetId + U8_TO_CHARPTR(" 已清理");
        Logger::Instance().Log(LogLevel::INFO, message);
    }
    else
    {
		std::string message = U8_TO_CHARPTR("音频资源 ") + assetId + U8_TO_CHARPTR(" 不存在");
		Logger::Instance().Log(LogLevel::WARN, message);
    }
}

// 清理所有音频资源
void AssetStore::ClearAudioAssets()
{
    for (auto& [assetId, asset] : audioAssets) {
        Mix_FreeMusic(asset);  // 释放每个音频资源
    }
    audioAssets.clear(); // 清空容器

	std::string message = U8_TO_CHARPTR("已清理所有音频资源");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// 清理特定音效资源
void AssetStore::ClearSoundEffectAsset(const std::string& assetId)
{
    auto soundEffectIt = soundEffectAssets.find(assetId); // 查找指定的资源 ID
    if (soundEffectIt != soundEffectAssets.end()) {
        // 释放音效资源
        Mix_FreeChunk(soundEffectIt->second);
        soundEffectAssets.erase(soundEffectIt);  // 从容器中移除该资源
        
		std::string message = U8_TO_CHARPTR("音效资源 ") + assetId + U8_TO_CHARPTR(" 已清理");
		Logger::Instance().Log(LogLevel::INFO, message);
	}
	else
	{
		std::string message = U8_TO_CHARPTR("音效资源 ") + assetId + U8_TO_CHARPTR(" 不存在");
		Logger::Instance().Log(LogLevel::WARN, message);
    }
}

// 清理所有音效资源
void AssetStore::ClearSoundEffectAssets()
{
    for (auto& [assetId, asset] : soundEffectAssets) {
        Mix_FreeChunk(asset);  // 释放每个音效资源
    }
    soundEffectAssets.clear(); // 清空容器

	std::string message = U8_TO_CHARPTR("已清理所有音效资源");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// 清理特定字体资源
void AssetStore::ClearFontAsset(const std::string& assetId)
{
    auto fontIt = fontAssets.find(assetId); // 查找指定的资源 ID
    if (fontIt != fontAssets.end()) {
        // 释放字体资源
        TTF_CloseFont(fontIt->second);
        fontAssets.erase(fontIt);  // 从容器中移除该资源

		std::string message = U8_TO_CHARPTR("字体资源 ") + assetId + U8_TO_CHARPTR(" 已清理");
		Logger::Instance().Log(LogLevel::INFO, message);
    }
    else
    {
		std::string message = U8_TO_CHARPTR("字体资源 ") + assetId + U8_TO_CHARPTR(" 不存在");
		Logger::Instance().Log(LogLevel::WARN, message);
    }
}

// 清理所有字体资源
void AssetStore::ClearFontAssets()
{
    for (auto& [assetId, asset] : fontAssets) {
        TTF_CloseFont(asset);  // 释放每个字体资源
    }
    fontAssets.clear(); // 清空容器

	std::string message = U8_TO_CHARPTR("已清理所有字体资源");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// 清理特定地图样式资源
void AssetStore::ClearMapStyleAsset(const std::string& assetId)
{
	auto mapStyleIt = mapStyleAssets.find(assetId); // 查找指定的资源 ID
	if (mapStyleIt != mapStyleAssets.end()) {
		// 释放地图样式资源
		delete mapStyleIt->second;
		mapStyleAssets.erase(mapStyleIt);  // 从容器中移除该资源
		std::string message = U8_TO_CHARPTR("地图样式资源 ") + assetId + U8_TO_CHARPTR(" 已清理");
		Logger::Instance().Log(LogLevel::INFO, message);
	}
	else
	{
		std::string message = U8_TO_CHARPTR("地图样式资源 ") + assetId + U8_TO_CHARPTR(" 不存在");
		Logger::Instance().Log(LogLevel::WARN, message);
	}
}

// 清理所有地图样式资源
void AssetStore::ClearMapStyleAssets()
{
	for (auto& [assetId, asset] : mapStyleAssets) {
		delete asset;  // 释放每个地图样式资源
	}
	mapStyleAssets.clear(); // 清空容器

	std::string message = U8_TO_CHARPTR("已清理所有地图样式资源");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// 清理所有资源
void AssetStore::ClearAllAssets()
{
    ClearImageAssets();       // 清理所有图片资源
    ClearAudioAssets();       // 清理所有音频资源
    ClearSoundEffectAssets(); // 清理所有音效资源
    ClearFontAssets();        // 清理所有字体资源
	ClearMapStyleAssets();    // 清理所有地图样式资源

	std::string message = U8_TO_CHARPTR("已清理所有资源");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// 添加图片资源：根据文件路径加载图片并存储到 imageAssets 容器中
void AssetStore::AddImageAsset(SDL_Renderer* renderer, const std::string& assetId, const std::string& path)
{
    // 检查资源是否已存在
    if (imageAssets.find(assetId) != imageAssets.end()) {
		std::string message = U8_TO_CHARPTR("图片资源ID已存在：") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;
    }

    // 加载图片文件为 surface
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
		std::string message = U8_TO_CHARPTR("加载图片失败：") + path + " " + IMG_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 创建纹理对象
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // 不再需要 surface，立即释放
    if (!texture) {
		std::string message = U8_TO_CHARPTR("创建纹理失败：") + path + " " + SDL_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 将纹理添加到资源容器中
	imageAssets.emplace(assetId, texture);

	std::string message = U8_TO_CHARPTR("加载图片资源：资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 添加音频资源：根据文件路径加载音频并存储到 audioAssets 容器中
void AssetStore::AddAudioAsset(const std::string& assetId, const std::string& path)
{
    // 检查资源是否已存在
    if (audioAssets.find(assetId) != audioAssets.end()) {
		std::string message = U8_TO_CHARPTR("音频资源ID已存在：") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;
    }

    // 加载音频资源
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
		std::string message = U8_TO_CHARPTR("加载音频失败：") + path + " " + Mix_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 将音频资源添加到容器中
	audioAssets.emplace(assetId, music);

	std::string message = U8_TO_CHARPTR("加载音频资源：资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 添加音效资源：根据文件路径加载音效并存储到 soundEffectAssets 容器中
void AssetStore::AddSoundEffectAsset(const std::string& assetId, const std::string& path)
{
    // 检查资源是否已存在
    if (soundEffectAssets.find(assetId) != soundEffectAssets.end()) {
		std::string message = U8_TO_CHARPTR("音效资源ID已存在：") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;
    }

    // 加载音效资源
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
		std::string message = U8_TO_CHARPTR("加载音效失败：") + path + " " + Mix_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 将音效资源添加到容器中
	soundEffectAssets.emplace(assetId, chunk);

	std::string message = U8_TO_CHARPTR("加载音效资源：资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 添加字体资源：根据文件路径加载字体并存储到 fontAssets 容器中
void AssetStore::AddFontAsset(const std::string& assetId, const std::string& path, size_t psize)
{
    // 检查资源是否已存在
    if (fontAssets.find(assetId) != fontAssets.end()) {
		std::string message = U8_TO_CHARPTR("字体资源ID已存在：") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;
    }

    // 加载字体资源
    TTF_Font* font = TTF_OpenFont(path.c_str(), psize);
    if (!font) {
		std::string message = U8_TO_CHARPTR("加载字体失败：") + path + " " + TTF_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 将字体资源添加到容器中
	fontAssets.emplace(assetId, font);

	std::string message = U8_TO_CHARPTR("加载字体资源：资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 添加地图样式资源：根据文件路径加载地图样式并存储到 mapStyleAssets 容器中
void AssetStore::AddMapStyleAsset(const std::string& assetId, const std::string& path)
{
	// 检查资源是否已存在
	if (mapStyleAssets.find(assetId) != mapStyleAssets.end()) {
		std::string message = U8_TO_CHARPTR("地图样式资源ID已存在：") + assetId;
		Logger::Instance().Log(LogLevel::WARN, message);
		return;
	}

	MapStyle* mapStyle = new MapStyle(path); // 创建地图样式对象

	// 将地图样式资源添加到容器中
	mapStyleAssets.emplace(assetId, mapStyle);

	std::string message = U8_TO_CHARPTR("加载地图样式资源：资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
	Logger::Instance().Log(LogLevel::INFO, message);
}

// 修改图片资源：根据路径加载图片并存储（如果资源ID已存在则更新，不存在则退出）
void AssetStore::ChangeImageAsset(SDL_Renderer* renderer, const std::string& assetId, const std::string& path)
{
    // 检查资源ID是否存在，不存在则退出
    if (imageAssets.find(assetId) == imageAssets.end()) {
		std::string message = U8_TO_CHARPTR("图片资源ID不存在，退出操作：") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;  // 资源不存在时，直接退出
    }

    // 销毁旧的图片资源
    SDL_DestroyTexture(imageAssets[assetId]);

    // 加载新的图片资源
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
		std::string message = U8_TO_CHARPTR("加载图片失败：") + path + " " + SDL_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 创建新的纹理
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // 释放 surface 内存
    if (!texture) {
		std::string message = U8_TO_CHARPTR("创建纹理失败：") + path + " " + SDL_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 存储新的纹理到资源容器
    imageAssets[assetId] = texture;

	std::string message = U8_TO_CHARPTR("替换图片资源 资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 修改音频资源：根据路径加载音频并存储（如果资源ID已存在则更新，不存在则退出）
void AssetStore::ChangeAudioAsset(const std::string& assetId, const std::string& path)
{
    // 检查资源ID是否存在，不存在则退出
    if (audioAssets.find(assetId) == audioAssets.end()) {
		std::string message = U8_TO_CHARPTR("音频资源ID不存在，退出操作：") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;  // 资源不存在时，直接退出
    }

    // 销毁旧的音频资源
    Mix_FreeMusic(audioAssets[assetId]);

    // 加载新的音频资源
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
		std::string message = U8_TO_CHARPTR("加载音频失败：") + path + " " + Mix_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 存储新的音频资源
    audioAssets[assetId] = music;

	std::string message = U8_TO_CHARPTR("替换音频资源 资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 修改音效资源：根据路径加载音效并存储（如果资源ID已存在则更新，不存在则退出）
void AssetStore::ChangeSoundEffectAsset(const std::string& assetId, const std::string& path)
{
    // 检查资源ID是否存在，不存在则退出
    if (soundEffectAssets.find(assetId) == soundEffectAssets.end()) {
		std::string message = U8_TO_CHARPTR("音效资源ID不存在，退出操作：") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;  // 资源不存在时，直接退出
    }

    // 销毁旧的音效资源
    Mix_FreeChunk(soundEffectAssets[assetId]);

    // 加载新的音效资源
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
		std::string message = U8_TO_CHARPTR("加载音效失败：") + path + " " + Mix_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 存储新的音效资源
    soundEffectAssets[assetId] = chunk;

	std::string message = U8_TO_CHARPTR("替换音效资源 资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 修改字体资源：根据路径加载字体并存储（如果资源ID已存在则更新，不存在则退出）
void AssetStore::ChangeFontAsset(const std::string& assetId, const std::string& path, size_t psize)
{
    // 检查资源ID是否存在，不存在则退出
    if (fontAssets.find(assetId) == fontAssets.end()) {
		std::string message = U8_TO_CHARPTR("字体资源ID不存在，退出操作：") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;  // 资源不存在时，直接退出
    }

    // 销毁旧的字体资源
    TTF_CloseFont(fontAssets[assetId]);

    // 加载新的字体资源
    TTF_Font* font = TTF_OpenFont(path.c_str(), psize);
    if (!font) {
		std::string message = U8_TO_CHARPTR("加载字体失败：") + path + " " + TTF_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // 存储新的字体资源
    fontAssets[assetId] = font;

	std::string message = U8_TO_CHARPTR("替换字体资源 资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// 修改地图样式资源：根据路径加载地图样式并存储（如果资源ID已存在则更新，不存在则退出）
void AssetStore::ChangeMapStyleAsset(const std::string& assetId, const std::string& path)
{
	// 检查资源ID是否存在，不存在则退出
	if (mapStyleAssets.find(assetId) == mapStyleAssets.end()) {
		std::string message = U8_TO_CHARPTR("地图样式资源ID不存在，退出操作：") + assetId;
		Logger::Instance().Log(LogLevel::WARN, message);
		return;  // 资源不存在时，直接退出
	}

	// 销毁旧的地图样式资源
	delete mapStyleAssets[assetId];

	// 创建新的地图样式对象
	MapStyle* mapStyle = new MapStyle(path);

	// 存储新的地图样式资源
	mapStyleAssets[assetId] = mapStyle;
	std::string message = U8_TO_CHARPTR("替换地图样式资源 资产编号：") + assetId + U8_TO_CHARPTR(" 资产路径：") + path;
	Logger::Instance().Log(LogLevel::INFO, message);
}

// 获取图片资源：根据资源 ID 返回对应的纹理
SDL_Texture* AssetStore::GetImageAsset(const std::string& assetId)
{
	if (imageAssets.find(assetId) != imageAssets.end()) 
    {
		return imageAssets[assetId]; // 返回对应的纹理
	}
    else 
    {
		return nullptr; // 资源不存在，返回空指针
    }
}

// 获取音频资源：根据资源 ID 返回对应的音乐流
Mix_Music* AssetStore::GetAudioAsset(const std::string& assetId)
{
	if (audioAssets.find(assetId) != audioAssets.end())
	{
		return audioAssets[assetId]; // 返回对应的音频
	}
    else
    {
        return nullptr; // 资源不存在，返回空指针
    }
}

// 获取音效资源：根据资源 ID 返回对应的音效
Mix_Chunk* AssetStore::GetSoundEffectAsset(const std::string& assetId)
{
	if (soundEffectAssets.find(assetId) != soundEffectAssets.end())
	{
		return soundEffectAssets[assetId]; // 返回对应的音效
	}
	else
	{
		return nullptr; // 资源不存在，返回空指针
	}
}

// 获取字体资源：根据资源 ID 返回对应的字体
TTF_Font* AssetStore::GetFontAsset(const std::string& assetId)
{
	if (fontAssets.find(assetId) != fontAssets.end())
	{
		return fontAssets[assetId]; // 返回对应的字体
	}
	else
	{
		return nullptr; // 资源不存在，返回空指针
	}
}

// 获取地图样式资源：根据资源 ID 返回对应的地图样式
MapStyle* AssetStore::GetMapStyleAsset(const std::string& assetId)
{
	if (mapStyleAssets.find(assetId) != mapStyleAssets.end())
	{
		return mapStyleAssets[assetId]; // 返回对应的地图样式
	}
	else
	{
		return nullptr; // 资源不存在，返回空指针
	}
}
