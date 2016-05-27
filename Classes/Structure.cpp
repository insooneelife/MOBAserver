#include "Structure.h"
#include "Util.h"
#include "GameWorld.h"
#include "GameMap.h"

USING_NS_CC;

Structure::Structure(
	GameWorld* const		world,
	geometry::Shape* const	bounding_shape,
	const std::string&		name,
	const cocos2d::Vec2&	pos,
	double radius)
	: 
	BaseGameEntity(
		world,
		bounding_shape,
		STRUCTURE,
		name,
		radius,
		pos)
{}

Structure* Structure::create(
	GameWorld* const		world,
	geometry::Shape* const	bounding_shape,
	const std::string&		name,
	const cocos2d::Vec2&	pos,
	double radius)
{
	return new Structure(world, bounding_shape, name, pos, radius);
}

Structure::~Structure()
{}

//this is defined as a pure virtual function in BasegameEntity so it must be implemented
void Structure::update(double time_elapsed)
{}

void Structure::render()
{
}

bool Structure::handleMessage(const Telegram& msg)
{
	return false;
}