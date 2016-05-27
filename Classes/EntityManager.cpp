#include "EntityManager.h"
#include "BaseGameEntity.h"

BaseGameEntity* const EntityManager::getEntityFromID(unsigned int id) const
{
	//find the entity
	EntityMap::const_iterator ent = _entity_map.find(id);

	//return nullptr if there is no entity in the map
	//this entity is the case that has already dead and removed from world
	if (ent == std::end(_entity_map))
		return nullptr;

	return ent->second;
}

void EntityManager::removeEntity(BaseGameEntity* const p_entity)
{
	auto iter = _entity_map.find(p_entity->getId());
	if (iter != std::end(_entity_map))
		_entity_map.erase(iter);
} 

void EntityManager::reset() 
{
	_entity_map.clear(); 
}

void EntityManager::registerEntity(BaseGameEntity* const new_entity) 
{
	_entity_map.insert(std::make_pair(new_entity->getId(), new_entity));
}

const EntityManager::EntityMap& EntityManager::getAllEntity() const
{
	return _entity_map;
}

bool EntityManager::exists(unsigned int id) const
{
	auto iter = _entity_map.find(id);
	if (iter != std::end(_entity_map))
	{
		return true;
	}
	return false;
}


