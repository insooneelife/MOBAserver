#include "PenetrateShootingSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "GlobalValues.h"
#include "Message/MessageDispatcher.h"

#include "ImpactNonRepeatSplash.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
PenetrateShootingSkill::PenetrateShootingSkill(
	AbstCharacter* const shooter,
	Vec2 target_pos,
	int damage,
	int radius,
	int max_speed,
	double survive_time,
	const std::string& name,
	int team)
	:
	AbstNonTargetSkill(
		shooter->getWorld(),
		target_pos,
		shooter->getId(),
		shooter->getPos(),
		shooter->getHeading(),
		damage,
		radius,
		max_speed,
		2,
		10,
		name,
		SKILL,
		survive_time)
{
	CCASSERT(target_pos != Vec2(), "target_pos != Vec2()");

	setTeam(team);
	movePos(_heading * _bounding_radius * 2);

	_impact_strategy.reset(new ImpactNonRepeatSplash(this, shooter, _bounding_radius));
}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void PenetrateShootingSkill::update(double time_elapsed)
{
	if (Clock.getCurrentTime() > _survive_time + _time_of_creation)
	{
		_dead = true;
	}

	updateMovement(time_elapsed);

	_impact_strategy->impact();
}

void PenetrateShootingSkill::updateMovement(double time_elapsed)
{
	_velocity = getMaxSpeed() * getHeading();

	//make sure vehicle does not exceed maximum velocity
	if (_velocity.getLength() > _max_speed)
	{
		_velocity.normalize();
		_velocity *= _max_speed;
	}

	movePos(_velocity);
}


//-------------------------- render -------------------------------------------
//-----------------------------------------------------------------------------
void PenetrateShootingSkill::render()
{}
