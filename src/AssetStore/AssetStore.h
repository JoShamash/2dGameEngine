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

private:
    // �洢ͼƬ��Դ��������ʹ����Դ ID ��������
    std::unordered_map<std::string, SDL_Texture*> imageAssets;

    // �洢��Ƶ��Դ��������ʹ����Դ ID ��������
    std::unordered_map<std::string, Mix_Music*> audioAssets;

    // �洢��Ч��Դ��������ʹ����Դ ID ��������
    std::unordered_map<std::string, Mix_Chunk*> soundEffectAssets;

    // �洢������Դ��������ʹ����Դ ID ��������
    std::unordered_map<std::string, TTF_Font*> fontAssets;

	// �洢��ͼ��ʽ��������ʹ����Դ ID ��������
	std::unordered_map<std::string, MapStyle*> mapStyleAssets;
};

#endif // ASSETSTORE_H
