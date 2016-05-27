#include "GoalSpecialCasting.h"
#include "../AbstCharacter.h"
#include "../GlobalValues.h"
#include "../Motion.h"
#include "../AbstWeaponSystem.h"
#include "../AbstWeapon.h"

USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalSpecialCasting::GoalSpecialCasting(
	AbstCharacter* const bot,
	Vec2 target,
	CastingType type)
	:
	Goal<AbstCharacter>(bot, GOAL_SPECIAL_CASTING),
	_target(target),
	_type(type)
{}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalSpecialCasting::activate()
{
	_status = ACTIVE;
	_owner->setVelocity(Vec2(0, 0));
	_owner->setHeading((_target - _owner->getPos()).getNormalized());
	_owner->setMotionDirect("Casting");

	_owner->getWeaponSys()->getWeaponFromInventory(_type)->attack(_target);
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
int GoalSpecialCasting::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();
	if (!_owner->getMotion()->getFrameActive())
	{
		return _status = COMPLETED;
	}
	return _status = ACTIVE;
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalSpecialCasting::terminate()
{
	_status = COMPLETED;
	_owner->setMotionDirect("Neutral");
}


//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalSpecialCasting::render()
{}

