#include "AbstTargetSkill.h"
#include "GameWorld.h"
#include "AbstCharacter.h"
#include "Geometry\Geometry.h"

USING_NS_CC;

AbstTargetSkill::AbstTargetSkill(
	GameWorld* const		world,
	AbstCharacter* const	target,
	int						shooter_id,
	cocos2d::Vec2			origin,
	cocos2d::Vec2			heading,
	int						damage,
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
		target->getPos(),
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
		survive_time),
	_target(target)
{}

AbstCharacter* const AbstTargetSkill::getTarget() const
{
	return _target;
}