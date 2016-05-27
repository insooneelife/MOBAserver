#ifndef GAME_ENTITY_FUNCTION_TEMPLATES
#define GAME_ENTITY_FUNCTION_TEMPLATES

#include <tuple>
#include "BaseGameEntity.h"
#include "cocos2d.h"

//////////////////////////////////////////////////////////////////////////
//
//  Some useful template functions
//
//////////////////////////////////////////////////////////////////////////

//------------------------- Overlapped -----------------------------------
//
//  tests to see if an entity is overlapping any of a number of entities
//  stored in a std container
//------------------------------------------------------------------------
template <class Entity, class Container>
bool overlapped(const Entity* e, const Container& con, double min_dist_between_obstacles = 40.0)
{
	typename Container::const_iterator it;

	for (it = con.begin(); it != con.end(); ++it)
	{
		if (TwoCirclesOverlapped(e->getPos(),
			e->getBRadius() + min_dist_between_obstacles,
			(*it)->getPos(),
			(*it)->getBRadius()))
		{
			return true;
		}
	}

	return false;
}

//----------------------- TagNeighbors ----------------------------------
//
//  tags any entities contained in a std container that are within the
//  radius of the single entity parameter
//------------------------------------------------------------------------
template <class Entity, class Container>
void tagNeighbors(Entity entity, Container& others, const double radius)
{
	typename Container::iterator it;

	//iterate through all entities checking for range
	for (it = others.begin(); it != others.end(); ++it) {
		//first clear any current tag
		(*it)->unTag();

		//work in distance squared to avoid sqrts
		cocos2d::Vec2 to = (*it)->getPos() - entity->getPos();

		//the bounding radius of the other is taken into account by adding it 
		//to the range
		double range = radius + (*it)->getBRadius();

		//if entity within range, tag for further consideration
		if (((*it)->getId() != entity->getId()) && (to.getLengthSq() < range*range))
		{
			(*it)->tag();
		}
	}//next entity
}


//------------------- enforceNonPenetration ---------------------
//
//  Given a pointer to an entity and a std container of pointers to nearby
//  entities, this function checks to see if there is an overlap between
//  entities. If there is, then the entities are moved away from each
//  other
//------------------------------------------------------------------------
template <class Entity, class Container>
void enforceNonPenetration(Entity entity, const Container& others)
{
	typename Container::const_iterator it;

	//iterate through all entities checking for any overlap of bounding
	//radii
	for (it = others.begin(); it != others.end(); ++it)
	{
		//make sure we don't check against this entity
		if ((*it)->getId() == entity->getId()) continue;

		//calculate the distance between the positions of the entities
		cocos2d::Vec2 to_entity = entity->getPos() - (*it)->getPos();

		float dist_from_each_other = to_entity.getLength();

		//if this distance is smaller than the sum of their radii then this
		//entity must be moved away in the direction parallel to the
		//ToEntity vector   
		float amount_of_overlap = (*it)->getBRadius() + entity->getBRadius() -
			dist_from_each_other;

		if (amount_of_overlap >= 0) 
		{
			//move the entity a distance away equivalent to the amount of overlap.
			entity->movePos(
				entity->getShape()->enforceFrom((*it)->getShape().get()));
		}
	}//next entity
}


//---------------------- enforceNonPenetrationCellSpace ------------------------
//
//  Cell space version for enforceNonPenetration.
//------------------------------------------------------------------------------
template <class Entity, class CellSpace>
void enforceNonPenetrationCellSpace(Entity entity, const CellSpace& others)
{
	others->calculateNeighborsForSmall(entity->getPos(), entity->getShape().get());

	for (auto iter = others->begin(); !others->end(); iter = others->next())
	{
		if (iter->getId() == entity->getId()) continue;

		//calculate the distance between the positions of the entities
		cocos2d::Vec2 to_entity = entity->getPos() - iter->getPos();

		float dist_from_each_other = to_entity.getLength();

		//if this distance is smaller than the sum of their radii then this
		//entity must be moved away in the direction parallel to the
		//ToEntity vector   
		float amount_of_overlap = iter->getBRadius() + entity->getBRadius() -
			dist_from_each_other;

		if (amount_of_overlap >= 0)
		{
			//move the entity a distance away equivalent to the amount of overlap.
			entity->movePos(entity->getShape()->enforceFrom(iter->getShape().get()));
		}
	}
}


template <typename Container>
void loadEntities(const std::string& filename, cocos2d::Layer* const veiw, Container& entities)
{
	rapidjson::Document json;
	util::readJSON(filename, json);

	for (auto iter = json.Begin(); iter != json.End(); iter++)
	{
		entities.emplace_back(new Entity(veiw, (*iter)));
	}
}

template <typename Container>
void saveEntities(const std::string& filename, const Container& entities)
{
	std::ofstream out(filename);

	if (!out)
	{
		CCASSERT(false, ("Cannot open file: " + std::string(filename)).c_str());
		return;
	}

	std::stringstream ss;
	std::string json = "";

	json += "[\n";
	for (auto iter = std::begin(entities); iter != std::end(entities); iter++)
	{
		ss << *(*iter) << "," << std::endl;
	}
	json += ss.str();
	json.pop_back();
	json.pop_back();
	json += "\n]";

	out << json;
}








#endif