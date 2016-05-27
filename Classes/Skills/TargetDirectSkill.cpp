#include "TargetDirectSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "GlobalValues.h"
#include "Message/MessageDispatcher.h"

#include "ImpactTarget.h"
#include "ActivateDirect.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
TargetDirectSkill::TargetDirectSkill(
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
	//setDamage(shooter->getDamage());
	setTeam(team);
	movePos(_target->getPos() - _pos);

	_activate_strategy.reset(new ActivateDirect(this));
	_impact_strategy.reset(new ImpactTarget(this, splash_radius, shooter, target));
}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void TargetDirectSkill::update(double time_elapsed)
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

void TargetDirectSkill::updateMovement(double time_elapsed)
{
	
	/*_velocity = getMaxSpeed() * (_target->getPos() - _pos).getNormalized();

	//make sure vehicle does not exceed maximum velocity
	if (_velocity.getLength() > _max_speed)
	{
		_velocity.normalize();
		_velocity *= _max_speed;
	}
	*/
	//setPos(_target->getPos());
	//_velocity = _pos - _target->getPos();
	movePos( _target->getPos() - _pos );
}

//-------------------------- render -------------------------------------------
//-----------------------------------------------------------------------------
void TargetDirectSkill::render()
{}
