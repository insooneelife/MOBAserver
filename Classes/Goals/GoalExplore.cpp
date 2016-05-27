#include "GoalExplore.h"
#include "../GameWorld.h"
#include "../GameMap.h"
#include "../Message/Telegram.h"
#include "../GlobalValues.h"
#include "../Util.h"
#include "../ParameterLoader.h"
#include "GoalSeekToPosition.h"
#include "GoalFollowPath.h"
#include "GoalMoveToPosition.h"
#include "LineGoalThink.h"


GoalExplore::GoalExplore(AbstCharacter* const owner)
	:
	GoalComposite<AbstCharacter>(owner, GOAL_EXPLORE)
{}

//------------------------------ activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalExplore::activate()
{
	_status = ACTIVE;

	//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
	removeAllSubgoals();
	
	//and request a path to that position

	//the bot may have to wait a few update cycles before a path is calculated
	//so for appearances sake it simple ARRIVES at the destination until a path
	//has been found
	
	geometry::Circle dest = 
		_owner->getWorld()->getDestination(
			_owner->getPathNumber(),
			_owner->getLine(),
			_owner->getTeam());

	addSubgoal(new GoalMoveToPosition(_owner, dest.origin, dest.radius));
}

//------------------------------ process -------------------------------------
//-----------------------------------------------------------------------------
int GoalExplore::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();

	//process the subgoals
	_status = processSubgoals();

	if (_status == COMPLETED)
		_owner->setPathNumber((_owner->getPathNumber() + 1) % 7);

	//	_owner->getPathQ().pop();

	return _status;
}


//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalExplore::render()
{
	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}



