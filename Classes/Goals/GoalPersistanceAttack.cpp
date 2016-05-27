#include "GoalPersistanceAttack.h"
#include "../AbstCharacter.h"
#include "../GameWorld.h"
#include "../AbstTargetingSystem.h"
#include "GoalHuntTarget.h"
#include "GoalMainAttack.h"

USING_NS_CC;

//------------------------------ ctor -----------------------------------------
//-----------------------------------------------------------------------------
GoalPersistanceAttack::GoalPersistanceAttack(AbstCharacter* const owner)
	:
	GoalComposite<AbstCharacter>(owner, GOAL_PERSISTANCE_ATTACK)
{}


//------------------------------ activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalPersistanceAttack::activate()
{
	_status = ACTIVE;

	if (_owner->getTargetSys()->isTargetPresent())
	{
		if (_owner->getTargetSys()->getTarget()->isAlive())
		{
			if (_owner->getTargetSys()->isTargetAttackable())
			{
				addSubgoal(new GoalMainAttack(_owner));
			}
			else
			{
				addSubgoal(new GoalHuntTarget(_owner));
			}
		}
		else
		{
			_status = COMPLETED;
		}
	}
	else
	{
		_status = COMPLETED;
	}
}


//-------------------------- process ------------------------------------------
//-----------------------------------------------------------------------------
int GoalPersistanceAttack::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();

	_status = processSubgoals();
	if (_status == COMPLETED && _owner->getTargetSys()->getTarget()->isAlive())
	{
		activate();
	}

	return _status;
}

void GoalPersistanceAttack::terminate()
{
	_owner->setMotionDirect("Neutral");
	_owner->setVelocity(Vec2(0, 0));
}

//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalPersistanceAttack::render()
{
	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}



