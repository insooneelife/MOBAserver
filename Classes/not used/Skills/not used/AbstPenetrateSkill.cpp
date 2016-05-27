#include <list>
#include <algorithm>
#include "AbstPenetrateSkill.h"
#include "GameWorld.h"
#include "AbstCharacter.h"
#include "Geometry\Geometry.h"
#include "CollisionTestTemplates.h"

USING_NS_CC;

AbstPenetrateSkill::AbstPenetrateSkill(
	GameWorld* const		world,		//a pointer to the world data
	cocos2d::Layer* const	view_layer,
	cocos2d::Vec2			target,		//the target's position
	int						shooter_id, //the ID of the bot that fired this shot
	cocos2d::Vec2			origin,		//the start position of the projectile
	cocos2d::Vec2			heading,	//the heading of the projectile
	int						damage,		//how much damage it inflicts
	double					radius,
	double					max_speed,
	double					mass,
	double					max_force,
	std::string				name,
	int						type,
	double					survive_time)
	:
	AbstSkill(
		world,
		view_layer,
		target,
		shooter_id,
		origin,
		heading,
		damage,
		radius,
		max_speed,
		mass,
		max_force,
		name,
		type,
		survive_time)
{}

AbstPenetrateSkill::~AbstPenetrateSkill()
{}
