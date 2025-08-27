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

constexpr glm::uint64 MAX_COMPONENTS = 1024; // �����������

using Signature = std::bitset<MAX_COMPONENTS>; // ���λͼǩ����ÿ�������Ӧһ��bitλ

class Registry; // ǰ������ע�����

//   _____           _     _   _           
//  | ____|  _ __   | |_  (_) | |_   _   _ 
//  |  _|   | '_ \  | __| | | | __| | | | |
//  | |___  | | | | | |_  | | | |_  | |_| |
//  |_____| |_| |_|  \__| |_|  \__|  \__, |
//                                   |___/ 
// ʵ���࣬��ʾ�����еĶ�������ҡ����˵ȣ�������ʶ�͹���ʵ���Ψһ��ʶ��
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

	// ��ȡʵ���Ψһ��ʶ��
	inline const glm::uint64 GetId() const
	{
		return id;
	}

	// ����ʵ���ע���ָ��
	inline static void SetRegistry(Registry* registryPtr)
	{
		registry = registryPtr;
	}

	// ��ȡʵ���ע���ָ��
	inline static Registry* GetRegistry()
	{
		return registry;
	}

	// ʵ�����tag��ǩ
	void AddTag(const std::string& input) const;

	// ʵ���޸�tag��ǩ
	void ChangeTag(const std::string& input) const;

	// ʵ���Ƴ�tag��ǩ
	void RemoveTag() const;

	// ���ʵ���Ƿ���tag��ǩ
	bool HasTag(const std::string& input) const;

	// ͨ��ʵ���ȡtag��ǩ
	const std::string GetTag() const;

	// ʵ�����group����
	void AddGroup(const std::string& input) const;

	// ʵ���޸�group����
	void ChangeGroup(const std::string& input) const;

	// ʵ���Ƴ�group����
	void RemoveGroup() const;

	// ���ʵ���Ƿ���group����
	bool HasGroup(const std::string& input) const;

	// ͨ��ʵ���ȡgroup����
	const std::string GetGroup() const;

	// �ͷ�ʵ��
	void KillSelf() const;

	// ������
	template <class TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args) const;

	// �Ƴ����
	template <class TComponent> void RemoveComponent() const;

	// ����Ƿ������
	template <class TComponent> bool HasComponent() const;

	// ��ȡ���
	template <class TComponent> TComponent& GetComponent() const;

private:
	glm::uint64 id; // ʵ��Ψһ��ʶ��

	inline static Registry* registry = nullptr; // ע���ָ�룬���ڷ���ע���
};

//    ____                                                              _   
//   / ___|   ___    _ __ ___    _ __     ___    _ __     ___   _ __   | |_ 
//  | |      / _ \  | '_ ` _ \  | '_ \   / _ \  | '_ \   / _ \ | '_ \  | __|
//  | |___  | (_) | | | | | | | | |_) | | (_) | | | | | |  __/ | | | | | |_ 
//   \____|  \___/  |_| |_| |_| | .__/   \___/  |_| |_|  \___| |_| |_|  \__|
//                              |_|                                         
// ������࣬����������̳��Ը��࣬ͨ��IComponent*ʵ�ֶ�̬�������
struct IComponent
{
protected:
	inline static glm::uint64 nextId = 0; // ��һ�����Ψһ��ʶ��
};

// ���ģ���࣬��ʾ�����е��������λ���������������ȣ�������ʶ�͹��������Ψһ��ʶ��
template <class TComponent>
class Component: public IComponent
{
	inline static const glm::uint64 id = nextId++; // ���Ψһ��ʶ��������Component��

public:
	// ��ȡ�����Ψһ��ʶ��
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
// ϵͳ�࣬��ʾ�����е�ϵͳ������Ⱦϵͳ������ϵͳ�ȣ���������ʹ���ʵ�������߼�
class System 
{
public:
	System() = default;
	~System() = default;

	// ���ʵ�嵽ϵͳ
	inline void AddEntity(const Entity& entity)
	{
		auto id = entity.GetId();
		entities.push_back(entity);
		idToIdxMap[id] = entitySize;
		idxToIdMap[entitySize] = id;
		entitySize++;
	}

	// �Ƴ�ʵ���ϵͳ
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

	// ��ȡϵͳ�е�ʵ���б�
	inline const std::vector<Entity>& GetEntities() const
	{
		return entities;
	}
	
	// ��ȡϵͳ�����ǩ��
	inline const Signature& GetComponentSignature() const
	{
		return componentSignature;
	}

	// ������Ҫ�������ϵͳ
	template <class TComponent> void RequireComponent();

	// �Ƴ�ϵͳ���ض����������
	template <class TComponent> void DisRequireComponent();

	// ���ϵͳ�Ƿ���Ҫӵ���ض����
	template <class TComponent> bool HasComponent() const;

private:
	glm::uint64 entitySize = 0;									// ϵͳ�����ʵ���б�ĸ���	
	Signature componentSignature;								// ϵͳ�����ǩ��
	std::vector<Entity> entities;								// ϵͳ�����ʵ���б�
	std::unordered_map<glm::uint64, glm::uint64> idToIdxMap;	// ͨ��ʵ��ID�ҵ�entities��Ӧ���±�
	std::unordered_map<glm::uint64, glm::uint64> idxToIdMap;	// ͨ��entities��Ӧ���±��ҵ�ʵ��ID
};

//   ____                    _ 
//  |  _ \    ___     ___   | |
//  | |_) |  / _ \   / _ \  | |
//  |  __/  | (_) | | (_) | | |
//  |_|      \___/   \___/  |_|
//                             
// ����ؽӿ���
class IPool
{
public:
	IPool() = default;
	virtual ~IPool() = default;

	// ͨ��ʵ��ID�Ƴ�����������
	virtual void Remove(glm::uint64 entityId) = 0;
};

// �����ģ���࣬��ʾ�����е���Ӧ������͵������
template <class TComponent>
class Pool: public IPool
{
public:
	Pool() = default;
	virtual ~Pool() = default;

	// ������Ƿ�Ϊ��
	inline bool IsEmpty () const;

	// ��ȡ����ص��������
	inline glm::uint64 GetSize() const;

	// ��ʼ������ص�����
	inline void Reserve(glm::uint64 capacity = 1024)
	{
		data.reserve(capacity);
	}
	
	// ��������
	inline void Clear();

	// �������������ĩβ
	inline void Push_back(TComponent component);

	// ɾ������������ĩβ
	inline void Pop_back();
	
	// ͨ��ʵ��ID��������������
	inline void Set(glm::uint64 entityId, TComponent component);

	// ͨ��ʵ��ID�Ƴ�����������
	inline void Remove(glm::uint64 entityId) override;

	// ��ȡ�������ʵ���Ӧ�����
	inline TComponent& Get(glm::uint64 entityId);

	// ��ȡ�������ʵ���Ӧ�����, const�汾
	inline const TComponent& Get(glm::uint64 entityId) const;

	// ��ȡ������ж�Ӧʵ��ID�����
	inline TComponent& operator[](glm::uint64 entityId);

	// ��ȡ������ж�Ӧʵ��ID�������const�汾
	inline const TComponent& operator[](glm::uint64 entityId) const;

private:
	glm::uint64 componentSize = 0;										// ��ǰ����ص��������
	std::vector<TComponent> data;										// ��������ݣ��±�Ϊʵ��ID���洢��ͬʵ��ID��Ӧ��TComponent�������
	std::unordered_map<glm::uint64, glm::uint64> idToComponentIdxMap;	// ͨ��entityID�ҵ�componentPool��Ӧ��component�±�
	std::unordered_map<glm::uint64, glm::uint64> componentIdxToIdMap;	// ͨ��componentPool��Ӧ��component�±��ҵ�entityID
};

//   ____                   _         _                  
//  |  _ \    ___    __ _  (_)  ___  | |_   _ __   _   _ 
//  | |_) |  / _ \  / _` | | | / __| | __| | '__| | | | |
//  |  _ <  |  __/ | (_| | | | \__ \ | |_  | |    | |_| |
//  |_| \_\  \___|  \__, | |_| |___/  \__| |_|     \__, |
//                  |___/                          |___/ 
// ע����࣬����ECS��ʵ�塢�����ϵͳ
class Registry 
{
public:
	Registry(glm::uint64 capacity = 1024);
	~Registry() = default;

	// ����entityʵ��
	Entity CreateEntity();

	// ����entityʵ��
	void DestroyEntity(const Entity& entity);

	// ���entityʵ�嵽�ɼ���ϵͳ
	void AddEntityToSystems(const Entity& entity);

	// �Ƴ�entityʵ�������ϵͳ
	void RemoveEntityFromSystems(const Entity& entity);

	// ����ע���������Ӻ�ɾ����ʵ��
	void Update();

	// �Ƿ����tag��ǩ
	bool HasTag(const std::string& tag) const;

	// �Ƿ����group��ǩ
	bool HasGroup(const std::string& group) const;

	// ʵ�����tag��ǩ
	void EntityAddTag(const Entity& entity, const std::string& tag);

	// ʵ���޸�tag��ǩ
	void EntityChangeTag(const Entity& entity, const std::string& tag);

	// ʵ���Ƴ�tag��ǩ
	void EntityRemoveTag(const Entity& entity);

	// ���ʵ���Ƿ���tag��ǩ
	bool EntityHasTag(const Entity& entity, const std::string& tag) const;

	// ͨ��ʵ���ȡtag��ǩ
	const std::string GetTagByEntity(const Entity& entity) const;

	// ͨ��tag��ǩ��ȡʵ��
	const Entity& GetEntityByTag(const std::string& tag) const;

	// ʵ�����group����
	void EntityAddGroup(const Entity& entity, const std::string& group);

	// ʵ���޸�group����
	void EntityChangeGroup(const Entity& entity, const std::string& group);

	// ʵ���Ƴ�group����
	void EntityRemoveGroup(const Entity& entity);

	// ���ʵ���Ƿ���group����
	bool EntityHasGroup(const Entity& entity, const std::string& group) const;

	// ͨ��ʵ���ȡgroup����
	const std::string GetGroupByEntity(const Entity& entity) const;

	// ͨ��group������ȡʵ�弯��
	const std::set<Entity>& GetEntitiesByGroup(const std::string& group) const;

	// entityʵ�������Ӧ���
	template <class TComponent, typename ...TArgs> void AddComponent(const Entity& entity, TArgs&& ...args);

	// entityʵ���Ƴ���Ӧ���
	template <class TComponent> void RemoveComponent(const Entity& entity);

	// ���entityʵ���Ƿ�����Ӧ���
	template <class TComponent> bool HasComponent(const Entity& entity);

	// ��ȡentityʵ�����Ӧ���
	template <class TComponent> TComponent& GetComponent(const Entity& entity);

	// ���������Ӧϵͳ
	template <class TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);

	// �����Ƴ���Ӧϵͳ
	template <class TSystem> void RemoveSystem();

	// ��������Ƿ���ĳһϵͳ
	template <class TSystem> bool HasSystem() const;

	// ��ȡ����ĳһϵͳ
	template <class TSystem> TSystem& GetSystem() const;

public:
	// stringתСд
	static std::string to_lower(std::string str);

	// stringת��д
	static std::string to_upper(std::string str);

	// string�жϵ�����ͬ
	static bool is_same_word(const std::string& str1, const std::string& str2);

private:  
	glm::uint64 numEntities = 0;		// ��ǰʵ��������ͬʱҲ������һ��ʵ��ID
	glm::uint64 numComponents = 0;		// ��ǰ����������
	std::set<Entity> entities;			// ʵ�弯�ϣ��洢���д��ڵ�ʵ��
	std::set<Entity> entitiesToBeAdd;   // ϵͳ����ӵ�ʵ�弯��
	std::set<Entity> entitiesToBeKill;  // ϵͳ�����ٵ�ʵ�弯��
	std::deque<glm::uint64> freeIds;	// ���õĿ���ID����ʵ������ʱID����

	std::unordered_map<glm::uint64, glm::uint64> componentIdToPoolIdxMap;			// ͨ�����ID�ҵ�componentPools��Ӧ��componentPool�±�
	std::unordered_map<glm::uint64, glm::uint64> poolIdxToComponentIdMap;			// ͨ��componentPools��Ӧ��componentPool�±��ҵ����ID
	std::unordered_map<glm::uint64, glm::uint64> entityIdToSignaturesIdxMap;		// ͨ��ʵ��ID�ҵ�componentSignatures��Ӧ�±�
	std::unordered_map<glm::uint64, glm::uint64> signaturesIdxToEntityIdMap;		// ͨ��componentSignatures��Ӧ�±��ҵ�ʵ��ID

	std::vector<std::shared_ptr<IPool>> componentPools;								// ����ؼ��ϣ��洢��Ӧ���������أ��±�Ϊ�������ID��eg. componentPools[componentId][entityId]
	std::vector<Signature> componentSignatures;										// ���ǩ�����ϣ��洢��Ӧʵ������ǩ�����±�Ϊʵ��ID��eg. componentSignatures[entityId]
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;			// ϵͳ���ϣ��洢����ϵͳ��keyΪϵͳ����

	// ��ǩ��ʵ������һһ��Ӧ�Ĺ�ϵ��������ʵ������һ�Զ�Ĺ�ϵ
	std::unordered_map<std::string, Entity> tagEntityMap;							// tag��ǩΨһ��ͨ����ǩ�ҵ�ʵ��
	std::unordered_map<glm::uint64, std::string> entityTagMap;						// entityΨһ��ͨ��ʵ��ID�ҵ���ǩ
	std::unordered_map<std::string, std::set<Entity>> groupEntitiesMap;				// group����Ψһ��ͨ�������ҵ�ʵ�弯��
	std::unordered_map<glm::uint64, std::string> entityGroupMap;					// entityΨһ��ͨ��ʵ��ID�ҵ�����
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
///	������Ƿ�Ϊ��
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <returns>����ֵ����ʾ������Ƿ�Ϊ��</returns>
template <class TComponent>
bool Pool<TComponent>::IsEmpty() const
{
	return data.empty(); 
}

/// <summary>
/// ��ȡ����ص��������
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <returns>�������</returns>
template <class TComponent>
glm::uint64 Pool<TComponent>::GetSize() const
{
	return componentSize;
}

/// <summary>
/// ��������
/// </summary>
/// <typeparam name="TComponent"></typeparam>
template <class TComponent>
void Pool<TComponent>::Clear()
{
	data.clear();
	componentSize = 0;
}

/// <summary>
/// �������������ĩβ
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
/// ɾ������������ĩβ
/// </summary>
/// <typeparam name="TComponent"></typeparam>
template <class TComponent>
void Pool<TComponent>::Pop_back()
{
	data.pop_back();
	componentSize--;
}

/// <summary>
/// ͨ��ʵ��ID�������������أ�����ص��±���ʵ��IDһ��
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <param name="entityId">ʵ��ID</param>
/// <param name="component">���</param>
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
/// ��ȡ�������ʵ���Ӧ�����
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="entityId"></param>
/// <returns>���������</returns>
template <class TComponent>
TComponent& Pool<TComponent>::Get(glm::uint64 entityId)
{
	// ����±��Ƿ�Խ��
	if (idToComponentIdxMap.find(entityId) != idToComponentIdxMap.end())
	{
		glm::uint64 index = idToComponentIdxMap[entityId];
		if (index >= componentSize)
		{
			std::string message = U8_TO_CHARPTR("����������±�Խ�磬���Է��ʵ��±�Ϊ") + std::to_string(index);
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
/// ��ȡ�������ʵ���Ӧ�����, const�汾
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="entityId"></param>
/// <returns>���������</returns>
template <class TComponent> 
const TComponent& Pool<TComponent>::Get(glm::uint64 entityId) const
{
	// ����±��Ƿ�Խ��
	if (idToComponentIdxMap.find(entityId) != idToComponentIdxMap.end())
	{
		glm::uint64 index = idToComponentIdxMap.at(entityId);
		if (index >= componentSize)
		{
			std::string message = U8_TO_CHARPTR("����������±�Խ�磬���Է��ʵ��±�Ϊ") + std::to_string(index);
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
/// ��ȡ������ж�Ӧʵ��ID�����
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="index"></param>
/// <returns>���������</returns>
template <class TComponent>
TComponent& Pool<TComponent>::operator[](glm::uint64 entityId)
{
	glm::uint64 index = idToComponentIdxMap[entityId];
	return data[index];
}

/// <summary>
/// ��ȡ������ж�Ӧʵ��ID�������const�汾
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="index"></param>
/// <returns>���������</returns>
template <class TComponent>
const TComponent& Pool<TComponent>::operator[](glm::uint64 entityId) const
{
	glm::uint64 index = idToComponentIdxMap.at(entityId);
	return data[index];
}

/*-------------------------------------------------------------------------------------------------*/

/// <summary>
/// ����ض������ʵ��
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <typeparam name="TArgs">������캯����������</typeparam>
/// <param name="args">������캯������</param>
template<class TComponent, typename ...TArgs>
inline void Entity::AddComponent(TArgs&& ...args) const
{
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

/// <summary>
/// �Ƴ�ʵ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
template<class TComponent>
inline void Entity::RemoveComponent() const
{
	registry->RemoveComponent<TComponent>(*this);
}

/// <summary>
/// ���ʵ���Ƿ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <returns>����ֵ����ʾʵ���Ƿ����ض����</returns>
template<class TComponent>
inline bool Entity::HasComponent() const
{
	return registry->HasComponent<TComponent>(*this);
}

/// <summary>
/// ��ȡʵ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <returns>���������</returns>
template<class TComponent>
inline TComponent& Entity::GetComponent() const
{
	return registry->GetComponent<TComponent>(*this);
}

/*-------------------------------------------------------------------------------------------------*/

/// <summary>
/// �����Ҫ���ض������ϵͳ������ϵͳ���ǩ��
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
template <class TComponent>
inline void System::RequireComponent()
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

/// <summary>
/// �Ƴ�ϵͳ���ض���������󣬸���ϵͳ���ǩ��
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
template <class TComponent>
inline void System::DisRequireComponent()
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	componentSignature.reset(componentId);
}

/// <summary>
/// ���ϵͳ�Ƿ���Ҫӵ���ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <returns>����ֵ����ʾϵͳ�Ƿ���Ҫ�ض����</returns>
template <class TComponent>
inline bool System::HasComponent() const
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	return componentSignature.test(componentId);
}

/*-------------------------------------------------------------------------------------------------*/

/// <summary>
/// ��ʵ�������Ӧ���
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <typeparam name="TArgs">������캯����������</typeparam>
/// <param name="entity">ʵ��</param>
/// <param name="args">������캯������</param>
template<class TComponent, typename ...TArgs>
inline void Registry::AddComponent(const Entity& entity, TArgs&& ...args)
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	const glm::uint64 entityId = entity.GetId();
	
	// δ�ҵ�������򴴽������������
	if (componentIdToPoolIdxMap.find(componentId) == componentIdToPoolIdxMap.end())
	{
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		newComponentPool->Reserve();

		componentPools.push_back(newComponentPool);

		componentIdToPoolIdxMap[componentId] = numComponents;
		poolIdxToComponentIdMap[numComponents] = componentId;
		numComponents++; 
	}
	
	// ���������ӵ��������
	glm::uint64 poolIdx = componentIdToPoolIdxMap[componentId];
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[poolIdx]);
	TComponent newComponent(std::forward<TArgs>(args)...);
	componentPool->Set(entityId, newComponent);

	// ���ø�ʵ��������ǩ
	glm::uint64 signatureIdx = entityIdToSignaturesIdxMap[entityId];
	componentSignatures[signatureIdx].set(componentId);

	std::string message = U8_TO_CHARPTR("���") + TComponent::NAME + U8_TO_CHARPTR("��ʵ��(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}
/// <summary>
/// �Ƴ�ʵ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <param name="entity">ʵ��</param>
template<class TComponent>
inline void Registry::RemoveComponent(const Entity& entity)
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	const glm::uint64 entityId = entity.GetId();

	// �Ƴ���Ӧ������еĸ�ʵ�����
	if (componentIdToPoolIdxMap.find(componentId) != componentIdToPoolIdxMap.end())
	{
		glm::uint64 poolIdx = componentIdToPoolIdxMap[componentId];
		std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[poolIdx]);
		componentPool->Remove(entityId);
	}

	// ���ø�ʵ��������ǩ
	glm::uint64 signatureIdx = entityIdToSignaturesIdxMap[entityId];
	componentSignatures[signatureIdx].reset(componentId);

	std::string message = U8_TO_CHARPTR("�Ƴ�") + TComponent::NAME + U8_TO_CHARPTR("��ʵ��(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}

/// <summary>
/// ���ʵ���Ƿ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <param name="entity">ʵ��</param>
template<class TComponent>
inline bool Registry::HasComponent(const Entity& entity)
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	const glm::uint64 entityId = entity.GetId();

	// ��ȡ��ʵ������ǩ�������
	glm::uint64 signatureIdx = entityIdToSignaturesIdxMap[entityId];
	return componentSignatures[signatureIdx].test(componentId);
}

/// <summary>
/// ��ȡʵ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <param name="entity">ʵ��</param>
/// <returns>���������</returns>
template<class TComponent>
inline TComponent& Registry::GetComponent(const Entity& entity)
{
	const glm::uint64 componentId = Component<TComponent>::GetId();
	const glm::uint64 entityId = entity.GetId();

	// �Ӷ�Ӧ������ػ�ȡ��ʵ�����
	glm::uint64 poolIndex = componentIdToPoolIdxMap[componentId];
	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[poolIndex]);
	return componentPool->Get(entityId);
}

/// <summary>
/// ����ض�ϵͳ������
/// </summary>
/// <typeparam name="TSystem">ϵͳ����</typeparam>
/// <typeparam name="TArgs">ϵͳ���캯����������</typeparam>
/// <param name="args">ϵͳ���캯������</param>
template<class TSystem, typename ...TArgs>
inline void Registry::AddSystem(TArgs && ...args)
{
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.emplace(std::type_index(typeid(TSystem)), newSystem);
}

/// <summary>
/// �Ƴ�������ض�ϵͳ
/// </summary>
/// <typeparam name="TSystem">ϵͳ����</typeparam>
template<class TSystem>
inline void Registry::RemoveSystem()
{
	systems.erase(std::type_index(typeid(TSystem)));
}

/// <summary>
/// ��������Ƿ����ض�ϵͳ
/// </summary>
/// <typeparam name="TSystem">ϵͳ����</typeparam>
template<class TSystem>
inline bool Registry::HasSystem() const
{
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

/// <summary>
/// ��ȡ������ض�ϵͳ
/// </summary>
/// <typeparam name="TSystem">ϵͳ����</typeparam>
/// <returns>ϵͳ������</returns>
template<class TSystem>
inline TSystem& Registry::GetSystem() const
{
	const auto& systemIt = systems.find(std::type_index(typeid(TSystem)));
	if(systemIt == systems.end())
	{
		std::string message = U8_TO_CHARPTR("����δ�ҵ�ϵͳ����") + TSystem::NAME;
		Logger::Instance().Log(LogLevel::ERROR, message);
		throw std::runtime_error("System not found");
	}

	auto& system = systemIt->second;
	return *(std::static_pointer_cast<TSystem>(system));
}

#endif // !ECS_H