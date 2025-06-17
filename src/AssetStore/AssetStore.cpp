#include "AssetStore.h"

// ���캯������ʼ����Դ����������������־��¼
AssetStore::AssetStore()
{
    // ������Դ������ʱ��¼��־
    std::string message = U8_TO_CHARPTR("������Դ������");
    Logger::Instance().Log(LogLevel::INFO, message);
}

// ����������������Դ������������������Դ����¼��־
AssetStore::~AssetStore()
{
    // ����������Դ
    ClearAllAssets();

    // ������Դ������ʱ��¼��־
    std::string message = U8_TO_CHARPTR("��Դ������������");
    Logger::Instance().Log(LogLevel::INFO, message);
}

// �����ض�ͼƬ��Դ
void AssetStore::ClearImageAsset(const std::string& assetId)
{
    auto imageIt = imageAssets.find(assetId); // ����ָ������Դ ID
    if (imageIt != imageAssets.end()) {
        // �ͷ���Դ
        SDL_DestroyTexture(imageIt->second);
        imageAssets.erase(imageIt);  // ���������Ƴ�����Դ

		std::string message = U8_TO_CHARPTR("ͼƬ��Դ ") + assetId + U8_TO_CHARPTR(" ������");
        Logger::Instance().Log(LogLevel::INFO, message);
	}
    else
    {
        std::string message = U8_TO_CHARPTR("ͼƬ��Դ ") + assetId + U8_TO_CHARPTR(" ������");
        Logger::Instance().Log(LogLevel::WARN, message);
    }
}

// ��������ͼƬ��Դ
void AssetStore::ClearImageAssets()
{
    for (auto& [assetId, asset] : imageAssets) {
        SDL_DestroyTexture(asset);  // �ͷ�ÿ��ͼƬ��Դ
    }
    imageAssets.clear(); // �������

	std::string message = U8_TO_CHARPTR("����������ͼƬ��Դ");
    Logger::Instance().Log(LogLevel::INFO, message);
}

// �����ض���Ƶ��Դ
void AssetStore::ClearAudioAsset(const std::string& assetId)
{
    auto audioIt = audioAssets.find(assetId); // ����ָ������Դ ID
    if (audioIt != audioAssets.end()) {
        // �ͷ���Ƶ��Դ
        Mix_FreeMusic(audioIt->second);
        audioAssets.erase(audioIt);  // ���������Ƴ�����Դ

		std::string message = U8_TO_CHARPTR("��Ƶ��Դ ") + assetId + U8_TO_CHARPTR(" ������");
        Logger::Instance().Log(LogLevel::INFO, message);
    }
    else
    {
		std::string message = U8_TO_CHARPTR("��Ƶ��Դ ") + assetId + U8_TO_CHARPTR(" ������");
		Logger::Instance().Log(LogLevel::WARN, message);
    }
}

// ����������Ƶ��Դ
void AssetStore::ClearAudioAssets()
{
    for (auto& [assetId, asset] : audioAssets) {
        Mix_FreeMusic(asset);  // �ͷ�ÿ����Ƶ��Դ
    }
    audioAssets.clear(); // �������

	std::string message = U8_TO_CHARPTR("������������Ƶ��Դ");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// �����ض���Ч��Դ
void AssetStore::ClearSoundEffectAsset(const std::string& assetId)
{
    auto soundEffectIt = soundEffectAssets.find(assetId); // ����ָ������Դ ID
    if (soundEffectIt != soundEffectAssets.end()) {
        // �ͷ���Ч��Դ
        Mix_FreeChunk(soundEffectIt->second);
        soundEffectAssets.erase(soundEffectIt);  // ���������Ƴ�����Դ
        
		std::string message = U8_TO_CHARPTR("��Ч��Դ ") + assetId + U8_TO_CHARPTR(" ������");
		Logger::Instance().Log(LogLevel::INFO, message);
	}
	else
	{
		std::string message = U8_TO_CHARPTR("��Ч��Դ ") + assetId + U8_TO_CHARPTR(" ������");
		Logger::Instance().Log(LogLevel::WARN, message);
    }
}

// ����������Ч��Դ
void AssetStore::ClearSoundEffectAssets()
{
    for (auto& [assetId, asset] : soundEffectAssets) {
        Mix_FreeChunk(asset);  // �ͷ�ÿ����Ч��Դ
    }
    soundEffectAssets.clear(); // �������

	std::string message = U8_TO_CHARPTR("������������Ч��Դ");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// �����ض�������Դ
void AssetStore::ClearFontAsset(const std::string& assetId)
{
    auto fontIt = fontAssets.find(assetId); // ����ָ������Դ ID
    if (fontIt != fontAssets.end()) {
        // �ͷ�������Դ
        TTF_CloseFont(fontIt->second);
        fontAssets.erase(fontIt);  // ���������Ƴ�����Դ

		std::string message = U8_TO_CHARPTR("������Դ ") + assetId + U8_TO_CHARPTR(" ������");
		Logger::Instance().Log(LogLevel::INFO, message);
    }
    else
    {
		std::string message = U8_TO_CHARPTR("������Դ ") + assetId + U8_TO_CHARPTR(" ������");
		Logger::Instance().Log(LogLevel::WARN, message);
    }
}

// ��������������Դ
void AssetStore::ClearFontAssets()
{
    for (auto& [assetId, asset] : fontAssets) {
        TTF_CloseFont(asset);  // �ͷ�ÿ��������Դ
    }
    fontAssets.clear(); // �������

	std::string message = U8_TO_CHARPTR("����������������Դ");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// �����ض���ͼ��ʽ��Դ
void AssetStore::ClearMapStyleAsset(const std::string& assetId)
{
	auto mapStyleIt = mapStyleAssets.find(assetId); // ����ָ������Դ ID
	if (mapStyleIt != mapStyleAssets.end()) {
		// �ͷŵ�ͼ��ʽ��Դ
		delete mapStyleIt->second;
		mapStyleAssets.erase(mapStyleIt);  // ���������Ƴ�����Դ
		std::string message = U8_TO_CHARPTR("��ͼ��ʽ��Դ ") + assetId + U8_TO_CHARPTR(" ������");
		Logger::Instance().Log(LogLevel::INFO, message);
	}
	else
	{
		std::string message = U8_TO_CHARPTR("��ͼ��ʽ��Դ ") + assetId + U8_TO_CHARPTR(" ������");
		Logger::Instance().Log(LogLevel::WARN, message);
	}
}

// �������е�ͼ��ʽ��Դ
void AssetStore::ClearMapStyleAssets()
{
	for (auto& [assetId, asset] : mapStyleAssets) {
		delete asset;  // �ͷ�ÿ����ͼ��ʽ��Դ
	}
	mapStyleAssets.clear(); // �������

	std::string message = U8_TO_CHARPTR("���������е�ͼ��ʽ��Դ");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// ����������Դ
void AssetStore::ClearAllAssets()
{
    ClearImageAssets();       // ��������ͼƬ��Դ
    ClearAudioAssets();       // ����������Ƶ��Դ
    ClearSoundEffectAssets(); // ����������Ч��Դ
    ClearFontAssets();        // ��������������Դ
	ClearMapStyleAssets();    // �������е�ͼ��ʽ��Դ

	std::string message = U8_TO_CHARPTR("������������Դ");
	Logger::Instance().Log(LogLevel::INFO, message);
}

// ���ͼƬ��Դ�������ļ�·������ͼƬ���洢�� imageAssets ������
void AssetStore::AddImageAsset(SDL_Renderer* renderer, const std::string& assetId, const std::string& path)
{
    // �����Դ�Ƿ��Ѵ���
    if (imageAssets.find(assetId) != imageAssets.end()) {
		std::string message = U8_TO_CHARPTR("ͼƬ��ԴID�Ѵ��ڣ�") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;
    }

    // ����ͼƬ�ļ�Ϊ surface
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
		std::string message = U8_TO_CHARPTR("����ͼƬʧ�ܣ�") + path + " " + IMG_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // �����������
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // ������Ҫ surface�������ͷ�
    if (!texture) {
		std::string message = U8_TO_CHARPTR("��������ʧ�ܣ�") + path + " " + SDL_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // ��������ӵ���Դ������
	imageAssets.emplace(assetId, texture);

	std::string message = U8_TO_CHARPTR("����ͼƬ��Դ���ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// �����Ƶ��Դ�������ļ�·��������Ƶ���洢�� audioAssets ������
void AssetStore::AddAudioAsset(const std::string& assetId, const std::string& path)
{
    // �����Դ�Ƿ��Ѵ���
    if (audioAssets.find(assetId) != audioAssets.end()) {
		std::string message = U8_TO_CHARPTR("��Ƶ��ԴID�Ѵ��ڣ�") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;
    }

    // ������Ƶ��Դ
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
		std::string message = U8_TO_CHARPTR("������Ƶʧ�ܣ�") + path + " " + Mix_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // ����Ƶ��Դ��ӵ�������
	audioAssets.emplace(assetId, music);

	std::string message = U8_TO_CHARPTR("������Ƶ��Դ���ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// �����Ч��Դ�������ļ�·��������Ч���洢�� soundEffectAssets ������
void AssetStore::AddSoundEffectAsset(const std::string& assetId, const std::string& path)
{
    // �����Դ�Ƿ��Ѵ���
    if (soundEffectAssets.find(assetId) != soundEffectAssets.end()) {
		std::string message = U8_TO_CHARPTR("��Ч��ԴID�Ѵ��ڣ�") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;
    }

    // ������Ч��Դ
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
		std::string message = U8_TO_CHARPTR("������Чʧ�ܣ�") + path + " " + Mix_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // ����Ч��Դ��ӵ�������
	soundEffectAssets.emplace(assetId, chunk);

	std::string message = U8_TO_CHARPTR("������Ч��Դ���ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// ���������Դ�������ļ�·���������岢�洢�� fontAssets ������
void AssetStore::AddFontAsset(const std::string& assetId, const std::string& path, size_t psize)
{
    // �����Դ�Ƿ��Ѵ���
    if (fontAssets.find(assetId) != fontAssets.end()) {
		std::string message = U8_TO_CHARPTR("������ԴID�Ѵ��ڣ�") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;
    }

    // ����������Դ
    TTF_Font* font = TTF_OpenFont(path.c_str(), psize);
    if (!font) {
		std::string message = U8_TO_CHARPTR("��������ʧ�ܣ�") + path + " " + TTF_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // ��������Դ��ӵ�������
	fontAssets.emplace(assetId, font);

	std::string message = U8_TO_CHARPTR("����������Դ���ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// ��ӵ�ͼ��ʽ��Դ�������ļ�·�����ص�ͼ��ʽ���洢�� mapStyleAssets ������
void AssetStore::AddMapStyleAsset(const std::string& assetId, const std::string& path)
{
	// �����Դ�Ƿ��Ѵ���
	if (mapStyleAssets.find(assetId) != mapStyleAssets.end()) {
		std::string message = U8_TO_CHARPTR("��ͼ��ʽ��ԴID�Ѵ��ڣ�") + assetId;
		Logger::Instance().Log(LogLevel::WARN, message);
		return;
	}

	MapStyle* mapStyle = new MapStyle(path); // ������ͼ��ʽ����

	// ����ͼ��ʽ��Դ��ӵ�������
	mapStyleAssets.emplace(assetId, mapStyle);

	std::string message = U8_TO_CHARPTR("���ص�ͼ��ʽ��Դ���ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
	Logger::Instance().Log(LogLevel::INFO, message);
}

// �޸�ͼƬ��Դ������·������ͼƬ���洢�������ԴID�Ѵ�������£����������˳���
void AssetStore::ChangeImageAsset(SDL_Renderer* renderer, const std::string& assetId, const std::string& path)
{
    // �����ԴID�Ƿ���ڣ����������˳�
    if (imageAssets.find(assetId) == imageAssets.end()) {
		std::string message = U8_TO_CHARPTR("ͼƬ��ԴID�����ڣ��˳�������") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;  // ��Դ������ʱ��ֱ���˳�
    }

    // ���پɵ�ͼƬ��Դ
    SDL_DestroyTexture(imageAssets[assetId]);

    // �����µ�ͼƬ��Դ
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
		std::string message = U8_TO_CHARPTR("����ͼƬʧ�ܣ�") + path + " " + SDL_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // �����µ�����
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // �ͷ� surface �ڴ�
    if (!texture) {
		std::string message = U8_TO_CHARPTR("��������ʧ�ܣ�") + path + " " + SDL_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // �洢�µ�������Դ����
    imageAssets[assetId] = texture;

	std::string message = U8_TO_CHARPTR("�滻ͼƬ��Դ �ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// �޸���Ƶ��Դ������·��������Ƶ���洢�������ԴID�Ѵ�������£����������˳���
void AssetStore::ChangeAudioAsset(const std::string& assetId, const std::string& path)
{
    // �����ԴID�Ƿ���ڣ����������˳�
    if (audioAssets.find(assetId) == audioAssets.end()) {
		std::string message = U8_TO_CHARPTR("��Ƶ��ԴID�����ڣ��˳�������") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;  // ��Դ������ʱ��ֱ���˳�
    }

    // ���پɵ���Ƶ��Դ
    Mix_FreeMusic(audioAssets[assetId]);

    // �����µ���Ƶ��Դ
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (!music) {
		std::string message = U8_TO_CHARPTR("������Ƶʧ�ܣ�") + path + " " + Mix_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // �洢�µ���Ƶ��Դ
    audioAssets[assetId] = music;

	std::string message = U8_TO_CHARPTR("�滻��Ƶ��Դ �ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// �޸���Ч��Դ������·��������Ч���洢�������ԴID�Ѵ�������£����������˳���
void AssetStore::ChangeSoundEffectAsset(const std::string& assetId, const std::string& path)
{
    // �����ԴID�Ƿ���ڣ����������˳�
    if (soundEffectAssets.find(assetId) == soundEffectAssets.end()) {
		std::string message = U8_TO_CHARPTR("��Ч��ԴID�����ڣ��˳�������") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;  // ��Դ������ʱ��ֱ���˳�
    }

    // ���پɵ���Ч��Դ
    Mix_FreeChunk(soundEffectAssets[assetId]);

    // �����µ���Ч��Դ
    Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
		std::string message = U8_TO_CHARPTR("������Чʧ�ܣ�") + path + " " + Mix_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // �洢�µ���Ч��Դ
    soundEffectAssets[assetId] = chunk;

	std::string message = U8_TO_CHARPTR("�滻��Ч��Դ �ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// �޸�������Դ������·���������岢�洢�������ԴID�Ѵ�������£����������˳���
void AssetStore::ChangeFontAsset(const std::string& assetId, const std::string& path, size_t psize)
{
    // �����ԴID�Ƿ���ڣ����������˳�
    if (fontAssets.find(assetId) == fontAssets.end()) {
		std::string message = U8_TO_CHARPTR("������ԴID�����ڣ��˳�������") + assetId;
        Logger::Instance().Log(LogLevel::WARN, message);
        return;  // ��Դ������ʱ��ֱ���˳�
    }

    // ���پɵ�������Դ
    TTF_CloseFont(fontAssets[assetId]);

    // �����µ�������Դ
    TTF_Font* font = TTF_OpenFont(path.c_str(), psize);
    if (!font) {
		std::string message = U8_TO_CHARPTR("��������ʧ�ܣ�") + path + " " + TTF_GetError();
        Logger::Instance().Log(LogLevel::ERROR, message);
        return;
    }

    // �洢�µ�������Դ
    fontAssets[assetId] = font;

	std::string message = U8_TO_CHARPTR("�滻������Դ �ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
    Logger::Instance().Log(LogLevel::INFO, message);
}

// �޸ĵ�ͼ��ʽ��Դ������·�����ص�ͼ��ʽ���洢�������ԴID�Ѵ�������£����������˳���
void AssetStore::ChangeMapStyleAsset(const std::string& assetId, const std::string& path)
{
	// �����ԴID�Ƿ���ڣ����������˳�
	if (mapStyleAssets.find(assetId) == mapStyleAssets.end()) {
		std::string message = U8_TO_CHARPTR("��ͼ��ʽ��ԴID�����ڣ��˳�������") + assetId;
		Logger::Instance().Log(LogLevel::WARN, message);
		return;  // ��Դ������ʱ��ֱ���˳�
	}

	// ���پɵĵ�ͼ��ʽ��Դ
	delete mapStyleAssets[assetId];

	// �����µĵ�ͼ��ʽ����
	MapStyle* mapStyle = new MapStyle(path);

	// �洢�µĵ�ͼ��ʽ��Դ
	mapStyleAssets[assetId] = mapStyle;
	std::string message = U8_TO_CHARPTR("�滻��ͼ��ʽ��Դ �ʲ���ţ�") + assetId + U8_TO_CHARPTR(" �ʲ�·����") + path;
	Logger::Instance().Log(LogLevel::INFO, message);
}

// ��ȡͼƬ��Դ��������Դ ID ���ض�Ӧ������
SDL_Texture* AssetStore::GetImageAsset(const std::string& assetId)
{
	if (imageAssets.find(assetId) != imageAssets.end()) 
    {
		return imageAssets[assetId]; // ���ض�Ӧ������
	}
    else 
    {
		return nullptr; // ��Դ�����ڣ����ؿ�ָ��
    }
}

// ��ȡ��Ƶ��Դ��������Դ ID ���ض�Ӧ��������
Mix_Music* AssetStore::GetAudioAsset(const std::string& assetId)
{
	if (audioAssets.find(assetId) != audioAssets.end())
	{
		return audioAssets[assetId]; // ���ض�Ӧ����Ƶ
	}
    else
    {
        return nullptr; // ��Դ�����ڣ����ؿ�ָ��
    }
}

// ��ȡ��Ч��Դ��������Դ ID ���ض�Ӧ����Ч
Mix_Chunk* AssetStore::GetSoundEffectAsset(const std::string& assetId)
{
	if (soundEffectAssets.find(assetId) != soundEffectAssets.end())
	{
		return soundEffectAssets[assetId]; // ���ض�Ӧ����Ч
	}
	else
	{
		return nullptr; // ��Դ�����ڣ����ؿ�ָ��
	}
}

// ��ȡ������Դ��������Դ ID ���ض�Ӧ������
TTF_Font* AssetStore::GetFontAsset(const std::string& assetId)
{
	if (fontAssets.find(assetId) != fontAssets.end())
	{
		return fontAssets[assetId]; // ���ض�Ӧ������
	}
	else
	{
		return nullptr; // ��Դ�����ڣ����ؿ�ָ��
	}
}

// ��ȡ��ͼ��ʽ��Դ��������Դ ID ���ض�Ӧ�ĵ�ͼ��ʽ
MapStyle* AssetStore::GetMapStyleAsset(const std::string& assetId)
{
	if (mapStyleAssets.find(assetId) != mapStyleAssets.end())
	{
		return mapStyleAssets[assetId]; // ���ض�Ӧ�ĵ�ͼ��ʽ
	}
	else
	{
		return nullptr; // ��Դ�����ڣ����ؿ�ָ��
	}
}
