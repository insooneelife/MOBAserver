#include "GoalMainAttack.h"
#include "Message/MessageDispatcher.h"
#include "../AbstCharacter.h"
#include "../GlobalValues.h"
#include "../Motion.h"
#include "../AbstWeaponSystem.h"
#include "../AbstTargetingSystem.h"

USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalMainAttack::GoalMainAttack(AbstCharacter* const owner)
	:
	Goal<AbstCharacter>(owner, GOAL_MAIN_ATTACK)
{}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalMainAttack::activate()
{
	_status = ACTIVE;
	if (_owner->getTargetSys()->isTargetPresent())
	{
		if (_owner->getTargetSys()->isTargetAttackable())
		{
			_owner->setVelocity(Vec2(0, 0));
			_owner->setHeading(_owner->getAttackDirection());

			_owner->getWeaponSys()->attack();

			_owner->setMotionDirect("Attack1");
			_owner->getMotion()->setActive(true);
		}
		else
		{
			_status = FAILED;
		}
	}
	else
	{
		_status = FAILED;
	}
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
int GoalMainAttack::process()
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
void GoalMainAttack::terminate()
{
	_status = COMPLETED;
	_owner->getMotion()->setActive(false);
}


//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalMainAttack::render()
{}

