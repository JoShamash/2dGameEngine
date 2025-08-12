#ifndef ECS_H
#define ECS_H

#include <glm/glm.hpp>
#include <string>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <set>
#include <deque>
#include <typeindex>
#include <memory>
#include <iostream>

#include "../Logger/Logger.h"

constexpr glm::uint64 MAX_COMPONENTS = 1024; // 组件数量上限

using Signature = std::bitset<MAX_COMPONENTS>; // 组件位图签名，每个组件对应一个bit位

class Registry; // 前向声明注册表类

//   _____           _     _   _           
//  | ____|  _ __   | |_  (_) | |_   _   _ 
//  |  _|   | '_ \  | __| | | | __| | | | |
//  | |___  | | | | | |_  | | | |_  | |_| |
//  |_____| |_| |_|  \__| |_|  \__|  \__, |
//                                   |___/ 
// 实体类，表示引擎中的对象，如玩家、敌人等，用来标识和管理实体的唯一标识符
class Entity 
{
public:
	Entity(glm::uint64 id) : id(id){};
	Entity(const Entity& entity) = default;
	~Entity() = default;

	Entity& operator=(const Entity& other) = default;
	bool operator==(const Entity& other) const;
	bool operator!=(const Entity& other) const;
	bool operator>(const Entity& other) const;
	bool operator<(const Entity& other) const;

	// 获取实体的唯一标识符
	inline const glm::uint64 GetId() const
	{
		return id;
	}

	// 设置实体的注册表指针
	inline static void SetRegistry(Registry* registryPtr)
	{
		registry = registryPtr;
	}

	// 获取实体的注册表指针
	inline static Registry* GetRegistry()
	{
		return registry;
	}

	// 实体添加tag标签
	void AddTag(const std::string& input) const;

	// 实体修改tag标签
	void ChangeTag(const std::string& input) const;

	// 实体移除tag标签
	void RemoveTag() const;

	// 检查实体是否有tag标签
	bool HasTag(const std::string& input) const;

	// 通过实体获取tag标签
	const std::string GetTag() const;

	// 实体添加group组名
	void AddGroup(const std::string& input) const;

	// 实体修改group组名
	void ChangeGroup(const std::string& input) const;

	// 实体移除group组名
	void RemoveGroup() const;

	// 检查实体是否有group组名
	bool HasGroup(const std::string& input) const;

	// 通过实体获取group组名
	const std::string GetGroup() const;

	// 释放实体
	void KillSelf() const;

	// 添加组件
	template <class TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args) const;

	// 移除组件
	template <class TComponent> void RemoveComponent() const;

	// 检查是否有组件
	template <class TComponent> bool HasComponent() const;

	// 获取组件
	template <class TComponent> TComponent& GetComponent() const;

private:
	glm::uint64 id; // 实体唯一标识符

	inline static Registry* registry = nullptr; // 注册表指针，用于访问注册表
};

//    ____                                                              _   
//   / ___|   ___    _ __ ___    _ __     ___    _ __     ___   _ __   | |_ 
//  | |      / _ \  | '_ ` _ \  | '_ \   / _ \  | '_ \   / _ \ | '_ \  | __|
//  | |___  | (_) | | | | | | | | |_) | | (_) | | | | | |  __/ | | | | | |_ 
//   \____|  \___/  |_| |_| |_| | .__/   \___/  |_| |_|  \___| |_| |_|  \__|
//                              |_|                                         
// 组件基类，所有组件都继承自该类，通过IComponent*实现多态访问组件
struct IComponent
{
protected:
	inline static glm::uint64 nextId = 0; // 下一个组件唯一标识符
};

// 组件模板类，表示引擎中的组件，如位置组件、精灵组件等，用来标识和管理组件的唯一标识符
template <class TComponent>
class Component: public IComponent
{
	inline static const glm::uint64 id = nextId++; // 组件唯一标识符，属于Component类

public:
	// 获取组件的唯一标识符
	inline static glm::uint64 GetId()
	{
		return id;
	}
};

//   ____                  _                      
//  / ___|   _   _   ___  | |_    ___   _ __ ___  
//  \___ \  | | | | / __| | __|  / _ \ | '_ ` _ \ 
//   ___) | | |_| | \__ \ | |_  |  __/ | | | | | |
//  |____/   \__, | |___/  \__|  \___| |_| |_| |_|
//           |___/                                
// 系统类，表示引擎中的系统，如渲染系统、物理系统等，用来管理和处理实体的组件逻辑
class System 
{
public:
	System() = default;
	~System() = default;

	// 添加实体到系统
	inline void AddEntity(const Entity& entity)
	{
		auto id = entity.GetId();
		entities.push_back(entity);
		idToIdxMap[id] = entitySize;
		idxToIdMap[entitySize] = id;
		entitySize++;
	}

	// 移除实体从系统
	inline void RemoveEntity(const Entity& entity)
	{
		auto id = entity.GetId();
		if(idToIdxMap.find(id) != idToIdxMap.end())
		{
			auto idx = idToIdxMap[id];
			auto lastEntityId = idxToIdMap[--entitySize];

			idToIdxMap[lastEntityId] = idx;
			idxToIdMap[idx] = lastEntityId;
			idToIdxMap.erase(id);
			idxToIdMap.erase(entitySize);

			entities[idx] = entities.back();
			entities.pop_back();
		}
	}

	// 获取系统中的实体列表
	inline const std::vector<Entity>& GetEntities() const
	{
		return entities;
	}
	
	// 获取系统的组件签名
	inline const Signature& GetComponentSignature() const
	{
		return componentSignature;
	}

	// 设置需要的组件给系统
	template <class TComponent> void RequireComponent();

	// 移除系统对特定组件的需求
	template <class TComponent> void DisRequireComponent();

	// 检查系统是否需要拥有特定组件
	template <class TComponent> bool HasComponent() const;

private:
	glm::uint64 entitySize = 0;									// 系统管理的实体列表的个数	
	Signature componentSignature;								// 系统的组件签名
	std::vector<Entity> entities;								// 系统管理的实体列表
	std::unordered_map<glm::uint64, glm::uint64> idToIdxMap;	// 通过实体ID找到entities对应的下标
	std::unordered_map<glm::uint64, glm::uint64> idxToIdMap;	// 通过entities对应的下标找到实体ID
};

//   ____                    _ 
//  |  _ \    ___     ___   | |
//  | |_) |  / _ \   / _ \  | |
//  |  __/  | (_) | | (_) | | |
//  |_|      \___/   \___/  |_|
//                             
// 组件池接口类
class IPool
{
public:
	IPool() = default;
	virtual ~IPool() = default;

	// 通过实体ID移除组件从组件池
	virtual void Remove(glm::uint64 entityId) = 0;
};

// 组件池模板类，表示引擎中的相应组件类型的组件池
template <class TComponent>
class Pool: public IPool
{
public:
	Pool() = default;
	virtual ~Pool() = default;

	// 组件池是否为空
	inline bool IsEmpty () const;

	// 获取组件池的组件数量
	inline glm::uint64 GetSize() const;

	// 初始化组件池的容量
	inline void Reserve(glm::uint64 capacity = 1024)
	{
		data.reserve(capacity);
	}
	
	// 清空组件池
	inline void Clear();

	// 添加组件到组件池末尾
	inline void Push_back(TComponent component);

	// 删除组件从组件池末尾
	inline void Pop_back();
	
	// 通过实体ID设置组件到组件池
	inline void Set(glm::uint64 entityId, TComponent component);

	// 通过实体ID移除组件从组件池
	inline void Remove(glm::uint64 entityId) override;

	// 获取组件池中实体对应的组件
	inline TComponent& Get(glm::uint64 entityId);

	// 获取组件池中实体对应的组件, const版本
	inline const TComponent& Get(glm::uint64 entityId) const;

	// 获取组件池中对应实体ID的组件
	inline TComponent& operator[](glm::uint64 entityId);

	// 获取组件池中对应实体ID的组件，const版本
	inline const TComponent& operator[](glm::uint64 entityId) const;

private:
	glm::uint64 componentSize = 0;										// 当前组件池的组件数量
	std::vector<TComponent> data;										// 组件池数据，下标为实体ID，存储不同实体ID对应的TComponent类型组件
	std::unordered_map<glm::uint64, glm::uint64> idToComponentIdxMap;	// 通过entityID找到componentPool对应的component下标
	std::unordered_map<glm::uint64, glm::uint64> componentIdxToIdMap;	// 通过componentPool对应的component下标找到entityID
};

//   ____                   _         _                  
//  |  _ \    ___    __ _  (_)  ___  | |_   _ __   _   _ 
//  | |_) |  / _ \  / _` | | | / __| | __| | '__| | | | |
//  |  _ <  |  __/ | (_| | | | \__ \ | |_  | |    | |_| |
//  |_| \_\  \___|  \__, | |_| |___/  \__| |_|     \__, |
//                  |___/                          |___/ 
// 注册表类，管理ECS的实体、组件和系统
class Registry 
{
public:
	Registry(glm::uint64 capacity = 1024);
	~Registry() = default;

	// 创建entity实体
	Entity CreateEntity();

	// 销毁entity实体
	void DestroyEntity(const Entity& entity);

	// 添加entity实体到可兼容系统
	void AddEntityToSystems(const Entity& entity);

	// 移除entity实体从所有系统
	void RemoveEntityFromSystems(const Entity& entity);

	// 更新注册表，处理添加和删除的实体
	void Update();

	// 是否存在tag标签
	bool HasTag(const std::string& tag) const;

	// 是否存在group标签
	bool HasGroup(const std::string& group) const;

	// 实体添加tag标签
	void EntityAddTag(const Entity& entity, const std::string& tag);

	// 实体修改tag标签
	void EntityChangeTag(const Entity& entity, const std::string& tag);

	// 实体移除tag标签
	void EntityRemoveTag(const Entity& entity);

	// 检查实体是否有tag标签
	bool EntityHasTag(const Entity& entity, const std::string& tag) const;

	// 通过实体获取tag标签
	const std::string GetTagByEntity(const Entity& entity) const;

	// 通过tag标签获取实体
	const Entity& GetEntityByTag(const std::string& tag) const;

	// 实体添加group组名
	void EntityAddGroup(const Entity& entity, const std::string& group);

	// 实体修改group组名
	void EntityChangeGroup(const Entity& entity, const std::string& group);

	// 实体移除group组名
	void EntityRemoveGroup(const Entity& entity);

	// 检查实体是否有group组名
	bool EntityHasGroup(const Entity& entity, const std::string& group) const;

	// 通过实体获取group组名
	const std::string GetGroupByEntity(const Entity& entity) const;

	// 通过group组名获取实体集合
	const std::set<Entity>& GetEntitiesByGroup(const std::string& group) const;

	// entity实体添加相应组件
	template <class TComponent, typename ...TArgs> void AddComponent(const Entity& entity, TArgs&& ...args);

	// entity实体移除相应组件
	template <class TComponent> void RemoveComponent(const Entity& entity);

	// 检查entity实体是否有相应组件
	template <class TComponent> bool HasComponent(const Entity& entity);

	// 获取entity实体的相应组件
	template <class TComponent> TComponent& GetComponent(const Entity& entity);

	// 引擎添加相应系统
	template <class TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);

	// 引擎移除相应系统
	template <class TSystem> void RemoveSystem();

	// 检查引擎是否有某一系统
	template <class TSystem> bool HasSystem() const;

	// 获取引擎某一系统
	template <class TSystem> TSystem& GetSystem() const;

public:
	// string转小写
	static std::string to_lower(std::string str);

	// string转大写
	static std::string to_upper(std::string str);

	// string判断单词相同
	static bool is_same_word(const std::string& str1, const std::string& str2);

private:  
	glm::uint64 numEntities = 0;		// 当前实体数量，同时也代表下一个实体ID
	glm::uint64 numComponents = 0;		// 当前组件种类个数
	std::set<Entity> entities;			// 实体集合，存储所有存在的实体
	std::set<Entity> entitiesToBeAdd;   // 系统待添加的实体集合
	std::set<Entity> entitiesToBeKill;  // 系统待销毁的实体集合
	std::deque<glm::uint64> freeIds;	// 可用的空闲ID，当实体销毁时ID存入

	std::unordered_map<glm::uint64, glm::uint64> componentIdToPoolIdxMap;			// 通过组件ID找到componentPools对应的componentPool下标
	std::unordered_map<glm::uint64, glm::uint64> poolIdxToComponentIdMap;			// 通过componentPools对应的componentPool下标找到组件ID
	std::unordered_map<glm::uint64, glm::uint64> entityIdToSignaturesIdxMap;		// 通过实体ID找到componentSignatures对应下标
	std::unordered_map<glm::uint64, glm::uint64> signaturesIdxToEntityIdMap;		// 通过componentSignatures对应下标找到实体ID

	std::vector<std::shared_ptr<IPool>> componentPools;								// 组件池集合，存储对应组件的组件池，下标为组件类型ID，eg. componentPools[componentId][entityId]
	std::vector<Signature> componentSignatures;										// 组件签名集合，存储对应实体的组件签名，下标为实体ID，eg. componentSignatures[entityId]
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;			// 系统集合，存储所有系统，key为系统类型

	// 标签和实体属于一一对应的关系，组名和实体属于一对多的关系
	std::unordered_map<std::string, Entity> tagEntityMap;							// tag标签唯一，通过标签找到实体
	std::unordered_map<glm::uint64, std::string> entityTagMap;						// entity唯一，通过实体ID找到标签
	std::unordered_map<std::string, std::set<Entity>> groupEntitiesMap;				// group组名唯一，通过组名找到实体集合
	std::unordered_map<glm::uint64, std::string> entityGroupMap;					// entity唯一，通过实体ID找到组名
};

//   _____                              _           _          
//  |_   _|   ___   _ __ ___    _ __   | |   __ _  | |_    ___ 
//    | |    / _ \ | '_ ` _ \  | '_ \  | |  / _` | | __|  / _ \
//    | |   |  __/ | | | | | | | |_) | | | | (_| | | |_  |  __/
//    |_|    \___| |_| |_| |_| | .__/  |_|  \__,_|  \__|  \___|
//                             |_|                             
//   ___                       _                                     _             _     _                 
//  |_ _|  _ __ ___    _ __   | |   ___   _ __ ___     ___   _ __   | |_    __ _  | |_  (_)   ___    _ __  
//   | |  | '_ ` _ \  | '_ \  | |  / _ \ | '_ ` _ \   / _ \ | '_ \  | __|  / _` | | __| | |  / _ \  | '_ \ 
//   | |  | | | | | | | |_) | | | |  __/ | | | | | | |  __/ | | | | | |_  | (_| | | |_  | | | (_) | | | | |
//  |___| |_| |_| |_| | .__/  |_|  \___| |_| |_| |_|  \___| |_| |_|  \__|  \__,_|  \__| |_|  \___/  |_| |_|
//                    |_|                                                                                  
/// <summary>
///	组件池是否为空
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <returns>布尔值，表示组件池是否为空</returns>
template <class TComponent>
bool Pool<TComponent>::IsEmpty() const
{
	return data.empty(); 
}

/// <summary>
/// 获取组件池的组件数量
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <returns>组件数量</returns>
template <class TComponent>
glm::uint64 Pool<TComponent>::GetSize() const
{
	return componentSize;
}

/// <summary>
/// 清空组件池
/// </summary>
/// <typeparam name="TComponent"></typeparam>
template <class TComponent>
void Pool<TComponent>::Clear()
{
	data.clear();
	componentSize = 0;
}

/// <summary>
/// 添加组件到组件池末尾
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="component"></param>
template <class TComponent>
void Pool<TComponent>::Push_back(TComponent component)
{
	data.push_back(component);
	componentSize++;
}

/// <summary>
/// 删除组件从组件池末尾
/// </summary>
/// <typeparam name="TComponent"></typeparam>
template <class TComponent>
void Pool<TComponent>::Pop_back()
{
	data.pop_back();
	componentSize--;
}

/// <summary>
/// 通过实体ID设置组件到组件池，组件池的下标与实体ID一致
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <param name="entityId">实体ID</param>
/// <param name="component">组件</param>
template <class TComponent>
void Pool<TComponent>::Set(glm::uint64 entityId, TComponent component)
{
	if(idToComponentIdxMap.find(entityId) == idToComponentIdxMap.end())
	{
		idToComponentIdxMap[entityId] = componentSize;
		componentIdxToIdMap[componentSize] = entityId;
		Push_back(component);
	}
	else
	{
		glm::uint64 idx = idToComponentIdxMap[entityId];
		data[idx] = component;
	}
}

template<class TComponent>
inline void Pool<TComponent>::Remove(glm::uint64 entityId)
{
	if (idToComponentIdxMap.find(entityId) != idToComponentIdxMap.end())
	{
		glm::uint64 idx = idToComponentIdxMap[entityId];
		glm::uint64 lastEntityId = componentIdxToIdMap[componentSize - 1];

		idToComponentIdxMap[lastEntityId] = idx;
		componentIdxToIdMap[idx] = lastEntityId;
		idToComponentIdxMap.erase(entityId);
		componentIdxToIdMap.erase(componentSize - 1);

		data[idx] = data.back(); 
		Pop_back();
	}
}

/// <summary>
/// 获取组件池中实体对应的组件
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="entityId"></param>
/// <returns>组件的引用</returns>
template <class TComponent>
TComponent& Pool<TComponent>::Get(glm::uint64 entityId)
{
	// 检查下标是否越界
	if (idToComponentIdxMap.find(entityId) != idToComponentIdxMap.end())
	{
		glm::uint64 index = idToComponentIdxMap[entityId];
		if (index >= componentSize)
		{
			std::string message = U8_TO_CHARPTR("错误：组件池下标越界，尝试访问的下标为") + std::to_string(index);
			Logger::Instance().Log(LogLevel::ERROR, message);
			throw std::out_of_range("Index out of bounds");
		}
		return data[index];
	}
	else
	{
		throw std::out_of_range("entityId has no component");
	}
}

/// <summary>
/// 获取组件池中实体对应的组件, const版本
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="entityId"></param>
/// <returns>组件的引用</returns>
template <class TComponent> 
const TComponent& Pool<TComponent>::Get(glm::uint64 entityId) const
{
	// 检查下标是否越界
	if (idToComponentIdxMap.find(entityId) != idToComponentIdxMap.end())
	{
		glm::uint64 index = idToComponentIdxMap.at(entityId);
		if (index >= componentSize)
		{
			std::string message = U8_TO_CHARPTR("错误：组件池下标越界，尝试访问的下标为") + std::to_string(index);
			Logger::Instance().Log(LogLevel::ERROR, message);
			throw std::out_of_range("Index out of bounds");
		}
		return data[index];
	}
	else
	{
		throw std::out_of_range("entityId has no component");
	}
}

/// <summary>
/// 获取组件池中对应实体ID的组件
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="index"></param>
/// <returns>组件的引用</returns>
template <class TComponent>
TComponent& Pool<TComponent>::operator[](glm::uint64 entityId)
{
	glm::uint64 index = idToComponentIdxMap[entityId];
	return data[index];
}

/// <summary>
/// 获取组件池中对应实体ID的组件，const版本
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="index"></param>
/// <returns>组件的引用</returns>
template <class TComponent>
const TComponent& Pool<TComponent>::operator[](glm::uint64 entityId) const
{
	glm::uint64 index = idToComponentIdxMap.at(entityId);
	return data[index];
}

/*-------------------------------------------------------------------------------------------------*/

/// <summary>
/// 添加特定组件到实体
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <typeparam name="TArgs">组件构造函数参数类型</typeparam>
/// <param name="args">组件构造函数参数</param>
template<class TComponent, typename ...TArgs>
inline void Entity::AddComponent(TArgs&& ...args) const
{
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

/// <summary>
/// 移除实体的特定组件
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
template<class TComponent>
inline void Entity::RemoveComponent() const
{
	registry->RemoveComponent<TComponent>(*this);
}

/// <summary>
/// 检查实体是否有特定组件
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <returns>布尔值，表示实体是否有特定组件</returns>
template<class TComponent>
inline bool Entity::HasComponent() const
{
	return registry->HasComponent<TComponent>(*this);
}

/// <summary>
/// 获取实体的特定组件
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <returns>组件的引用</returns>
template<class TComponent>
inline TComponent& Entity::GetComponent() const
{
	return registry->GetComponent<TComponent>(*this);
}

/*-------------------------------------------------------------------------------------------------*/

/// <summary>
/// 添加需要的特定组件到系统，更改系统组件签名
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
template <class TComponent>
inline void System::RequireComponent()
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

/// <summary>
/// 移除系统对特定组件的需求，更改系统组件签名
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
template <class TComponent>
inline void System::DisRequireComponent()
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	componentSignature.reset(componentId);
}

/// <summary>
/// 检查系统是否需要拥有特定组件
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <returns>布尔值，表示系统是否需要特定组件</returns>
template <class TComponent>
inline bool System::HasComponent() const
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	return componentSignature.test(componentId);
}

/*-------------------------------------------------------------------------------------------------*/

/// <summary>
/// 给实体添加相应组件
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <typeparam name="TArgs">组件构造函数参数类型</typeparam>
/// <param name="entity">实体</param>
/// <param name="args">组件构造函数参数</param>
template<class TComponent, typename ...TArgs>
inline void Registry::AddComponent(const Entity& entity, TArgs&& ...args)
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	const glm::uint64 entityId = entity.GetId();
	
	// 未找到组件池则创建该类型组件池
	if (componentIdToPoolIdxMap.find(componentId) == componentIdToPoolIdxMap.end())
	{
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		newComponentPool->Reserve();

		componentPools.push_back(newComponentPool);

		componentIdToPoolIdxMap[componentId] = numComponents;
		poolIdxToComponentIdMap[numComponents] = componentId;
		numComponents++; 
	}
	
	// 将该组件添加到组件池中
	glm::uint64 poolIdx = componentIdToPoolIdxMap[componentId];
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[poolIdx]);
	TComponent newComponent(std::forward<TArgs>(args)...);
	componentPool->Set(entityId, newComponent);

	// 设置该实体的组件标签
	glm::uint64 signatureIdx = entityIdToSignaturesIdxMap[entityId];
	componentSignatures[signatureIdx].set(componentId);

	std::string message = U8_TO_CHARPTR("添加") + TComponent::NAME + U8_TO_CHARPTR("到实体(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}
/// <summary>
/// 移除实体的特定组件
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <param name="entity">实体</param>
template<class TComponent>
inline void Registry::RemoveComponent(const Entity& entity)
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	const glm::uint64 entityId = entity.GetId();

	// 移除对应组件池中的该实体组件
	if (componentIdToPoolIdxMap.find(componentId) != componentIdToPoolIdxMap.end())
	{
		glm::uint64 poolIdx = componentIdToPoolIdxMap[componentId];
		std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[poolIdx]);
		componentPool->Remove(entityId);
	}

	// 设置该实体的组件标签
	glm::uint64 signatureIdx = entityIdToSignaturesIdxMap[entityId];
	componentSignatures[signatureIdx].reset(componentId);

	std::string message = U8_TO_CHARPTR("移除") + TComponent::NAME + U8_TO_CHARPTR("从实体(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}

/// <summary>
/// 检查实体是否有特定组件
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <param name="entity">实体</param>
template<class TComponent>
inline bool Registry::HasComponent(const Entity& entity)
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	const glm::uint64 entityId = entity.GetId();

	// 获取该实体的组件签名并检查
	glm::uint64 signatureIdx = entityIdToSignaturesIdxMap[entityId];
	return componentSignatures[signatureIdx].test(componentId);
}

/// <summary>
/// 获取实体的特定组件
/// </summary>
/// <typeparam name="TComponent">组件类型</typeparam>
/// <param name="entity">实体</param>
/// <returns>组件的引用</returns>
template<class TComponent>
inline TComponent& Registry::GetComponent(const Entity& entity)
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	const glm::uint64 entityId = entity.GetId();

	// 从对应的组件池获取该实体组件
	glm::uint64 poolIndex = componentIdToPoolIdxMap[componentId];
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[poolIndex]);
	return componentPool->Get(entityId);
}

/// <summary>
/// 添加特定系统到引擎
/// </summary>
/// <typeparam name="TSystem">系统类型</typeparam>
/// <typeparam name="TArgs">系统构造函数参数类型</typeparam>
/// <param name="args">系统构造函数参数</param>
template<class TSystem, typename ...TArgs>
inline void Registry::AddSystem(TArgs && ...args)
{
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.emplace(std::type_index(typeid(TSystem)), newSystem);
}

/// <summary>
/// 移除引擎的特定系统
/// </summary>
/// <typeparam name="TSystem">系统类型</typeparam>
template<class TSystem>
inline void Registry::RemoveSystem()
{
	systems.erase(std::type_index(typeid(TSystem)));
}

/// <summary>
/// 检查引擎是否有特定系统
/// </summary>
/// <typeparam name="TSystem">系统类型</typeparam>
template<class TSystem>
inline bool Registry::HasSystem() const
{
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

/// <summary>
/// 获取引擎的特定系统
/// </summary>
/// <typeparam name="TSystem">系统类型</typeparam>
/// <returns>系统的引用</returns>
template<class TSystem>
inline TSystem& Registry::GetSystem() const
{
	const auto& systemIt = systems.find(std::type_index(typeid(TSystem)));
	if(systemIt == systems.end())
	{
		std::string message = U8_TO_CHARPTR("错误：未找到系统类型") + TSystem::NAME;
		Logger::Instance().Log(LogLevel::ERROR, message);
		throw std::runtime_error("System not found");
	}

	auto& system = systemIt->second;
	return *(std::static_pointer_cast<TSystem>(system));
}

#endif // !ECS_H