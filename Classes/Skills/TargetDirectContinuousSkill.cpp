#include "TargetDirectContinuousSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "GlobalValues.h"
#include "Message/MessageDispatcher.h"

#include "ImpactContinuousTarget.h"
#include "ActivateContinuous.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
TargetDirectContinuousSkill::TargetDirectContinuousSkill(
	AbstCharacter* const shooter,
	AbstCharacter* const target,
	double period,
	int damage,
	int radius,
	int max_speed,
	double survive_time,
	float splash_radius,
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
	_period(period),
	_activated(false)
{
	CCASSERT(target, "target == nullptr");
	
	setTeam(team);
	movePos(_target->getPos() - _pos);

	_activate_strategy.reset(new ActivateContinuous(this, shooter, period, survive_time));
	_impact_strategy.reset(new ImpactContinuousTarget(this, shooter, target, splash_radius));
}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void TargetDirectContinuousSkill::update(double time_elapsed)
{
	_activated = _activate_strategy->test();

	if (_activated)
	{
		_impact_strategy->impact();
	}

	updateMovement(time_elapsed);
	
}

void TargetDirectContinuousSkill::updateMovement(double time_elapsed)
{
	movePos( _target->getPos() - _pos );
}

//-------------------------- render -------------------------------------------
//-----------------------------------------------------------------------------
void TargetDirectContinuousSkill::render()
{}
