#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <nlohmann/json.hpp>

#include <string>
#include <memory>
#include <unordered_map>

#include "../Logger/Logger.h"
#include "../AssetStore/Image/ImageInfo.h"
#include "MapStyle.h"

// �������־ö��
enum class SortFlag
{
    ORIGINAL = 0,   // ԭʼ˳��
    ASCENDING = 1,  // ����
    DESCENDING = 2  // ����
};

class AssetStore
{
public:
    // ���캯������ʼ����Դ��������׼����Դ����
    AssetStore();

    // ����������������Դ��������Դ������
    ~AssetStore();

    // �����ض���ͼƬ��Դ
    void ClearImageAsset(const std::string& assetId);

    // ��������ͼƬ��Դ
    void ClearImageAssets();

    // �����ض�����Ƶ��Դ
    void ClearAudioAsset(const std::string& assetId);

    // ����������Ƶ��Դ
    void ClearAudioAssets();

    // �����ض�����Ч��Դ
    void ClearSoundEffectAsset(const std::string& assetId);

    // ����������Ч��Դ
    void ClearSoundEffectAssets();

    // �����ض���������Դ
    void ClearFontAsset(const std::string& assetId);

    // ��������������Դ
    void ClearFontAssets();

	// �����ض��ĵ�ͼ��ʽ��Դ
	void ClearMapStyleAsset(const std::string& assetId);

	// �������е�ͼ��ʽ��Դ
	void ClearMapStyleAssets();

    // ����������Դ
    void ClearAllAssets();

    // ���ͼƬ��Դ������·������ͼƬ���洢
    void AddImageAsset(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);

    // �����Ƶ��Դ������·��������Ƶ���洢
    void AddAudioAsset(const std::string& assetId, const std::string& path);

    // �����Ч��Դ������·��������Ч���洢
    void AddSoundEffectAsset(const std::string& assetId, const std::string& path);

    // ���������Դ������·���������岢�洢
    void AddFontAsset(const std::string& assetId, const std::string& path, size_t psize);

	// ��ӵ�ͼ��ʽ��Դ������·�����ص�ͼ��ʽ���洢
	void AddMapStyleAsset(const std::string& assetId, const std::string& path);

    // �޸�ͼƬ��Դ������·������ͼƬ���滻
    void ChangeImageAsset(SDL_Renderer* renderer, const std::string& assetId, const std::string& path);

    // �޸���Ƶ��Դ������·��������Ƶ���滻
    void ChangeAudioAsset(const std::string& assetId, const std::string& path);

    // �޸���Ч��Դ������·��������Ч���滻
    void ChangeSoundEffectAsset(const std::string& assetId, const std::string& path);

    // �޸�������Դ������·���������岢�滻
    void ChangeFontAsset(const std::string& assetId, const std::string& path, size_t psize = 24);

	// �޸ĵ�ͼ��ʽ��Դ������·�����ص�ͼ��ʽ���滻
	void ChangeMapStyleAsset(const std::string& assetId, const std::string& path);

    // ��ȡͼƬ��Դ������ͼƬ����
    SDL_Texture* GetImageAsset(const std::string& assetId);

    // ��ȡ��Ƶ��Դ������������
    Mix_Music* GetAudioAsset(const std::string& assetId);

    // ��ȡ��Ч��Դ��������Ч
    Mix_Chunk* GetSoundEffectAsset(const std::string& assetId);

    // ��ȡ������Դ����������
    TTF_Font* GetFontAsset(const std::string& assetId);

	// ��ȡ��ͼ��ʽ��Դ�����ص�ͼ��ʽ
	MapStyle* GetMapStyleAsset(const std::string& assetId);

	// ��ȡ����ͼƬ��ԴId�б�flagΪ0Ϊԭʼ�б�flagΪ1Ϊ�����б�flagΪ2Ϊ�����б�
	std::vector<std::string> GetAllImageAssetIds(SortFlag flag = SortFlag::ORIGINAL) const;

    // ��ȡ������Ƶ��ԴId�б�flagΪ0Ϊԭʼ�б�flagΪ1Ϊ�����б�flagΪ2Ϊ�����б�
    std::vector<std::string> GetAllAudioAssetIds(SortFlag flag = SortFlag::ORIGINAL) const;

    // ��ȡ������Ч��ԴId�б�flagΪ0Ϊԭʼ�б�flagΪ1Ϊ�����б�flagΪ2Ϊ�����б�
    std::vector<std::string> GetAllSoundEffectAssetIds(SortFlag flag = SortFlag::ORIGINAL) const;

    // ��ȡ����������ԴId�б�flagΪ0Ϊԭʼ�б�flagΪ1Ϊ�����б�flagΪ2Ϊ�����б�
	std::vector<std::string> GetAllFontAssetIds(SortFlag flag = SortFlag::ORIGINAL) const;

	// ��ȡ���е�ͼ��ʽ��ԴId�б�flagΪ0Ϊԭʼ�б�flagΪ1Ϊ�����б�flagΪ2Ϊ�����б�
	std::vector<std::string> GetAllMapStyleAssetIds(SortFlag flag = SortFlag::ORIGINAL) const;

	// ��ȡ����ͼƬ��ԴMap
    std::unordered_map<std::string, SDL_Texture*>& GetAllImageAssets();

    // ��ȡ������Ƶ��ԴMap
    std::unordered_map<std::string, Mix_Music*>& GetAllAudioAssets();

    // ��ȡ������Ч��ԴMap
    std::unordered_map<std::string, Mix_Chunk*>& GetAllSoundEffectAssets();

    // ��ȡ����������ԴMap
    std::unordered_map<std::string, TTF_Font*>& GetAllFontAssets();
    
	// ��ȡ���е�ͼ��ʽ��ԴMap
	std::unordered_map<std::string, MapStyle*>& GetAllMapStyleAssets();

    // ��ȡ����ͼƬ��Ϣ��ԴMap
	std::unordered_map<std::string, ImageInfo>& GetAllImageInfos();

private:
    // �洢ͼƬ��Դ��������ʹ����Դ ID ��������
    std::unordered_map<std::string, SDL_Texture*> imageAssets;

    // �洢ͼƬ��Ϣ��������ʹ����Դ ID ��������
	std::unordered_map<std::string, ImageInfo> imageInfos;

    // �洢��Ƶ��Դ��������ʹ����Դ ID ��������
    std::unordered_map<std::string, Mix_Music*> audioAssets;

    // �洢��Ч��Դ��������ʹ����Դ ID ��������
    std::unordered_map<std::string, Mix_Chunk*> soundEffectAssets;

    // �洢������Դ��������ʹ����Դ ID ��������
    std::unordered_map<std::string, TTF_Font*> fontAssets;

	// �洢��ͼ��ʽ��������ʹ����Դ ID ��������
	std::unordered_map<std::string, MapStyle*> mapStyleAssets;

private:
        nlohmann::json imageJson;
};

#endif // ASSETSTORE_H
