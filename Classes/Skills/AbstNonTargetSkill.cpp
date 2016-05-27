#include "AbstNonTargetSkill.h"
#include "GameWorld.h"
#include "Geometry\Geometry.h"

USING_NS_CC;

AbstNonTargetSkill::AbstNonTargetSkill(
	GameWorld* const		world,
	cocos2d::Vec2			target_pos,
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
		target_pos,
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

AbstNonTargetSkill::~AbstNonTargetSkill()
{}