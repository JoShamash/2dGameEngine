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

using Signature = std::bitset<MAX_COMPONENTS>; // ���ǩ����ÿ�������Ӧһ��bitλ

class Registry; // ǰ������ע�����

// ʵ���࣬��ʾ�����еĶ���
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
	glm::uint64 GetId() const;

	// ������
	template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);

	// �Ƴ����
	template <typename TComponent> void RemoveComponent();

	// ����Ƿ������
	template <typename TComponent> bool HasComponent() const;

	// ��ȡ���
	template <typename TComponent> TComponent& GetComponent() const;

	Registry* registry = nullptr; // ʵ��ע���ָ�룬���ڷ���ע���

private:
	glm::uint64 id;
};

// ������࣬����������̳��Ը���
struct IComponent
{
protected:
	inline static glm::uint64 nextId = 0; // ��һ�����Ψһ��ʶ��
};

// ���ģ���࣬��ʾ�����е����
template <typename T>
class Component: public IComponent
{
	inline static glm::uint64 id = nextId++; // ���Ψһ��ʶ��

public:
	// ��ȡ�����Ψһ��ʶ��
	static glm::uint64 GetId()
	{
		return id;
	}
};

// ϵͳ�࣬��ʾ�����е�ϵͳ
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

	// �ṩ�����ϵͳ
	template <typename TComponent> void RequireComponent();

private:
	Signature componentSignature; // ϵͳ���ǩ��
	std::vector<Entity> entities; // ϵͳ�����ʵ���б�
};

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
	Pool(glm::uint64 capacity = 128)
	{
		data.resize(capacity);
	}
	virtual ~Pool() = default;

	// ������Ƿ�Ϊ��
	bool IsEmpty()
	{
		return data.empty();
	}

	// ��ȡ����ص��������
	glm::uint64 GetSize()
	{
		return data.size();
	}

	// ���µ�������ش�С
	void Resize(glm::uint64 size)
	{
		data.resize(size);
	}
	
	// ��������
	void Clear()
	{
		data.clear();
	}

	// �������������
	void Add(TComponent component)
	{
		data.push_back(component);
	}

	// ͨ��entityʵ��ID�±��������������أ�����ص��±���ʵ��IDһ��
	void Set(glm::uint64 entityId, TComponent component)
	{
		// ���ʵ��ID��������صĴ�С�������������
		if (entityId >= GetSize())
		{
			data.resize(entityId * 2);
		}

		// ������ж�Ӧʵ��ID�±���������Ϊ��������
		data[entityId] = component;
	}

	// ��ȡ������ж�Ӧ�±�����
	TComponent& Get(glm::uint64 index)
	{
		return static_cast<TComponent&>(data[index]);
	}


	// ��ȡ������ж�Ӧ�±�����
	TComponent& operator[](glm::uint64 index)
	{
		return data[index];
	}

private:
	std::vector<TComponent> data; // ��������ݣ��±�Ϊʵ��ID���洢��Ӧ�����
};

// ע����࣬����ECS��ʵ�塢�����ϵͳ
class Registry 
{
public:
	Registry(glm::uint64 capacity = 128);
	~Registry() = default;

	// ����ʵ��
	Entity& CreateEntity();

	// ����ʵ��
	void DestroyEntity(const Entity& entity);

	// ���ʵ�嵽ϵͳ
	void AddEntityToSystem(const Entity& entity);

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
	void Update();

private:  
	glm::uint64 numEntities = 0;		// ��ǰʵ��������ͬʱҲ������һ��ʵ��ID
	std::set<Entity> entitiesToBeAdd;   // ����ӵ�ʵ�弯��
	std::set<Entity> entitiesToBeKill;  // �����ٵ�ʵ�弯��

	std::vector<std::shared_ptr<IPool>> componentPools;					   // ����ؼ��ϣ��洢��������أ��±�Ϊ���ID
	std::vector<Signature> entityComponentSignatures;					   // ʵ�����ǩ�����ϣ��洢ÿ��ʵ������ǩ�����±�Ϊʵ��ID
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;  // ϵͳ���ϣ��洢����ϵͳ��keyΪϵͳ����
};


/// <summary>
/// ����ض������ʵ��
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <typeparam name="TArgs">������캯����������</typeparam>
/// <param name="args">������캯������</param>
template<typename TComponent, typename ...TArgs>
inline void Entity::AddComponent(TArgs && ...args)
{
	registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

/// <summary>
/// �Ƴ�ʵ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
template<typename TComponent>
inline void Entity::RemoveComponent()
{
	registry->RemoveComponent<TComponent>(*this);
}

/// <summary>
/// ���ʵ���Ƿ����ض����
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
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

/// <summary>
/// �����Ҫ���ض������ϵͳ
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
template <typename TComponent>
inline void System::RequireComponent()
{
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

/// <summary>
/// ��ʵ�������Ӧ���
/// </summary>
/// <typeparam name="TComponent">�������</typeparam>
/// <typeparam name="TArgs">������캯����������</typeparam>
/// <param name="entity">ʵ��</param>
/// <param name="args">������캯������</param>
template<typename TComponent, typename ...TArgs>
inline void Registry::AddComponent(const Entity& entity, TArgs && ...args)
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
	entityComponentSignatures[entityId].set(componentId);

	std::string message = reinterpret_cast<const char*>(u8"���") + TComponent::NAME + reinterpret_cast<const char*>(u8"��ʵ��(id=") + std::to_string(entityId) + ")";
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
	entityComponentSignatures[entityId].reset(componentId);

	std::string message = reinterpret_cast<const char*>(u8"�Ƴ�") + TComponent::NAME + reinterpret_cast<const char*>(u8"��ʵ��(id=") + std::to_string(entityId) + ")";
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
	return entityComponentSignatures[entityId].test(componentId);
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