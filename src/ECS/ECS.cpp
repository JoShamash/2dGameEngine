#include "ECS.h"

void Entity::AddTag(const std::string& tag) const
{
	registry->EntityAddTag(*this, tag);
}

void Entity::ChangeTag(const std::string& tag) const
{
	registry->EntityChangeTag(*this, tag);
}

void Entity::RemoveTag() const
{
	registry->EntityRemoveTag(*this);
}

bool Entity::HasTag(const std::string& tag) const
{
	return registry->EntityHasTag(*this, tag);
}

const std::string Entity::GetTag() const
{
	return registry->GetTagByEntity(*this);
}

void Entity::AddGroup(const std::string& group) const
{
	registry->EntityAddGroup(*this, group);
}

void Entity::ChangeGroup(const std::string& group) const
{
	registry->EntityChangeGroup(*this, group);
}

void Entity::RemoveGroup() const
{
	registry->EntityRemoveGroup(*this);
}

bool Entity::HasGroup(const std::string& group) const
{
	return registry->EntityHasGroup(*this, group);
}

const std::string Entity::GetGroup() const
{
	return registry->GetGroupByEntity(*this);
}

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
	componentPools.reserve(capacity);
	componentSignatures.reserve(capacity);
}

Entity Registry::CreateEntity()
{
	// ����ʵ��ID
	glm::uint64 entityId;
	if (freeIds.empty())
	{
		entityId = numEntities;
	}
	else
	{
		entityId = freeIds.front();
		freeIds.pop_front();
	}

	// ��ʼ����ʵ������ǩ��
	entityIdToSignaturesIdxMap[entityId] = numEntities;
	signaturesIdxToEntityIdMap[numEntities] = entityId;
	componentSignatures.emplace_back();
	numEntities++;

	Entity entity(entityId);		// ����ʵ����󣬴���ʵ��ID��ע���ָ��
	entities.insert(entity);		// ��ʵ����ӵ�ʵ�弯����	
	entitiesToBeAdd.insert(entity); // ��ʵ����ӵ�����Ӽ�����

	std::string message = U8_TO_CHARPTR("����ʵ��(id=") + std::to_string(entityId) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);

	return entity;
}

void Registry::DestroyEntity(const Entity& entity)
{
	const auto& entityId = entity.GetId();
	const auto& signatureIdx = entityIdToSignaturesIdxMap[entityId];

	entities.erase(entity);				// ��ʵ�弯�����Ƴ�ʵ��
	entitiesToBeKill.insert(entity);	// ��ʵ����ӵ������ټ�����
	entity.RemoveTag();					// ����󶨵�tag��ǩ
	entity.RemoveGroup();				// ����󶨵�group����

	std::string message = U8_TO_CHARPTR("����ʵ��(id=") + std::to_string(entity.GetId()) + ")";
	Logger::Instance().Log(LogLevel::INFO, message);
}

void Registry::AddEntityToSystems(const Entity& entity)
{
	const auto& entityId = entity.GetId();

	const auto& signatureIdx = entityIdToSignaturesIdxMap[entityId];
	const auto& entityComponentSignature = componentSignatures[signatureIdx];

	// ���ʵ�嵽����������ϵͳ
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
	// ���ʵ�嵽ϵͳ
	for (auto& entity : entitiesToBeAdd)
	{
		AddEntityToSystems(entity);
	}
	entitiesToBeAdd.clear();

	// 
	for (auto& entity : entitiesToBeKill)
	{
		RemoveEntityFromSystems(entity);

		const auto& entityId = entity.GetId();
		const auto& signatureIdx = entityIdToSignaturesIdxMap[entityId];

		// ע�⣺�ͷ����������������ϵͳ����֮ǰ��֮��
		// �ͷ�ʵ��ID���������
		const auto& signature = componentSignatures[signatureIdx];
		for (auto& componentPool : componentPools)
		{
			componentPool->Remove(entityId);
		}

		// ɾ����ʵ�����ǩ��
		const auto& lastEntityId = signaturesIdxToEntityIdMap[--numEntities];
		entityIdToSignaturesIdxMap[lastEntityId] = signatureIdx;
		signaturesIdxToEntityIdMap[signatureIdx] = lastEntityId;
		entityIdToSignaturesIdxMap.erase(entityId);
		signaturesIdxToEntityIdMap.erase(numEntities);
		componentSignatures[signatureIdx] = componentSignatures.back();
		componentSignatures.pop_back();

		// �ͷ�ʵ��ID�Թ�����ʹ��
		freeIds.push_back(entityId);
	}
	entitiesToBeKill.clear();
}

bool Registry::HasTag(const std::string& input) const
{
	auto tag = to_lower(input);
	if (tagEntityMap.find(tag) == tagEntityMap.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Registry::HasGroup(const std::string& input) const
{
	auto group = to_lower(input);
	if (groupEntitiesMap.find(group) == groupEntitiesMap.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Registry::EntityAddTag(const Entity& entity, const std::string& input)
{
	if (input.empty())
	{
		return;
	}

	auto tag = to_lower(input);
	auto id = entity.GetId();
	if (tagEntityMap.find(tag) != tagEntityMap.end())
	{
		std::string message = U8_TO_CHARPTR("ʵ��") + std::to_string(id) + U8_TO_CHARPTR("��ӱ�ǩ") + tag + U8_TO_CHARPTR("ʧ�ܣ��Ѵ��ڸñ�ǩ");
		Logger::Instance().Log(LogLevel::ERROR, message);
		return;
	}

	tagEntityMap.emplace(tag, entity);
	entityTagMap.emplace(id, tag);
}

void Registry::EntityChangeTag(const Entity& entity, const std::string& input)
{
	auto tag = to_lower(input);
	auto id = entity.GetId();
	if (entityTagMap.find(id) == entityTagMap.end())
	{
		std::string message = U8_TO_CHARPTR("ʵ��") + std::to_string(id) + U8_TO_CHARPTR("������ǩʧ�ܣ������ڱ�ǩ");
		Logger::Instance().Log(LogLevel::ERROR, message);
		return;
	}

	auto& exTag = entityTagMap[id];
	tagEntityMap.erase(exTag);
	entityTagMap.erase(id);
	tagEntityMap.emplace(tag, entity);
	entityTagMap.emplace(id, tag);
}

void Registry::EntityRemoveTag(const Entity& entity)
{
	auto id = entity.GetId();
	if (entityTagMap.find(id) != entityTagMap.end())
	{
		auto& tag = entityTagMap[id];
		tagEntityMap.erase(tag);
		entityTagMap.erase(id);
	}
}

bool Registry::EntityHasTag(const Entity& entity, const std::string& input) const
{
	auto tag = to_lower(input);
	auto id = entity.GetId();
	if (entityTagMap.find(id) == entityTagMap.end() || entityTagMap.at(id) != tag)
	{
		return false;
	}
	else
	{
		return true;
	}
}

const std::string Registry::GetTagByEntity(const Entity& entity) const
{
	auto id = entity.GetId();
	if (entityTagMap.find(id) != entityTagMap.end())
	{
		return entityTagMap.at(id);
	}
	else
	{
		return "";
	}
}

const Entity& Registry::GetEntityByTag(const std::string& input) const
{
	auto tag = to_lower(input);
	return tagEntityMap.at(tag);
}

void Registry::EntityAddGroup(const Entity& entity, const std::string& input)
{
	if (input.empty())
	{
		return;
	}

	auto group = to_lower(input);
	auto id = entity.GetId();
	if (entityGroupMap.find(id) != entityGroupMap.end())
	{
		std::string message = U8_TO_CHARPTR("ʵ��") + std::to_string(id) + U8_TO_CHARPTR("�������") + group + U8_TO_CHARPTR("ʧ�ܣ��Ѵ��ڸ�����");
		Logger::Instance().Log(LogLevel::ERROR, message);
		return;
	}

	entityGroupMap[id] = group;
	groupEntitiesMap[group].insert(entity);
}

void Registry::EntityChangeGroup(const Entity& entity, const std::string& input)
{
	auto group = to_lower(input);
	auto id = entity.GetId();
	if (entityGroupMap.find(id) == entityGroupMap.end())
	{
		std::string message = U8_TO_CHARPTR("ʵ��") + std::to_string(id) + U8_TO_CHARPTR("��������ʧ�ܣ�����������");
		Logger::Instance().Log(LogLevel::ERROR, message);
		return;
	}

	auto& exGroup = entityGroupMap[id];
	groupEntitiesMap[exGroup].erase(entity);
	entityGroupMap[id] = group;
	groupEntitiesMap[group].insert(entity);
}

void Registry::EntityRemoveGroup(const Entity& entity)
{
	auto id = entity.GetId();
	if (entityGroupMap.find(id) != entityGroupMap.end())
	{
		auto& exGroup = entityGroupMap[id];
		groupEntitiesMap[exGroup].erase(entity);
		entityGroupMap.erase(id);
	}
}

bool Registry::EntityHasGroup(const Entity& entity, const std::string& input) const
{
	auto group = to_lower(input);
	auto id = entity.GetId();
	if (entityGroupMap.find(id) == entityGroupMap.end() || entityGroupMap.at(id) != group)
	{
		return false;
	}
	else
	{
		return true;
	}
}

const std::string Registry::GetGroupByEntity(const Entity& entity) const
{
	auto id = entity.GetId();
	if (entityGroupMap.find(id) != entityGroupMap.end())
	{
		return entityGroupMap.at(id);
	}
	else
	{
		return "";
	}
}

const std::set<Entity>& Registry::GetEntitiesByGroup(const std::string& input) const
{
	auto group = to_lower(input);
	return groupEntitiesMap.at(group);
}

std::string Registry::to_lower(std::string str)
{
	std::ranges::transform(str, str.begin(), [](unsigned char c) {
		return std::tolower(c);
		});
	return str;
}

std::string Registry::to_upper(std::string str)
{
	std::ranges::transform(str, str.begin(), [](unsigned char c) {
		return std::toupper(c);
		});
	return str;
}

bool Registry::is_same_word(const std::string& str1, const std::string& str2)
{
	return to_lower(str1) == to_lower(str2);
}
