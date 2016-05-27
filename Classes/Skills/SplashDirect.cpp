#include "TargetSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "GlobalValues.h"
#include "Message/MessageDispatcher.h"

#include "ImpactSplash.h"
#include "ActivateDirect.h"
#include "SplashDirect.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
SplashDirect::SplashDirect(
	AbstCharacter* const shooter,
	cocos2d::Vec2 target_pos,
	int damage,
	int radius,
	int max_speed,
	double survive_time,
	float splash_radius,
	const std::string& name)
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
	_splash_radius(splash_radius)
{
	setPos(_target_pos);
	CCLOG("survive time: %d", _survive_time);
	_activate_strategy.reset(new ActivateDirect(this));
	_impact_strategy.reset(new ImpactSplash(this, shooter, splash_radius));

}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void SplashDirect::update(double time_elapsed)
{
	/*if (Clock.getCurrentTime() > _survive_time + _time_of_creation)
	{
		_dead = true;
	}
	*/

	//if (isDead())
	//	return;

	_activated = _activate_strategy->test();

	
	if (_activated)
	{
		_impact_strategy->impact();
	}

	//updateMovement(time_elapsed);
	
}

void SplashDirect::updateMovement(double time_elapsed)
{
	//setPos(_target_pos);
}

//-------------------------- render -------------------------------------------
//-----------------------------------------------------------------------------
void SplashDirect::render()
{}
