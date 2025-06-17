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

	Entity entity(entityId, this);  // 创建实体对象，传入实体ID和注册表指针
	entities.insert(entity);		// 将实体添加到实体集合中	
	entitiesToBeAdd.insert(entity); // 将实体添加到待添加集合中

	std::string message = U8_TO_CHARPTR("创建实体(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
	return entity;
}

void Registry::DestroyEntity(const Entity& entity)
{
	entities.erase(entity);			 // 从实体集合中移除实体
	entitiesToBeKill.insert(entity); // 将实体添加到待销毁集合中

	std::string message = U8_TO_CHARPTR("销毁实体(id=") + std::to_string(entity.GetId()) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}

void Registry::AddEntityToSystems(const Entity& entity)
{
	const auto& entityId = entity.GetId();
	const auto& entityComponentSignature = componentSignatures[entityId];

	// 添加实体到对应系统
	for (const auto& [index, system] : systems)
	{
		const auto& systemComponentSignature = system->GetComponentSignature();

		// 当实体的组件签名包含系统的组件签名匹配时，添加实体到系统。
		if ((systemComponentSignature & entityComponentSignature) == systemComponentSignature)
		{
			system->AddEntity(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(const Entity& entity)
{
	// 从系统中移除实体
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
		componentSignatures[id].reset(); // 该实体组件签名重置
		freeIds.push_back(id);			 // 释放实体ID以供后续使用
	}
	entitiesToBeKill.clear();
}
