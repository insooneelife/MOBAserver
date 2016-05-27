#include "CollisionArea.h"
#include "Util.h"
#include "GameWorld.h"
#include "GameMap.h"

USING_NS_CC;

CollisionArea::CollisionArea(
	GameWorld* const		world,
	geometry::Shape* const	bounding_shape,
	const cocos2d::Vec2&	pos,
	double radius)
	: 
	BaseGameEntity(world, bounding_shape, STRUCTURE, "", radius, pos)
{}

CollisionArea* CollisionArea::create(
	GameWorld* const		world,
	geometry::Shape* const	bounding_shape,
	const cocos2d::Vec2&	pos,
	double radius)
{
	return new CollisionArea(world, bounding_shape, pos, radius);
}

CollisionArea::~CollisionArea()
{}
