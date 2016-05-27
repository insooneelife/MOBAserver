//-----------------------------------------------------------------------------
//
//  Name:   CollisionTestTemplates.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   a few functions for testing entity bounding shapes 
//          against containers of collision areas.
//
//-----------------------------------------------------------------------------

#include <list>
#include "CollisionArea.h"
#include "Geometry\Geometry.h"


//------------------ closestIntersectPointWithCollisionAreas ------------------
//
//  tests a line segment against the container of structures  to calculate
//  the closest intersection point, which is stored in the reference 'intersect_point'.
//  The distance to the point is assigned to the reference 'min_dis'
//
//  returns false if no intersection point found
//-----------------------------------------------------------------------------

template <class CollisionAreas>
inline bool closestIntersectPointWithCollisionAreas(
	cocos2d::Vec2			source,
	cocos2d::Vec2			destination,
	double&					min_dis,
	cocos2d::Vec2&			intersect_point,
	const CollisionAreas&	collision_areas)
{

	min_dis = std::numeric_limits<double>::max();
	intersect_point = destination;

	for (auto iter = std::begin(collision_areas);
	iter != std::end(collision_areas); iter++)
	{
		Vec2 temp;
		std::unique_ptr<geometry::Segment> seg(new geometry::Segment(source, destination));
		std::vector<cocos2d::Vec2> points =
			(seg.get())->intersectPoints((*iter)->getShape().get());

		if ( points.size() > 0)
		{
			for (auto e : points)
			{
				float distance = util::dis2(source.x, source.y, e.x, e.y);
				if (min_dis > distance)
				{
					min_dis = distance;
					intersect_point = e;
				}
			}
		}
	}

	if (min_dis < std::numeric_limits<double>::max()) return true;

	return false;
}


//---------------------------- collisionToOthers ------------------------------
//
//  given a shape, iterate through all
//  the container objects and test for any intersection
//  This method returns true if an overlap occurs.
//-----------------------------------------------------------------------------
template <class Container>
inline bool collisionToOthers(
	const geometry::Shape* const shape,
	const Container& others)
{
	for (auto iter = std::begin(others); iter != std::end(others); ++iter)
	{
		if (!(*iter)->isAlive())
			continue;

		if ((*iter)->getShape()->overlap(shape))
		{
			return true;
		}
	}

	return false;
}


//----------------------- collisionToOthersCellSpace --------------------------
//
//  given an entity, iterate through all the cellspace entities
//  and test for any intersection. 
//  This method returns true if an overlap occurs.
//-----------------------------------------------------------------------------
template <typename Entity, typename CellSpace>
inline bool collisionToOthersCellSpace(
	const Entity& entity,
	const CellSpace& others)
{
	others->calculateNeighborsForSmall(entity->getPos(), entity->getShape().get());

	for (auto iter = others->begin(); !others->end(); iter = others->next())
	{
		if (!iter->isAlive() || iter->getTeam() == entity->getTeam())
			continue;

		if (iter->getShape()->overlap(entity->getShape().get()))
		{
			return true;
		}
	}

	return false;
}


//----------------------- collisionToOthersCellSpace --------------------------
//
//  version for check collision with selective entities. 
//-----------------------------------------------------------------------------
template <typename Entity, typename CellSpace>
inline bool collisionToOthersCellSpace(
	const Entity& entity,
	const CellSpace& others,
	unsigned int filter_id)
{
	others->calculateNeighborsForSmall(entity->getPos(), entity->getShape().get());

	for (auto iter = others->begin(); !others->end(); iter = others->next())
	{
		if (filter_id == iter->getId() || !iter->isAlive() || iter->getTeam() == entity->getTeam()) 
			continue;

		if (iter->getShape()->overlap(entity->getShape().get()))
		{
			return true;
		}
	}

	return false;
}


//----------------------- getClosestCollisionEntity ---------------------------
//
//  given an entity, iterate through all the cellspace entities
//  and test for any overlap.
//  This method returns the closest one in the cell space to entity.
//-----------------------------------------------------------------------------
template <typename Entity, typename CellSpace>
typename CellSpace::entity_type getClosestCollisionEntity(
	const Entity& entity,
	CellSpace* others)
{
	typename CellSpace::entity_type closest_entity = nullptr;
	float closest_so_far = std::numeric_limits<float>::max();
	others->calculateNeighborsForSmall(entity->getPos(), entity->getShape().get());

	for (auto iter = others->begin(); !others->end(); iter = others->next())
	{
		if (iter->getId() == entity->getId() || !iter->isAlive() || iter->getTeam() == entity->getTeam())
			continue;

		if (iter->getShape()->overlap(entity->getShape().get()))
		{
			float dist = (iter->getPos() - entity->getPos()).getLengthSq();

			if (dist < closest_so_far)
			{
				dist = closest_so_far;
				closest_entity = iter;
			}
		}
	}

	return closest_entity;
}

//------------------- getListOfCollisionEntitiesCellSpace ---------------------
//
//  given an entity, iterate through all the cellspace entities
//  and test for any intersection.
//  This method returns the list of overlapping entities.
//-----------------------------------------------------------------------------
template <typename Entity, typename CellSpace>
inline std::list<typename CellSpace::entity_type> getListOfCollisionEntitiesCellSpace(
	const Entity& entity,
	CellSpace* others)
{
	std::list<typename CellSpace::entity_type> hits;
	others->calculateNeighborsForSmall(entity->getPos(), entity->getShape().get());

	for (auto iter = others->begin(); !others->end(); iter = others->next())
	{
		if (iter->getId() == entity->getId() || !iter->isAlive() || iter->getTeam() == entity->getTeam())
			continue;

		if (iter->getShape()->overlap(entity->getShape().get()))
		{
			hits.push_back(iter);
		}
	}

	return hits;
}