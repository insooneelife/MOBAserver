#include "GoalHuntTarget.h"
#include "GoalExplore.h"
#include "GoalMoveToPosition.h"
#include "../AbstCharacter.h"
#include "../SteeringBehavior.h"

USING_NS_CC;

//---------------------------- initialize -------------------------------------
//-----------------------------------------------------------------------------  
void GoalHuntTarget::activate()
{
	_status = ACTIVE;
	//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
	removeAllSubgoals();

	//it is possible for the target to die whilst this goal is active so we
	//must test to make sure the bot always has an active target
	if (_owner->getTargetSys()->isTargetPresent())
	{
		//grab a local copy of the last recorded position (LRP) of the target
		const cocos2d::Vec2 lrp = _owner->getTargetSys()->getLastRecordedPosition();

		//if the bot has reached the LRP and it still hasn't found the target
		//it starts to search by using the explore goal to move to random
		//map locations
		if (lrp.isZero() || _owner->isAtPosition(lrp))
		{
			_status = COMPLETED;
		}

		//else move to the LRP
		else
		{	
			addSubgoal(new GoalMoveToPosition(_owner, lrp));
		}
	}

	//if their is no active target then this goal can be removed from the queue
	else
	{
		_status = COMPLETED;
	}

}

//------------------------------ Process --------------------------------------
//-----------------------------------------------------------------------------
int GoalHuntTarget::process()
{
	//if status is inactive, call Activate()
	activateIfInactive();

	_status = processSubgoals();

	//if target is in view this goal is satisfied
	//if (_owner->getTargetSys()->isTargetWithinFOV())
	//if (_owner->getTargetSys()->isTargetAttackable())
	//{
	//	_status = COMPLETED;
	//}
	//CCLOG("GoalHuntTarget : %d", _status);
	return _status;
}

void GoalHuntTarget::render()
{
	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}
