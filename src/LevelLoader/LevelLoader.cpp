#include "LevelLoader.h"
#include "../GameEngine/GameEngine.h"

#include <filesystem>
#include <sol/sol.hpp>

LevelLoader::LevelLoader(sol::state& luaState, std::unique_ptr<Registry>& registry, std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer)
	: registry(registry), assetStore(assetStore), renderer(renderer), luaState(luaState) { }

bool LevelLoader::LoadLevel(int level)
{
	// 检查关卡号是否合法
	if (level < 1)
	{
		std::string message = U8_TO_CHARPTR("关卡数据加载失败，关卡号不合法：") + std::to_string(level);
		Logger::Instance().Log(LogLevel::ERROR, message);
		return false;
	}

	std::string levelFile = "./assets/scripts/Level" + std::to_string(level) + ".lua";
	std::string startMsg = U8_TO_CHARPTR("正在加载关卡数据：") + std::to_string(level);
	Logger::Instance().Log(LogLevel::INFO, startMsg);

	// 检查关卡文件是否存在
	if (!std::filesystem::exists(levelFile))
	{
		std::string message = U8_TO_CHARPTR("关卡数据加载失败，关卡文件不存在：") + levelFile;
		Logger::Instance().Log(LogLevel::ERROR, message);
		return false;
	}
	
	// 加载Lua脚本判断语法是否正确
	auto loadResult = luaState.load_file(levelFile);
	if (!loadResult.valid())
	{
		std::string message = U8_TO_CHARPTR("关卡数据加载失败，Lua脚本加载错误：") + loadResult.get<std::string>();
		Logger::Instance().Log(LogLevel::ERROR, message);
		return false;
	}

	// 执行Lua脚本
	auto luaResult = luaState.safe_script_file(levelFile);
	if (!luaResult.valid()) {
		sol::error err = luaResult;
		std::string message = U8_TO_CHARPTR("关卡数据加载失败，Lua脚本执行错误：") + std::string(err.what());
		Logger::Instance().Log(LogLevel::ERROR, message);
		return false;
	}

	// 清空注册表中的实体
	
	// 获取Level表
	sol::optional<sol::table> level_table = luaState["Level"];
	if (level_table == sol::nullopt)
	{
		return false;
	}

	// 获取Assets表和AssetsNum并加载资源
	sol::optional<sol::table> assets_table = level_table->get<sol::optional<sol::table>>("Assets");
	sol::optional<int> assetsNum = luaState["AssetsNum"];
	if (assets_table == sol::nullopt || assetsNum == sol::nullopt)
	{
		std::string message = U8_TO_CHARPTR("关卡数据加载失败，Lua脚本中缺少必要的assets资产信息");
		Logger::Instance().Log(LogLevel::ERROR, message);
		return false;
	}
	LoadAssets(assets_table.value(), assetsNum.value());

	// 获取Entities表和EntitiesNum并加载实体
	sol::optional<sol::table> entities_table = level_table->get<sol::optional<sol::table>>("Entities");
	sol::optional<int> entitiesNum = luaState["EntitiesNum"];
	if (entities_table == sol::nullopt || entitiesNum == sol::nullopt)
	{
		std::string message = U8_TO_CHARPTR("关卡数据加载失败，Lua脚本中缺少必要的entities实体信息");
		Logger::Instance().Log(LogLevel::ERROR, message);
		return false;
	}
	LoadEntities(entities_table.value(), entitiesNum.value());
	
	std::string endMsg = U8_TO_CHARPTR("关卡数据加载完成：") + std::to_string(level);
	Logger::Instance().Log(LogLevel::INFO, endMsg);
	/*
	{
		assetStore->AddImageAsset(renderer, "tank-tiger-right", "./assets/images/tank-tiger-spritesheet.png");
		assetStore->AddImageAsset(renderer, "f22", "./assets/images/f22.png");
		assetStore->AddImageAsset(renderer, "chopper", "./assets/images/chopper-spritesheet.png");
		assetStore->AddImageAsset(renderer, "whitechopper_sheet", "./assets/images/chopper-white-spritesheet.png");
		assetStore->AddImageAsset(renderer, "radar", "./assets/images/radar.png");
		assetStore->AddImageAsset(renderer, "bullet", "./assets/images/bullet.png");
		assetStore->AddImageAsset(renderer, "tree", "./assets/images/tree.png");

		assetStore->AddImageAsset(renderer, "jungle", "./assets/tilemaps/jungle.png");
		assetStore->AddMapStyleAsset("jungle", "./assets/tilemaps/jungle.map");

		assetStore->AddFontAsset("simhei_32", "./assets/fonts/simhei.ttf", 32);
		assetStore->AddFontAsset("simhei_16", "./assets/fonts/simhei.ttf", 16);


		// Read map file
		auto mapStyle = assetStore->GetMapStyleAsset("jungle");
		auto tileSizeX = mapStyle->pixelX * mapStyle->scaleX;
		auto tileSizeY = mapStyle->pixelY * mapStyle->scaleY;

		for (size_t y = 0; y < mapStyle->rangeY; ++y)
		{
			for (size_t x = 0; x < mapStyle->rangeX; ++x)
			{
				size_t tileId = mapStyle->map[y][x];
				size_t xPos = tileId % 10;
				size_t yPos = tileId / 10;

				double srcRectX = mapStyle->pixelX * xPos;
				double srcRectY = mapStyle->pixelY * yPos;

				Entity tile = registry->CreateEntity();
				tile.AddGroup("map");

				TransformComponent transformComponent({ x * tileSizeX, y * tileSizeY }, 0, { mapStyle->scaleX, mapStyle->scaleY });
				SpriteComponent spriteComponent("jungle", { mapStyle->pixelX, mapStyle->pixelY }, RenderLayer::Terrain, { srcRectX, srcRectY });
				tile.AddComponent<TransformComponent>(transformComponent);
				tile.AddComponent<SpriteComponent>(spriteComponent);
			}
		}

		GameEngine::mapWidth = tileSizeX * mapStyle->rangeX;
		GameEngine::mapHeight = tileSizeY * mapStyle->rangeY;

		// Create tank
		Entity tank = registry->CreateEntity();
		tank.AddGroup("enemy");

		TransformComponent transformComponent({ 500, 1280 });
		RigidBodyComponent rigidBodyComponent(10, { 1, 0 });
		SpriteComponent spriteComponent("tank-tiger-right", { 32, 32 }, RenderLayer::Characters);
		AnimationComponent animationComponent(1, 15);
		BoxColliderComponent boxColliderComponent({ 32, 32 });
		ProjectileEmitterComponent projectileEmitterComponent(true);
		CampComponent campComponent(Camp::Enemy);
		HealthComponent healthComponent;
		HealthBarComponent healthBarComponent("simhei_16");

		tank.AddComponent<TransformComponent>(transformComponent);
		tank.AddComponent<RigidBodyComponent>(rigidBodyComponent);
		tank.AddComponent<SpriteComponent>(spriteComponent);
		tank.AddComponent<AnimationComponent>(animationComponent);
		tank.AddComponent<BoxColliderComponent>(boxColliderComponent);
		tank.AddComponent<ProjectileEmitterComponent>(projectileEmitterComponent);
		tank.AddComponent<CampComponent>(campComponent);
		tank.AddComponent<HealthComponent>(healthComponent);
		tank.AddComponent<HealthBarComponent>(healthBarComponent);
		tank.AddComponent<EntityLifeCycleComponent>();


		// Create plane
		Entity plane = registry->CreateEntity();
		plane.AddGroup("enemy");

		TransformComponent transformComponent1({ 600, 500 }, 90, { 3, 3 });
		RigidBodyComponent rigidBodyComponent1(70, { 1, 0 });
		SpriteComponent spriteComponent1("f22", { 32, 32 }, RenderLayer::Characters);
		BoxColliderComponent boxColliderComponent1({ 32, 32 });
		CampComponent campComponent1(Camp::Enemy);
		HealthComponent healthComponent1;
		HealthBarComponent healthBarComponent1("simhei_16");

		plane.AddComponent<TransformComponent>(transformComponent1);
		plane.AddComponent<RigidBodyComponent>(rigidBodyComponent1);
		plane.AddComponent<SpriteComponent>(spriteComponent1);
		plane.AddComponent<BoxColliderComponent>(boxColliderComponent1);
		plane.AddComponent<CampComponent>(campComponent1);
		plane.AddComponent<HealthComponent>(healthComponent1);
		plane.AddComponent<HealthBarComponent>(healthBarComponent1);
		plane.AddComponent<EntityLifeCycleComponent>();

		// Create chopper
		Entity chopper = registry->CreateEntity();
		chopper.AddGroup("enemy");

		TransformComponent transformComponent2({ 1000, 500 }, 0, { 1, 1 });
		RigidBodyComponent rigidBodyComponent2(50, { 1, 0 });
		SpriteComponent spriteComponent2("chopper", { 32, 32 }, RenderLayer::Characters);
		AnimationComponent animationComponent2(2, 15);
		BoxColliderComponent boxColliderComponent2({ 32, 32 });
		CampComponent campComponent2(Camp::Enemy);
		HealthComponent healthComponent2;
		HealthBarComponent healthBarComponent2("simhei_16");

		chopper.AddComponent<TransformComponent>(transformComponent2);
		chopper.AddComponent<RigidBodyComponent>(rigidBodyComponent2);
		chopper.AddComponent<SpriteComponent>(spriteComponent2);
		chopper.AddComponent<AnimationComponent>(animationComponent2);
		chopper.AddComponent<BoxColliderComponent>(boxColliderComponent2);
		chopper.AddComponent<CampComponent>(campComponent2);
		chopper.AddComponent<HealthComponent>(healthComponent2);
		chopper.AddComponent<HealthBarComponent>(healthBarComponent2);
		chopper.AddComponent<EntityLifeCycleComponent>();

		// Create radar
		Entity radar = registry->CreateEntity();
		radar.AddGroup("UI");

		TransformComponent transformComponent3({ GameEngine::windowWidth - 74, 10 }, 0, { 1, 1 });
		SpriteComponent spriteComponent3("radar", { 64, 64 }, RenderLayer::UIBackground);
		AnimationComponent animationComponent3(8, 8);

		radar.AddComponent<TransformComponent>(transformComponent3);
		radar.AddComponent<SpriteComponent>(spriteComponent3);
		radar.AddComponent<AnimationComponent>(animationComponent3);

		// Create whiteChopper
		Entity whiteChopper = registry->CreateEntity();
		whiteChopper.AddTag("player");

		TransformComponent transformComponent4({ 300, 900 }, 0, { 1, 1 });
		RigidBodyComponent rigidBodyComponent4(150, { 0, 0 });
		SpriteComponent spriteComponent4("whitechopper_sheet", { 32, 32 }, RenderLayer::Characters);
		AnimationComponent animationComponent4(2, 15);
		BoxColliderComponent boxColliderComponent4({ 32, 32 });
		KeyboardControlComponent keyboardControlComponent4;
		CameraFollowComponent cameraFollowComponent4;
		ProjectileEmitterComponent projectileEmitterComponent4(false, 100);
		CampComponent campComponent4;
		HealthComponent healthComponent4;
		TextLabelComponent textLabelComponent4({ 1200, 10 }, U8_TO_CHARPTR("2D游戏引擎"), "simhei_32", { 255, 255, 255, 255 }, true);
		HealthBarComponent healthBarComponent4("simhei_16");

		whiteChopper.AddComponent<TransformComponent>(transformComponent4);
		whiteChopper.AddComponent<RigidBodyComponent>(rigidBodyComponent4);
		whiteChopper.AddComponent<SpriteComponent>(spriteComponent4);
		whiteChopper.AddComponent<AnimationComponent>(animationComponent4);
		whiteChopper.AddComponent<BoxColliderComponent>(boxColliderComponent4);
		whiteChopper.AddComponent<KeyboardControlComponent>(keyboardControlComponent4);
		whiteChopper.AddComponent<CameraFollowComponent>(cameraFollowComponent4);
		whiteChopper.AddComponent<ProjectileEmitterComponent>(projectileEmitterComponent4);
		whiteChopper.AddComponent<CampComponent>(campComponent4);
		whiteChopper.AddComponent<HealthComponent>(healthComponent4);
		whiteChopper.AddComponent<TextLabelComponent>(textLabelComponent4);
		whiteChopper.AddComponent<HealthBarComponent>(healthBarComponent4);
		whiteChopper.AddComponent<EntityLifeCycleComponent>();

		// Create trees
		Entity tree1 = registry->CreateEntity();
		tree1.AddGroup("obstacle");
		TransformComponent transformComponent5({ 800, 1280 });
		SpriteComponent spriteComponent5("tree", { 16, 32 }, RenderLayer::StaticGeometry);
		BoxColliderComponent boxColliderComponent5({ 16, 32 });
		CampComponent campComponent5(Camp::None);
		tree1.AddComponent<TransformComponent>(transformComponent5);
		tree1.AddComponent<SpriteComponent>(spriteComponent5);
		tree1.AddComponent<BoxColliderComponent>(boxColliderComponent5);
		tree1.AddComponent<CampComponent>(campComponent5);

		Entity tree2 = registry->CreateEntity();
		tree2.AddGroup("obstacle");
		TransformComponent transformComponent6({ 300, 1280 });
		SpriteComponent spriteComponent6("tree", { 16, 32 }, RenderLayer::StaticGeometry);
		BoxColliderComponent boxColliderComponent6({ 16, 32 });
		CampComponent campComponent6(Camp::None);
		tree2.AddComponent<TransformComponent>(transformComponent6);
		tree2.AddComponent<SpriteComponent>(spriteComponent6);
		tree2.AddComponent<BoxColliderComponent>(boxColliderComponent6);
		tree2.AddComponent<CampComponent>(campComponent6);
	}
	*/
	return true;
}

void LevelLoader::LoadAssets(const sol::table& assets, int assetsNum)
{
	for(int i = 1; i <= assetsNum; i++)
	{
		sol::optional<sol::table> asset = assets[i];
		if(asset == sol::nullopt)
		{
			continue;
		}

		std::string type = asset->get_or<std::string>("type", "");
		std::string id = asset->get_or<std::string>("id", "");
		std::string path = asset->get_or<std::string>("path", "");
		if(type.empty() || id.empty() || path.empty())
		{
			continue;
		}

		if(type == "texture")
		{
			assetStore->AddImageAsset(renderer, id, path);
		}
		else if(type == "font")
		{
            int fontSize = asset->get<sol::optional<int>>("font_size").value_or(16);
			assetStore->AddFontAsset(id, path, fontSize);
		}
		else if (type == "soundEffect")
		{
			assetStore->AddSoundEffectAsset(id, path);
		}
		else if (type == "audio")
		{
			assetStore->AddAudioAsset(id, path);
		}
		else if (type == "mapStyle")
		{
			assetStore->AddMapStyleAsset(id, path);
		}
		else
		{
			std::string message = U8_TO_CHARPTR("关卡数据加载失败，未知的资产类型：") + type;
			Logger::Instance().Log(LogLevel::WARN, message);
			continue;
		}
	}
}

void LevelLoader::LoadEntities(const sol::table& entities, int entitiesNum)
{
	for (int i = 1; i <= entitiesNum; i++)
	{
		sol::optional<sol::table> entityMap = entities[i];
		if (entityMap == sol::nullopt)
		{
			continue;
		}

		Entity entity = registry->CreateEntity();

		// 获取标签和组名并添加到实体
		std::string tag = entityMap->get_or<std::string>("tag", "");
		std::string group = entityMap->get_or<std::string>("group", "");
		entity.AddTag(tag);
		entity.AddGroup(group);

		sol::optional<sol::table> components = entityMap->get<sol::optional<sol::table>>("components");
		if(components == sol::nullopt)
		{
			continue;
		}

		// 获取TransformComponent转换组件并添加到实体
		sol::optional<sol::table> transform = components->get<sol::optional<sol::table>>("transform");
		if (transform != sol::nullopt)
		{
			TransformComponent transformComponent;

			// 获取 position（glm::f64vec2）
			sol::optional<sol::table> position = transform->get<sol::optional<sol::table>>("position");
			if (position != sol::nullopt)
			{
				transformComponent.position.x = position->get<sol::optional<double>>("x").value_or<double>(0.0);  // 获取 position.x
				transformComponent.position.y = position->get<sol::optional<double>>("y").value_or<double>(0.0);  // 获取 position.y
			}

			// 获取 rotation（double）
			transformComponent.rotation = transform->get<sol::optional<double>>("rotation").value_or<double>(0.0);  // 获取 rotation

			// 获取 scale（glm::f64vec2）
			sol::optional<sol::table> scale = transform->get<sol::optional<sol::table>>("scale");
			if(scale != sol::nullopt)
			{
				transformComponent.scale.x = scale->get<sol::optional<double>>("x").value_or<double>(1.0);  // 获取 scale.x
				transformComponent.scale.y = scale->get<sol::optional<double>>("y").value_or<double>(1.0);  // 获取 scale.y
			}
			
			entity.AddComponent<TransformComponent>(transformComponent);
		}

		// 获取RigidBodyComponent刚体组件并添加到实体
		sol::optional<sol::table> rigidbody = components->get<sol::optional<sol::table>>("rigidbody");
		if (rigidbody != sol::nullopt)
		{
			RigidBodyComponent rigidBodyComponent;

			// 获取 speed（double）
			rigidBodyComponent.speed = rigidbody->get<sol::optional<double>>("speed").value_or<double>(0.0);

			// 获取 velocity（glm::f64vec2）
			sol::optional<sol::table> velocity = rigidbody->get<sol::optional<sol::table>>("velocity");
			if (velocity != sol::nullopt)
			{
				rigidBodyComponent.velocity.x = velocity->get<sol::optional<double>>("x").value_or<double>(0.0);
				rigidBodyComponent.velocity.y = velocity->get<sol::optional<double>>("y").value_or<double>(0.0);
			}
			
			entity.AddComponent<RigidBodyComponent>(rigidBodyComponent);
		}

		// 获取CampComponent阵营组件并添加到实体
		sol::optional<sol::table> camp = components->get<sol::optional<sol::table>>("camp");
		if (camp != sol::nullopt)
		{
			CampComponent campComponent;

			// 获取 camp（Camp）
			std::string campStr = camp->get<sol::optional<std::string>>("camp").value_or<std::string>("None");
			campComponent.camp = CampFromString(campStr);
			
			entity.AddComponent<CampComponent>(campComponent);
		}

		// 获取SpriteComponent精灵组件并添加到实体
		sol::optional<sol::table> sprite = components->get<sol::optional<sol::table>>("sprite");
		if (sprite != sol::nullopt)
		{
			SpriteComponent spriteComponent;
			// 获取 textureId（std::string）
			spriteComponent.assetId = sprite->get<sol::optional<std::string>>("assetId").value_or<std::string>("");

			// 获取 size（glm::u64vec2）
			sol::optional<sol::table> size = sprite->get<sol::optional<sol::table>>("size");
			if (size != sol::nullopt)
			{
				spriteComponent.size.x = size->get<sol::optional<glm::uint64>>("width").value_or<glm::uint64>(0);
				spriteComponent.size.y = size->get<sol::optional<glm::uint64>>("height").value_or<glm::uint64>(0);
			}

			// 获取 layer（RenderLayer）
			std::string layerStr = sprite->get<sol::optional<std::string>>("layer").value_or<std::string>("Characters");
			spriteComponent.layer = RenderLayerFromString(layerStr);

			// 获取 srcRect（SDL_Rect）
			sol::optional<sol::table> srcRect = sprite->get<sol::optional<sol::table>>("srcRect");
			if (srcRect != sol::nullopt)
			{
				spriteComponent.srcRect.x = srcRect->get<sol::optional<int>>("x").value_or<int>(0);
				spriteComponent.srcRect.y = srcRect->get<sol::optional<int>>("y").value_or<int>(0);
				spriteComponent.srcRect.w = spriteComponent.size.x;
				spriteComponent.srcRect.h = spriteComponent.size.y;
			}
			
			entity.AddComponent<SpriteComponent>(spriteComponent);
		}

		// 获取AnimationComponent动画组件并添加到实体
		sol::optional<sol::table> animation = components->get<sol::optional<sol::table>>("animation");
		if (animation != sol::nullopt)
		{
			AnimationComponent animationComponent;
			
			// 获取 numFrames（Uint64）
			animationComponent.numFrames = animation->get<sol::optional<glm::uint64>>("numFrames").value_or<glm::uint64>(0);

			// 获取 currentFrame（Uint64）
			animationComponent.currentFrame = animation->get<sol::optional<glm::uint64>>("currentFrame").value_or<glm::uint64>(0);

			// 获取 frameSpeed（Uint64）
			animationComponent.frameSpeed = animation->get<sol::optional<glm::uint64>>("frameSpeed").value_or<glm::uint64>(0);

			// 获取 isLooping（bool）
			animationComponent.isLooping = animation->get<sol::optional<bool>>("isLooping").value_or<bool>(true);
			
			entity.AddComponent<AnimationComponent>(animationComponent);
		}

		// 获取BoxColliderComponent碰撞盒组件并添加到实体
		sol::optional<sol::table> boxCollider = components->get<sol::optional<sol::table>>("boxCollider");
		if (boxCollider != sol::nullopt)
		{
			BoxColliderComponent boxColliderComponent;

			// 获取 size（glm::u64vec2）
			sol::optional<sol::table> size = boxCollider->get<sol::optional<sol::table>>("size");
			if (size != sol::nullopt)
			{
				boxColliderComponent.size.x = size->get<sol::optional<glm::uint64>>("width").value_or<glm::uint64>(0);
				boxColliderComponent.size.y = size->get<sol::optional<glm::uint64>>("height").value_or<glm::uint64>(0);
			}

			// 获取 offset（glm::f64vec2）
			sol::optional<sol::table> offset = boxCollider->get<sol::optional<sol::table>>("offset");
			if (offset != sol::nullopt)
			{
				boxColliderComponent.offset.x = offset->get<sol::optional<double>>("x").value_or<double>(0.0);
				boxColliderComponent.offset.y = offset->get<sol::optional<double>>("y").value_or<double>(0.0);
			}
			
			entity.AddComponent<BoxColliderComponent>(boxColliderComponent);
		}

		// 获取KeyboardControlComponent键盘控制组件并添加到实体
		sol::optional<sol::table> keyboardControl = components->get<sol::optional<sol::table>>("keyboardControl");
		if (keyboardControl != sol::nullopt)
		{
			KeyboardControlComponent keyboardControlComponent;
			
			entity.AddComponent<KeyboardControlComponent>(keyboardControlComponent);
		}

		// 获取CameraFollowComponent摄像机跟随组件并添加到实体
		sol::optional<sol::table> cameraFollow = components->get<sol::optional<sol::table>>("cameraFollow");
		if (cameraFollow != sol::nullopt)
		{
			CameraFollowComponent cameraFollowComponent;
			
			entity.AddComponent<CameraFollowComponent>(cameraFollowComponent);
		}

		// 获取ProjectileEmitterComponent投射物发射组件并添加到实体
		sol::optional<sol::table> projectileEmitter = components->get<sol::optional<sol::table>>("projectileEmitter");
		if (projectileEmitter != sol::nullopt)
		{
			ProjectileEmitterComponent projectileEmitterComponent;

			// 获取 isEmit（bool）
			projectileEmitterComponent.isEmit = projectileEmitter->get<sol::optional<bool>>("isEmit").value_or<bool>(false);
			
			// 获取 repeatFrequency（int）
			projectileEmitterComponent.repeatFrequency = projectileEmitter->get<sol::optional<int>>("repeatFrequency").value_or<int>(2000);

			// 获取 projectileDuration（int）
			projectileEmitterComponent.projectileDuration = projectileEmitter->get<sol::optional<int>>("projectileDuration").value_or<int>(10000);

			// 获取 hitDamage（double）
			projectileEmitterComponent.hitDamage = projectileEmitter->get<sol::optional<double>>("hitDamage").value_or<double>(10.0);

			// 获取 speed（double）
			projectileEmitterComponent.speed = projectileEmitter->get<sol::optional<double>>("speed").value_or<double>(100.0);

			entity.AddComponent<ProjectileEmitterComponent>(projectileEmitterComponent);
		}

		// 获取HealthComponent生命值组件并添加到实体
		sol::optional<sol::table> health = components->get<sol::optional<sol::table>>("health");
		if (health != sol::nullopt)
		{
			HealthComponent healthComponent;

			// 获取 hp（double）
			healthComponent.hp = health->get<sol::optional<double>>("hp").value_or<double>(100.0);

			// 获取 maxHp（double）
			healthComponent.maxHp = health->get<sol::optional<double>>("maxHp").value_or<double>(100.0);

			entity.AddComponent<HealthComponent>(healthComponent);
		}

		// 获取HealthBarComponent生命条组件并添加到实体
		sol::optional<sol::table> healthBar = components->get<sol::optional<sol::table>>("healthBar");
		if (healthBar != sol::nullopt)
		{
			HealthBarComponent healthBarComponent;
			
			// 获取 assetId（std::string）
			healthBarComponent.assetId = healthBar->get<sol::optional<std::string>>("assetId").value_or<std::string>("");

			// 获取 borderColor（SDL_Color）
			sol::optional<sol::table> borderColor = healthBar->get<sol::optional<sol::table>>("borderColor");
			if (borderColor != sol::nullopt) 
			{
				if (borderColor->get<sol::optional<Uint8>>(1) && borderColor->get<sol::optional<Uint8>>(2) && borderColor->get<sol::optional<Uint8>>(3) && borderColor->get<sol::optional<Uint8>>(4))
				{
					// {255, 255, 255, 255}形式
					healthBarComponent.borderColor.r = borderColor->get<sol::optional<Uint8>>(1).value_or<Uint8>(255);
					healthBarComponent.borderColor.g = borderColor->get<sol::optional<Uint8>>(2).value_or<Uint8>(255);
					healthBarComponent.borderColor.b = borderColor->get<sol::optional<Uint8>>(3).value_or<Uint8>(255);
					healthBarComponent.borderColor.a = borderColor->get<sol::optional<Uint8>>(4).value_or<Uint8>(255);
				}
				else
				{
					// {r = 255, g = 255, b = 255, a = 255}形式
					healthBarComponent.borderColor.r = borderColor->get<sol::optional<Uint8>>("r").value_or<Uint8>(255);
					healthBarComponent.borderColor.g = borderColor->get<sol::optional<Uint8>>("g").value_or<Uint8>(255);
					healthBarComponent.borderColor.b = borderColor->get<sol::optional<Uint8>>("b").value_or<Uint8>(255);
					healthBarComponent.borderColor.a = borderColor->get<sol::optional<Uint8>>("a").value_or<Uint8>(255);
				}
			}

			// 获取 fillColor（SDL_Color）
			sol::optional<sol::table> fillColor = healthBar->get<sol::optional<sol::table>>("fillColor");
			if (fillColor != sol::nullopt)
			{
				if (fillColor->get<sol::optional<Uint8>>(1) && fillColor->get<sol::optional<Uint8>>(2) && fillColor->get<sol::optional<Uint8>>(3) && fillColor->get<sol::optional<Uint8>>(4))
				{
					// {255, 255, 255, 255}形式
					healthBarComponent.fillColor.r = fillColor->get<sol::optional<Uint8>>(1).value_or<Uint8>(255);
					healthBarComponent.fillColor.g = fillColor->get<sol::optional<Uint8>>(2).value_or<Uint8>(255);
					healthBarComponent.fillColor.b = fillColor->get<sol::optional<Uint8>>(3).value_or<Uint8>(255);
					healthBarComponent.fillColor.a = fillColor->get<sol::optional<Uint8>>(4).value_or<Uint8>(255);
				}
				else
				{
					// {r = 255, g = 255, b = 255, a = 255}形式
					healthBarComponent.fillColor.r = fillColor->get<sol::optional<Uint8>>("r").value_or<Uint8>(255);
					healthBarComponent.fillColor.g = fillColor->get<sol::optional<Uint8>>("g").value_or<Uint8>(255);
					healthBarComponent.fillColor.b = fillColor->get<sol::optional<Uint8>>("b").value_or<Uint8>(255);
					healthBarComponent.fillColor.a = fillColor->get<sol::optional<Uint8>>("a").value_or<Uint8>(255);
				}
			}

			// 获取 backgroundColor（SDL_Color）
			sol::optional<sol::table> backgroundColor = healthBar->get<sol::optional<sol::table>>("backgroundColor");
			if (backgroundColor != sol::nullopt)
			{
				if (backgroundColor->get<sol::optional<Uint8>>(1) && backgroundColor->get<sol::optional<Uint8>>(2) && backgroundColor->get<sol::optional<Uint8>>(3) && backgroundColor->get<sol::optional<Uint8>>(4))
				{
					// {255, 255, 255, 255}形式
					healthBarComponent.backgroundColor.r = backgroundColor->get<sol::optional<Uint8>>(1).value_or<Uint8>(255);
					healthBarComponent.backgroundColor.g = backgroundColor->get<sol::optional<Uint8>>(2).value_or<Uint8>(255);
					healthBarComponent.backgroundColor.b = backgroundColor->get<sol::optional<Uint8>>(3).value_or<Uint8>(255);
					healthBarComponent.backgroundColor.a = backgroundColor->get<sol::optional<Uint8>>(4).value_or<Uint8>(255);
				}
				else
				{
					// {r = 255, g = 255, b = 255, a = 255}形式
					healthBarComponent.backgroundColor.r = backgroundColor->get<sol::optional<Uint8>>("r").value_or<Uint8>(255);
					healthBarComponent.backgroundColor.g = backgroundColor->get<sol::optional<Uint8>>("g").value_or<Uint8>(255);
					healthBarComponent.backgroundColor.b = backgroundColor->get<sol::optional<Uint8>>("b").value_or<Uint8>(255);
					healthBarComponent.backgroundColor.a = backgroundColor->get<sol::optional<Uint8>>("a").value_or<Uint8>(255);
				}
			}
			
			entity.AddComponent<HealthBarComponent>(healthBarComponent);
		}

		// 获取TextLabelComponent文本标签组件并添加到实体
		sol::optional<sol::table> textLabel = components->get<sol::optional<sol::table>>("textLabel");
		if (textLabel != sol::nullopt)
		{
			TextLabelComponent textLabelComponent;

			// 获取 position（glm::i64vec2）
			sol::optional<sol::table> position = textLabel->get<sol::optional<sol::table>>("position");
			if (position != sol::nullopt)
			{
				textLabelComponent.position.x = position->get<sol::optional<glm::int64>>("x").value_or<glm::int64>(0);
				textLabelComponent.position.y = position->get<sol::optional<glm::int64>>("y").value_or<glm::int64>(0);
			}

			// 获取 text（std::string）
			textLabelComponent.text = textLabel->get<sol::optional<std::string>>("text").value_or<std::string>("");

			// 获取 assetId（std::string）
			textLabelComponent.assetId = textLabel->get<sol::optional<std::string>>("assetId").value_or<std::string>("");

			// 获取 color（SDL_Color）
			sol::optional<sol::table> color = textLabel->get<sol::optional<sol::table>>("color");
			if (color != sol::nullopt)
			{
				if (color->get<sol::optional<Uint8>>(1) && color->get<sol::optional<Uint8>>(2) && color->get<sol::optional<Uint8>>(3) && color->get<sol::optional<Uint8>>(4))
				{
					// {255, 255, 255, 255}形式
					textLabelComponent.color.r = color->get<sol::optional<Uint8>>(1).value_or<Uint8>(255);
					textLabelComponent.color.g = color->get<sol::optional<Uint8>>(2).value_or<Uint8>(255);
					textLabelComponent.color.b = color->get<sol::optional<Uint8>>(3).value_or<Uint8>(255);
					textLabelComponent.color.a = color->get<sol::optional<Uint8>>(4).value_or<Uint8>(255);
				}
				else
				{
					// {r = 255, g = 255, b = 255, a = 255}形式
					textLabelComponent.color.r = color->get<sol::optional<Uint8>>("r").value_or<Uint8>(255);
					textLabelComponent.color.g = color->get<sol::optional<Uint8>>("g").value_or<Uint8>(255);
					textLabelComponent.color.b = color->get<sol::optional<Uint8>>("b").value_or<Uint8>(255);
					textLabelComponent.color.a = color->get<sol::optional<Uint8>>("a").value_or<Uint8>(255);
				}
			}

			// 获取 isFixed（bool）
			textLabelComponent.isFixed = textLabel->get<sol::optional<bool>>("isFixed").value_or<bool>(false);

			entity.AddComponent<TextLabelComponent>(textLabelComponent);
		}

		// 获取EntityLifeCycleComponent实体生命周期组件并添加到实体
		sol::optional<sol::table> entityLifeCycle = components->get<sol::optional<sol::table>>("entityLifeCycle");
		if (entityLifeCycle != sol::nullopt)
		{
			EntityLifeCycleComponent entityLifeCycleComponent;
			
			entity.AddComponent<EntityLifeCycleComponent>(entityLifeCycleComponent);
		}
	}
}
