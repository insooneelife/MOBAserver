#include "TargetSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "GlobalValues.h"
#include "Message/MessageDispatcher.h"

#include "ImpactTarget.h"
#include "ActivateCollisionToTarget.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
TargetSkill::TargetSkill(
	AbstCharacter* const shooter,
	AbstCharacter* const target,
	float splash_radius,
	int damage,
	int radius,
	int max_speed,
	double survive_time,
	const std::string& name,
	int team)
	:
	AbstTargetSkill(
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
		survive_time),
	_activated(false)
{
	CCASSERT(target, "target == nullptr");
	
	setTeam(team);
	movePos(_heading * _bounding_radius);

	_activate_strategy.reset(new ActivateCollisionToTarget(this));
	_impact_strategy.reset(new ImpactTarget(this, splash_radius, shooter, target));
}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void TargetSkill::update(double time_elapsed)
{
	if (!_target->isAlive())
	{
		setDead();
		return;
	}

	if (_activated)
	{
		_impact_strategy->impact();
		setDead();
	}
	else
	{
		_activated = _activate_strategy->test();

		updateMovement(time_elapsed);
	}
}

void TargetSkill::updateMovement(double time_elapsed)
{
	_velocity = getMaxSpeed() * (_target->getPos() - _pos).getNormalized();

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
void TargetSkill::render()
{}
