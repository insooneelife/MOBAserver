#include "GoalHoldPosition.h"
#include "../GameWorld.h"
#include "../GameMap.h"
#include "../Message/Telegram.h"
#include "../GlobalValues.h"
#include "../Util.h"
#include "../ParameterLoader.h"
#include "GoalSeekToPosition.h"
#include "GoalFollowPath.h"
#include "GoalMoveToPosition.h"



//------------------------------ activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalHoldPosition::activate()
{
	_status = ACTIVE;

	//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
	removeAllSubgoals();

	//and request a path to that position

	//the bot may have to wait a few update cycles before a path is calculated
	//so for appearances sake it simple ARRIVES at the destination until a path
	//has been found
	
	// 목표위치 수정하기
	addSubgoal(new GoalMoveToPosition(_owner, _destination, _radius));
}

//------------------------------ process -------------------------------------
//-----------------------------------------------------------------------------
int GoalHoldPosition::process()
{
	if (_owner->isAtPosition(_destination, _radius))
		return _status = COMPLETED;
	//if status is INACTIVE, call activate()
	activateIfInactive();

	//process the subgoals
	_status = processSubgoals();

	return _status;
}


//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalHoldPosition::render()
{
}



