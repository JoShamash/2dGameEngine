#include "ECS.h"

void Entity::KillSelf() const
{
	registry->DestroyEntity(*this);
}

bool Entity::operator==(const Entity& other) const
{
	return id == other.id;
}

bool Entity::operator!=(const Entity& other) const
{
	return id != other.id;
}

bool Entity::operator>(const Entity& other) const
{
	return id > other.id;
}

bool Entity::operator<(const Entity & other) const
{
	return id < other.id;
}

/*-------------------------------------------------------------------------------------------------*/

Registry::Registry(glm::uint64 capacity)
{
	if (capacity < 16)
	{
		capacity = 16;
	}
	componentPools.resize(capacity);
	componentSignatures.resize(capacity);
}

Entity Registry::CreateEntity()
{
	glm::uint64 entityId;
	if (freeIds.empty())
	{
		entityId = numEntities++;

		if (entityId >= componentSignatures.size())
		{
			componentSignatures.resize(entityId * 2);
		}
	}
	else
	{
		entityId = freeIds.front();
		freeIds.pop_front();
	}

	Entity entity(entityId, this);  // ����ʵ����󣬴���ʵ��ID��ע���ָ��
	entities.insert(entity);		// ��ʵ����ӵ�ʵ�弯����	
	entitiesToBeAdd.insert(entity); // ��ʵ����ӵ�����Ӽ�����

	std::string message = U8_TO_CHARPTR("����ʵ��(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
	return entity;
}

void Registry::DestroyEntity(const Entity& entity)
{
	entities.erase(entity);			 // ��ʵ�弯�����Ƴ�ʵ��
	entitiesToBeKill.insert(entity); // ��ʵ����ӵ������ټ�����

	std::string message = U8_TO_CHARPTR("����ʵ��(id=") + std::to_string(entity.GetId()) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}

void Registry::AddEntityToSystems(const Entity& entity)
{
	const auto& entityId = entity.GetId();
	const auto& entityComponentSignature = componentSignatures[entityId];

	// ���ʵ�嵽��Ӧϵͳ
	for (const auto& [index, system] : systems)
	{
		const auto& systemComponentSignature = system->GetComponentSignature();

		// ��ʵ������ǩ������ϵͳ�����ǩ��ƥ��ʱ�����ʵ�嵽ϵͳ��
		if ((systemComponentSignature & entityComponentSignature) == systemComponentSignature)
		{
			system->AddEntity(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(const Entity& entity)
{
	// ��ϵͳ���Ƴ�ʵ��
	for (const auto& [index, system] : systems)
	{
		system->RemoveEntity(entity);
	}
}

void Registry::Update()
{
	for (auto& entity : entitiesToBeAdd)
	{
		AddEntityToSystems(entity);
	}
	entitiesToBeAdd.clear();

	for (auto& entity : entitiesToBeKill)
	{
		RemoveEntityFromSystems(entity);

		const auto& id = entity.GetId();
		componentSignatures[id].reset(); // ��ʵ�����ǩ������
		freeIds.push_back(id);			 // �ͷ�ʵ��ID�Թ�����ʹ��
	}
	entitiesToBeKill.clear();
}
