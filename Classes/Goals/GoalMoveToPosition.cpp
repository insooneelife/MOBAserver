#include "GoalMoveToPosition.h"
#include "../AbstCharacter.h"
#include "../GameMap.h"
#include "../Message/Telegram.h"

#include "GoalSeekToPosition.h"
#include "GoalFollowPath.h"

USING_NS_CC;

//------------------------------- activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalMoveToPosition::activate()
{
	_status = ACTIVE;

	//make sure the subgoal list is clear.
	removeAllSubgoals();

	//requests a path to the target position from the path planner. Because, for
	//demonstration purposes, the Raven path planner uses time-slicing when 
	//processing the path requests the bot may have to wait a few update cycles
	//before a path is calculated. Consequently, for appearances sake, it just
	//seeks directly to the target position whilst it's awaiting notification
	//that the path planning request has succeeded/FAILED
	//CCLOG("POSITION: %lf %lf\n", _destination.x, _destination.y);
	if (_owner->getPathPlanner()->requestPathToPosition(_destination))
	{
		addSubgoal(new GoalSeekToPosition(_owner, _destination, _radius));
	}
}

//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
int GoalMoveToPosition::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();

	//process the subgoals
	_status = processSubgoals();

	//if any of the subgoals have FAILED then this goal re-plans
	reactivateIfFailed();
	return _status;
}

//---------------------------- handleMessage ----------------------------------
//-----------------------------------------------------------------------------
bool GoalMoveToPosition::handleMessage(const Telegram& msg)
{
	//first, pass the message down the goal hierarchy
	bool bHandled = forwardMessageToFrontMostSubgoal(msg);
	// CR: deleted four lines containing pathPlanningTime
	//     see RavenPlanner::cycleOnce

	//if the msg was not handled, test to see if this goal can handle it
	if (bHandled == false)
	{
		switch (msg.msg)
		{
		case MSG_PATH_READY:

		  //clear any existing goals
			removeAllSubgoals();

			addSubgoal(new GoalFollowPath(_owner, _owner->getPathPlanner()->getPath()));

			return true; //msg handled


		case MSG_NOPATH_AVAILABLE:

			_status = FAILED;

			return true; //msg handled

		default: return false;
		}
	}

	//handled by subgoals
	return true;
}

//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalMoveToPosition::render()
{
	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}