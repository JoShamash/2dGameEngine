#include "ECS.h"

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

void System::AddEntityToSystem(const Entity& entity)
{
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(const Entity& entity)
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity& other) {
		return entity == other;
		}), entities.end());
}

const std::vector<Entity>& System::GetSystemEntities() const
{
	return entities;
}

const Signature& System::GetComponentSignature() const
{
	return componentSignature;
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
	glm::uint64 entityId = numEntities++;
	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdd.insert(entity);

	if (entityId >= componentSignatures.size())
	{
		componentSignatures.resize(entityId * 2);
	}

	std::string message = U8_TO_CHARPTR("����ʵ��(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
	return entity;
}

void Registry::DestroyEntity(const Entity& entity)
{
	entitiesToBeKill.insert(entity);

	std::string message = U8_TO_CHARPTR("����ʵ��(id=") + std::to_string(entity.GetId()) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}

void Registry::AddEntityToSystem(const Entity& entity)
{
	const auto& entityId = entity.GetId();
	const auto& entityComponentSignature = componentSignatures[entityId];
	
	for (const auto& [index, system] : systems)
	{
		const auto& systemComponentSignature = system->GetComponentSignature();

		// ��ʵ������ǩ������ϵͳ�����ǩ��ƥ��ʱ�����ʵ�嵽ϵͳ��
		if ((systemComponentSignature & entityComponentSignature) == systemComponentSignature)
		{
			system->AddEntityToSystem(entity);
		}
	}
}

void Registry::RemoveEntityFromSystem(const Entity& entity)
{
	for (const auto& [index, system] : systems)
	{
		system->RemoveEntityFromSystem(entity);
	}
}

void Registry::Update()
{
	for (auto& entity : entitiesToBeAdd)
	{
		AddEntityToSystem(entity);
	}
	entitiesToBeAdd.clear();

	for (auto& entity : entitiesToBeKill)
	{
		RemoveEntityFromSystem(entity);
	}
}

