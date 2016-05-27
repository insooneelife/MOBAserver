#include <list>
#include <algorithm>
#include "AbstSkill.h"
#include "GameWorld.h"
#include "AbstCharacter.h"
#include "Geometry\Geometry.h"
#include "ActivateStrategy.h"
#include "ImpactStrategy.h"

USING_NS_CC;

AbstSkill::AbstSkill(
	GameWorld* const		world,
	cocos2d::Vec2			target_pos,
	int						shooter_id, 
	cocos2d::Vec2			origin,
	cocos2d::Vec2			heading,	
	int						damage,		
	float					radius,
	float					max_speed,
	float					mass,
	float					max_force,
	std::string				name,
	int						type,
	float					survive_time)
	:
	MovingEntity(
		world,
		new geometry::Circle(origin, radius),
		type,
		name,
		origin,
		radius,
		cocos2d::Vec2(),
		max_speed,
		heading,
		mass,
		0,
		max_force,
		true),
	_activate_strategy(nullptr),
	_impact_strategy(nullptr),
	_target_pos(target_pos),
	_dead(false),
	_shooter_id(shooter_id),
	_origin(origin),
	_survive_time(survive_time),
	_damage(damage),
	_activated(false)
{
	_time_of_creation = Clock.getCurrentTime();
}

AbstSkill::~AbstSkill()
{}


bool AbstSkill::isDead() const
{
	return _dead;
}

int AbstSkill::getShooterId() const
{
	return _shooter_id;
}

cocos2d::Vec2 AbstSkill::getTargetPos() const
{
	return _target_pos;
}

cocos2d::Vec2 AbstSkill::getOrigin() const
{
	return _origin;
}

float AbstSkill::getTimeOfCreation() const 
{
	return _time_of_creation;
}

float AbstSkill::getSurviveTime() const
{
	return _survive_time;
}

int AbstSkill::getDamage() const
{
	return _damage;
}

void AbstSkill::setDamage(int damage)
{
	_damage = damage;
}

void AbstSkill::setDead()
{
	_dead = true;
}

bool AbstSkill::getActivated() const
{
	return _activated;
}

void AbstSkill::setActivated(bool activated)
{
	_activated = activated;
}