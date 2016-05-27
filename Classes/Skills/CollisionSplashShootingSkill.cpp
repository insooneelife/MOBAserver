#include "CollisionSplashShootingSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "Message/MessageDispatcher.h"
#include "GlobalValues.h"
#include "ParameterLoader.h"

#include "ActivateCollisionToAny.h"
#include "ImpactSplash.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
CollisionSplashShootingSkill::CollisionSplashShootingSkill(
	AbstCharacter* const shooter,
	Vec2 target,
	float splash_radius,
	int damage,
	int radius,
	int max_speed,
	double survive_time,
	const std::string& name,
	int team)
	:
	AbstNonTargetSkill(
		shooter->getWorld(),
		target,
		shooter->getId(),
		shooter->getPos(),
		shooter->getHeading(),
		damage,
		radius,
		max_speed,
		1,
		10,
		name,
		SKILL,
		survive_time)
{
	CCASSERT(target != Vec2(), "target != Vec2()");

	setTeam(team);
	movePos(_heading * _bounding_radius);

	_activate_strategy.reset(new ActivateCollisionToAny(this));
	_impact_strategy.reset(new ImpactSplash(this, shooter, splash_radius));

	
}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void CollisionSplashShootingSkill::update(double time_elapsed)
{
	if (Clock.getCurrentTime() > _survive_time + _time_of_creation)
	{
		_dead = true;
	}

	if (_activated)
	{
		_impact_strategy->impact();
	}
	else
	{
		_activated = _activate_strategy->test();

		updateMovement(time_elapsed);
	}
}

void CollisionSplashShootingSkill::updateMovement(double time_elapsed)
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
void CollisionSplashShootingSkill::render()
{}


