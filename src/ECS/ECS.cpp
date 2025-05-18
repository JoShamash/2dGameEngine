#include "ECS.h"

glm::uint64 Entity::GetId() const 
{
	return id;
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

Registry::Registry(glm::uint64 capacity)
{
	if (capacity < 128)
	{
		capacity = 128;
	}
	componentPools.resize(capacity);
	entityComponentSignatures.resize(capacity);
}

Entity& Registry::CreateEntity()
{
	glm::uint64 entityId = numEntities++;
	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdd.insert(entity);

	if (entityId >= entityComponentSignatures.size())
	{
		entityComponentSignatures.resize(entityId * 2);
	}

	std::string message = reinterpret_cast<const char*>(u8"创建实体(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
	return entity;
}

void Registry::DestroyEntity(const Entity& entity)
{
	entitiesToBeKill.insert(entity);

	std::string message = reinterpret_cast<const char*>(u8"销毁实体(id=") + std::to_string(entity.GetId()) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}

void Registry::AddEntityToSystem(const Entity& entity)
{
	const auto& entityId = entity.GetId();
	const auto& entityComponentSignature = entityComponentSignatures[entityId];
	
	for (const auto& [index, system] : systems)
	{
		const auto& systemComponentSignature = system->GetComponentSignature();

		// 当实体的组件签名包含系统的组件签名匹配时，添加实体到系统。
		if ((systemComponentSignature & entityComponentSignature) == systemComponentSignature)
		{
			system->AddEntityToSystem(entity);
		}
	}
}

void Registry::Update()
{
	for (auto& entity : entitiesToBeAdd)
	{
		AddEntityToSystem(entity);
	}
	entitiesToBeAdd.clear();

	for (auto entity : entitiesToBeKill)
	{
		for (const auto& [index, system] : systems)
		{
			system->RemoveEntityFromSystem(entity);
		}
	}
}

