#ifndef ECS_H
#define ECS_H

#include <glm/glm.hpp>
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
	Entity(glm::uint64 id = -1) : id(id){};
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
	const std::string& GetTag() const;

	// ʵ�����group����
	void AddGroup(const std::string& input) const;

	// ʵ���޸�group����
	void ChangeGroup(const std::string& input) const;

	// ʵ���Ƴ�group����
	void RemoveGroup() const;

	// ���ʵ���Ƿ���group����
	bool HasGroup(const std::string& input) const;

	// ͨ��ʵ���ȡgroup����
	const std::string& GetGroup() const;

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
		entities.push_back(entity);
	}

	// �Ƴ�ʵ���ϵͳ
	inline void RemoveEntity(const Entity& entity)
	{
		entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity& other) {
			return entity == other;
			}), entities.end());
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
	Signature componentSignature; // ϵͳ�����ǩ��
	std::vector<Entity> entities; // ϵͳ�����ʵ���б�
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
};

// �����ģ���࣬��ʾ�����е���Ӧ������͵������
template <class TComponent>
class Pool: public IPool
{
public:
	Pool(glm::uint64 capacity = 16);
	virtual ~Pool() = default;

	// ������Ƿ�Ϊ��
	inline bool IsEmpty () const;

	// ��ȡ����ص��������
	inline glm::uint64 GetSize() const;

	// ���µ�������ش�С
	inline void Resize(glm::uint64 size);
	
	// ��������
	inline void Clear();

	// �������������
	inline void Add(TComponent component);

	// ͨ��ʵ��ID�������������أ�����ص��±���ʵ��IDһ��
	inline void Set(glm::uint64 entityId, TComponent component);

	// ��ȡ������ж�Ӧ�±���������ʵ��ID��Ӧ�����
	inline TComponent& Get(glm::uint64 index);

	// ��ȡ������ж�Ӧ�±���������ʵ��ID��Ӧ�����, const�汾
	inline const TComponent& Get(glm::uint64 index) const;

	// ��ȡ������ж�Ӧ�±�����
	inline TComponent& operator[](glm::uint64 index);

	// ��ȡ������ж�Ӧ�±�������const�汾
	inline const TComponent& operator[](glm::uint64 index) const;

private:
	std::vector<TComponent> data; // ��������ݣ��±�Ϊʵ��ID���洢��ͬʵ��ID��Ӧ��TComponent�������
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
	Registry(glm::uint64 capacity = 16);
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
	const std::string& GetTagByEntity(const Entity& entity) const;

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
	const std::string& GetGroupByEntity(const Entity& entity) const;

	// ͨ��group������ȡʵ�弯��
	const std::set<Entity>& GetEntitiesByGroup(const std::string& group) const;

	// entityʵ�������Ӧ���
	template <class TComponent, typename ...TArgs> void AddComponent(const Entity& entity, TArgs&& ...args);

	// entityʵ���Ƴ���Ӧ���
	template <class TComponent> void RemoveComponent(const Entity& entity);

	// ���entityʵ���Ƿ�����Ӧ���
	template <class TComponent> bool HasComponent(const Entity& entity) const;

	// ��ȡentityʵ�����Ӧ���
	template <class TComponent> TComponent& GetComponent(const Entity& entity) const;

	// ���������Ӧϵͳ
	template <class TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);

	// �����Ƴ���Ӧϵͳ
	template <class TSystem> void RemoveSystem();

	// ��������Ƿ���ĳһϵͳ
	template <class TSystem> bool HasSystem() const;

	// ��ȡ����ĳһϵͳ
	template <class TSystem> TSystem& GetSystem() const;

private:
	// stringת��д
	static std::string to_lower(std::string str);

	// stringת��д
	static std::string to_upper(std::string str);

private:  
	glm::uint64 numEntities = 0;		// ��ǰʵ��������ͬʱҲ������һ��ʵ��ID
	std::set<Entity> entities;			// ʵ�弯�ϣ��洢���д��ڵ�ʵ��
	std::set<Entity> entitiesToBeAdd;   // ϵͳ����ӵ�ʵ�弯��
	std::set<Entity> entitiesToBeKill;  // ϵͳ�����ٵ�ʵ�弯��
	std::deque<glm::uint64> freeIds;	// ���õĿ���ID����ʵ������ʱID����

	std::vector<std::shared_ptr<IPool>> componentPools;						// ����ؼ��ϣ��洢��Ӧ���������أ��±�Ϊ�������ID��eg. componentPools[componentId][entityId]
	std::vector<Signature> componentSignatures;								// ���ǩ�����ϣ��洢��Ӧʵ������ǩ�����±�Ϊʵ��ID��eg. componentSignatures[entityId]
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;	// ϵͳ���ϣ��洢����ϵͳ��keyΪϵͳ����

	// ��ǩ��ʵ��һһ��Ӧ��������ʵ������һ�Զ�Ĺ�ϵ
	std::unordered_map<std::string, Entity> tagEntityMap;					// tag��ǩΨһ��ͨ����ǩ�ҵ�ʵ��
	std::unordered_map<glm::uint64, std::string> entityTagMap;				// entityΨһ��ͨ��ʵ��ID�ҵ���ǩ
	std::unordered_map<std::string, std::set<Entity>> groupEntitiesMap;		// group����Ψһ��ͨ�������ҵ�ʵ�弯��
	std::unordered_map<glm::uint64, std::string> entityGroupMap;			// entityΨһ��ͨ��ʵ��ID�ҵ�����
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
/// �����ģ���࣬��ʾ�����е���Ӧ������͵������
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <param name="capacity">����س�ʼ����</param>
template <class TComponent>
Pool<TComponent>::Pool(glm::uint64 capacity)
{
	if (capacity < 16)
	{
		capacity = 16;
	}
	data.resize(capacity); // ��ʼ����������ݣ����ó�ʼ����
}

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
	return data.size();
}

/// <summary>
/// ���µ�������ش�С
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <param name="size">�µĴ�С</param>
template <class TComponent>
void Pool<TComponent>::Resize(glm::uint64 size)
{
	data.resize(size);
}

/// <summary>
/// ��������
/// </summary>
/// <typeparam name="TComponent"></typeparam>
template <class TComponent>
void Pool<TComponent>::Clear()
{
	data.clear();
}

/// <summary>
/// ����ض�����������
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="component"></param>
template <class TComponent>
void Pool<TComponent>::Add(TComponent component)
{
	data.push_back(component);
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
	// ���ʵ��ID��������صĴ�С�������������
	if (entityId >= GetSize())
	{
		data.resize(entityId * 2);
	}

	// ������ж�Ӧʵ��ID�±���������Ϊ��������
	data[entityId] = component;
}

/// <summary>
/// ��ȡ������ж�Ӧ�±���������ʵ��ID��Ӧ�����
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="index"></param>
/// <returns>���������</returns>
template <class TComponent>
TComponent& Pool<TComponent>::Get(glm::uint64 index)
{
	// ����±��Ƿ�Խ��
	if (index >= data.size())
	{
		std::string message = U8_TO_CHARPTR("����������±�Խ�磬���Է��ʵ��±�Ϊ") + std::to_string(index);
		Logger::Instance().Log(LogLevel::ERROR, message);
		throw std::out_of_range("Index out of bounds");
	}
	return data[index];
}

/// <summary>
/// ��ȡ������ж�Ӧ�±���������ʵ��ID��Ӧ�����, const�汾
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="index"></param>
/// <returns>���������</returns>
template <class TComponent> 
const TComponent& Pool<TComponent>::Get(glm::uint64 index) const
{
	// ����±��Ƿ�Խ��
	if (index >= data.size())
	{
		std::string message = U8_TO_CHARPTR("����������±�Խ�磬���Է��ʵ��±�Ϊ") + std::to_string(index);
		Logger::Instance().Log(LogLevel::ERROR, message);
		throw std::out_of_range("Index out of bounds");
	}
	return data[index];
}

/// <summary>
/// ��ȡ������ж�Ӧ�±�(ʵ��ID)�����
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="index"></param>
/// <returns>���������</returns>
template <class TComponent>
TComponent& Pool<TComponent>::operator[](glm::uint64 index)
{
	return data[index];
}

/// <summary>
/// ��ȡ������ж�Ӧ�±�(ʵ��ID)�������const�汾
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="index"></param>
/// <returns>���������</returns>
template <class TComponent>
const TComponent& Pool<TComponent>::operator[](glm::uint64 index) const
{
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
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

/// <summary>
/// �Ƴ�ϵͳ���ض���������󣬸���ϵͳ���ǩ��
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
template <class TComponent>
inline void System::DisRequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
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
	const auto componentId = Component<TComponent>::GetId();
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
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	if (componentId >= componentPools.size())
	{
		componentPools.resize(componentId * 2, nullptr);
	}

	if (componentPools[componentId] == nullptr)
	{
		std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
		componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
	TComponent newComponent(std::forward<TArgs>(args)...);
	componentPool->Set(entityId, newComponent);

	componentSignatures[entityId].set(componentId);

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
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	componentSignatures[entityId].reset(componentId);

	std::string message = U8_TO_CHARPTR("�Ƴ�") + TComponent::NAME + U8_TO_CHARPTR("��ʵ��(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}

/// <summary>
/// ���ʵ���Ƿ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <param name="entity">ʵ��</param>
template<class TComponent>
inline bool Registry::HasComponent(const Entity& entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();
	return componentSignatures[entityId].test(componentId);
}

/// <summary>
/// ��ȡʵ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <param name="entity">ʵ��</param>
/// <returns>���������</returns>
template<class TComponent>
inline TComponent& Registry::GetComponent(const Entity& entity) const
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);
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