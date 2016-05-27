#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   EntityManager.h
//
//  Desc:   Singleton class to handle the  management of Entities.          
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <map>
#include <memory>
#include "Singleton.h"

class BaseGameEntity;

//provide easy access
#define EntityMgr EntityManager::getInstance()

class EntityManager : public Singleton<EntityManager> {
public:
	typedef std::map<unsigned int, BaseGameEntity* const> EntityMap;

	//returns a pointer to the entity with the ID given as a parameter
	BaseGameEntity* const getEntityFromID(unsigned int id) const;

	//this method stores a pointer to the entity in the std::vector
	//m_Entities at the index position indicated by the entity's ID
	//(makes for faster access)
	void				registerEntity(BaseGameEntity* const new_entity);

	//this method removes the entity from the list
	void				removeEntity(BaseGameEntity* const p_entity);

	//clears all entities from the entity map
	void				reset();

	const EntityMap&	getAllEntity() const;

	bool				exists(unsigned int id) const;

private:
	EntityManager(const EntityManager&) = delete; // no copies
	EntityManager& operator=(const EntityManager&) = delete; // no self-assignments
	EntityManager() {}
	friend class Singleton<EntityManager>;

	EntityMap _entity_map;
};



#endif