#include "GoalDeath.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "Motion.h"

USING_NS_CC;

GoalDeath::GoalDeath(AbstCharacter* bot)
	:
	Goal<AbstCharacter>(bot, GOAL_DEATH)
{}

//------------------------------ activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalDeath::activate()
{
	_status = ACTIVE;

	_owner->setVelocity(Vec2(0, 0));
	_owner->setMotionDirect("Death");
	_owner->getMotion()->setActive(true);
	_owner->setDying();
}

//------------------------------ process -------------------------------------
//-----------------------------------------------------------------------------
int GoalDeath::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();

	if (!_owner->getMotion()->getFrameActive())
	{
		_owner->setMotionDirect("Dead");
		_owner->getMotion()->setActive(true);
		_owner->setDead();
		_status = COMPLETED;
	}

	return _status;
}


