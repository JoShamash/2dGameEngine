#ifndef ECS_H
#define ECS_H

#include <glm/glm.hpp>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <set>
#include <typeindex>
#include <memory>
#include <iostream>

#include "../Logger/Logger.h"

const glm::uint64 MAX_COMPONENTS = 1024; // �����������

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
	Entity(glm::uint64 id) : id(id) {};
	Entity(const Entity& entity) = default;
	~Entity() = default;

	Entity& operator=(const Entity& other) = default;
	bool operator==(const Entity& other) const;
	bool operator!=(const Entity& other) const;
	bool operator>(const Entity& other) const;
	bool operator<(const Entity& other) const;

	// ��ȡʵ���Ψһ��ʶ��
	inline glm::uint64 GetId() const
	{
		return id;
	}

	// ������
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);

	// �Ƴ����
	template <typename TComponent> void RemoveComponent() const;

	// ����Ƿ������
	template <typename TComponent> bool HasComponent() const;

	// ��ȡ���
	template <typename TComponent> TComponent& GetComponent() const;

	Registry* registry = nullptr; // ע���ָ�룬���ڷ���ע���

private:
	glm::uint64 id;
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
template <typename T>
class Component: public IComponent
{
	inline static glm::uint64 id = nextId++; // ���Ψһ��ʶ��������Component��

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
	void AddEntityToSystem(const Entity &entity);

	// �Ƴ�ʵ���ϵͳ
	void RemoveEntityFromSystem(const Entity &entity);

	// ��ȡϵͳ�е�ʵ���б�
	const std::vector<Entity>& GetSystemEntities() const;
	
	// ��ȡϵͳ�����ǩ��
	const Signature& GetComponentSignature() const;

	// �ṩ��Ҫ�������ϵͳ
	template <typename TComponent> void RequireComponent();

	// �Ƴ�ϵͳ���ض����������
	template <typename TComponent> void DisRequireComponent();

	// ���ϵͳ�Ƿ���Ҫ�ض����
	template <typename TComponent> bool HasComponent() const;

private:
	Signature componentSignature; // ϵͳ���ǩ��
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
template <typename TComponent>
class Pool: public IPool
{
public:
	Pool(glm::uint64 capacity = 16);
	virtual ~Pool() = default;

	// ������Ƿ�Ϊ��
	inline bool IsEmpty();

	// ��ȡ����ص��������
	inline glm::uint64 GetSize();

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


	// ��ȡ������ж�Ӧ�±�����
	inline TComponent& operator[](glm::uint64 index);

private:
	std::vector<TComponent> data; // ��������ݣ��±�Ϊʵ��ID���洢��ͬʵ��ID��Ӧ��TComponent���
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

	// ����ʵ��
	Entity CreateEntity();

	// ����ʵ��
	void DestroyEntity(const Entity& entity);

	// ���ʵ�嵽ϵͳ
	void AddEntityToSystem(const Entity& entity);

	// �Ƴ�ʵ���ϵͳ
	void RemoveEntityFromSystem(const Entity& entity);

	// �����Ӧ���
	template <typename TComponent, typename ...TArgs> void AddComponent(const Entity& entity, TArgs&& ...args);

	// �Ƴ���Ӧ���
	template <typename TComponent> void RemoveComponent(const Entity& entity);

	// ���entityʵ���Ƿ�����Ӧ���
	template <typename TComponent> bool HasComponent(const Entity& entity) const;

	// ��ȡentityʵ�����Ӧ���
	template <typename TComponent> TComponent& GetComponent(const Entity& entity) const;

	// �����Ӧϵͳ
	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);

	// �Ƴ���Ӧϵͳ
	template <typename TSystem> void RemoveSystem();

	// ����Ƿ�����Ӧϵͳ
	template <typename TSystem> bool HasSystem() const;

	// ��ȡ��Ӧϵͳ
	template <typename TSystem> TSystem& GetSystem() const;

	// ����ע���������Ӻ�ɾ����ʵ��
	inline void Update();

private:  
	glm::uint64 numEntities = 0;		// ��ǰʵ��������ͬʱҲ������һ��ʵ��ID
	std::set<Entity> entitiesToBeAdd;   // ����ӵ�ʵ�弯��
	std::set<Entity> entitiesToBeKill;  // �����ٵ�ʵ�弯��

	std::vector<std::shared_ptr<IPool>> componentPools;					   // ����ؼ��ϣ��洢��Ӧ���������أ��±�Ϊ���ID��eg. componentPools[componentId]
	std::vector<Signature> componentSignatures;							   // ���ǩ�����ϣ��洢��Ӧʵ������ǩ�����±�Ϊʵ��ID��eg. componentSignatures[entityId]
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;  // ϵͳ���ϣ��洢����ϵͳ��keyΪϵͳ����
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
template <typename TComponent>
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
template <typename TComponent>
bool Pool<TComponent>::IsEmpty()
{
	return data.empty(); 
}

/// <summary>
/// ��ȡ����ص��������
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <returns>�������</returns>
template <typename TComponent>
glm::uint64 Pool<TComponent>::GetSize()
{
	return data.size();
}

/// <summary>
/// ���µ�������ش�С
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <param name="size">�µĴ�С</param>
template <typename TComponent>
void Pool<TComponent>::Resize(glm::uint64 size)
{
	data.resize(size);
}

/// <summary>
/// ��������
/// </summary>
/// <typeparam name="TComponent"></typeparam>
template <typename TComponent>
void Pool<TComponent>::Clear()
{
	data.clear();
}

/// <summary>
/// ����ض�����������
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="component"></param>
template <typename TComponent>
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
template <typename TComponent>
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
template <typename TComponent>
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
/// ��ȡ������ж�Ӧ�±�(ʵ��ID)�����
/// </summary>
/// <typeparam name="TComponent"></typeparam>
/// <param name="index"></param>
/// <returns>���������</returns>
template <typename TComponent>
TComponent& Pool<TComponent>::operator[](glm::uint64 index)
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
template<typename TComponent, typename ...TArgs>
inline void Entity::AddComponent(TArgs&& ...args)
{
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

/// <summary>
/// �Ƴ�ʵ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
template<typename TComponent>
inline void Entity::RemoveComponent() const
{
	registry->RemoveComponent<TComponent>(*this);
}

/// <summary>
/// ���ʵ���Ƿ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <returns>����ֵ����ʾʵ���Ƿ����ض����</returns>
template<typename TComponent>
inline bool Entity::HasComponent() const
{
	return registry->HasComponent<TComponent>(*this);
}

/// <summary>
/// ��ȡʵ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <returns>���������</returns>
template<typename TComponent>
inline TComponent& Entity::GetComponent() const
{
	return registry->GetComponent<TComponent>(*this);
}

/*-------------------------------------------------------------------------------------------------*/

/// <summary>
/// �����Ҫ���ض������ϵͳ������ϵͳ���ǩ��
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
template <typename TComponent>
inline void System::RequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

/// <summary>
/// �Ƴ�ϵͳ���ض���������󣬸���ϵͳ���ǩ��
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
template <typename TComponent>
inline void System::DisRequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.reset(componentId);
}

/// <summary>
/// ���ϵͳ�Ƿ���Ҫ�ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <returns>����ֵ����ʾϵͳ�Ƿ���Ҫ�ض����</returns>
template <typename TComponent>
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
template<typename TComponent, typename ...TArgs>
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
template<typename TComponent>
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
template<typename TComponent>
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
template<typename TComponent>
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
template<typename TSystem, typename ...TArgs>
inline void Registry::AddSystem(TArgs && ...args)
{
	std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
	systems.emplace(std::type_index(typeid(TSystem)), newSystem);
}

/// <summary>
/// �Ƴ�������ض�ϵͳ
/// </summary>
/// <typeparam name="TSystem">ϵͳ����</typeparam>
template<typename TSystem>
inline void Registry::RemoveSystem()
{
	systems.erase(std::type_index(typeid(TSystem)));
}

/// <summary>
/// ��������Ƿ����ض�ϵͳ
/// </summary>
/// <typeparam name="TSystem">ϵͳ����</typeparam>
template<typename TSystem>
inline bool Registry::HasSystem() const
{
	return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

/// <summary>
/// ��ȡ������ض�ϵͳ
/// </summary>
/// <typeparam name="TSystem">ϵͳ����</typeparam>
/// <returns>ϵͳ������</returns>
template<typename TSystem>
inline TSystem& Registry::GetSystem() const
{
	auto& system = systems.find(std::type_index(typeid(TSystem)))->second;
	return *(std::static_pointer_cast<TSystem>(system));
}

#endif // !ECS_H