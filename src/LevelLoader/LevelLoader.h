#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <sol/forward.hpp>

#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/CampComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/HealthBarComponent.h"
#include "../Components/EntityLifeCycleComponent.h"
#include "../Components/ScriptComponent.h"

// ���������
class LevelLoader
{
public:
	LevelLoader(sol::state& luaState, std::unique_ptr<Registry>& registry, std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer);
	~LevelLoader() = default;

	bool LoadLevel(int level = 0);

private:
	sol::state& luaState;						// Lua״̬��

	std::unique_ptr<Registry>& registry;		// ʵ�����ע���
	std::unique_ptr<AssetStore>& assetStore;	// ��Դ�洢��
	SDL_Renderer* renderer;						// SDL��Ⱦ��

private:
	// ���عؿ���Դ
	void LoadAssets(const sol::table& assets_table, int assetsNum); 

	// ���عؿ�ʵ��
	void LoadEntities(const sol::table& entities_table, int entitiesNum);
};

#endif // !LEVELLOADER_H

