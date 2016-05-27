#include "TargetSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "GlobalValues.h"
#include "Message/MessageDispatcher.h"

#include "ImpactContinuousSplash.h"
#include "ActivateContinuous.h"
#include "SplashContinuousSkill.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
SplashContinuousSkill::SplashContinuousSkill(
	AbstCharacter* const shooter,
	cocos2d::Vec2 target_pos,
	double period,
	int damage,
	int radius,
	int max_speed,
	double survive_time,
	float splash_radius,
	const std::string& name,
	int team)
	:
	AbstNonTargetSkill(
		shooter->getWorld(),
		target_pos,
		shooter->getId(),
		target_pos,
		shooter->getHeading(),
		damage,
		radius,
		max_speed,
		1,
		10,
		name,
		SKILL,
		survive_time),
	_activated(false),
	_target_pos(target_pos),
	_splash_radius(splash_radius),
	_period(period)
{
	setTeam(team);
	setPos(_target_pos);
	_activate_strategy.reset(new ActivateContinuous(this, shooter, _period, _survive_time));
	_impact_strategy.reset(new ImpactContinuousSplash(this, shooter, splash_radius));

}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void SplashContinuousSkill::update(double time_elapsed)
{
	/*if (Clock.getCurrentTime() > _survive_time + _time_of_creation)
	{
		_dead = true;
	}
	*/

	if (isDead())
		return;

	_activated = _activate_strategy->test();

	
	if (_activated)
	{
		_impact_strategy->impact();
	}

	//updateMovement(time_elapsed);
	
}

void SplashContinuousSkill::updateMovement(double time_elapsed)
{
	//setPos(_target_pos);
}

//-------------------------- render -------------------------------------------
//-----------------------------------------------------------------------------
void SplashContinuousSkill::render()
{}
