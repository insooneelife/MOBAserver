#include "GoalTraverseEdge.h"
#include "../AbstCharacter.h"
#include "../SteeringBehavior.h"
#include "../Timer/CrudeTimer.h"
#include "../GlobalValues.h"
#include "../PathPlanner/PathPlanner.h"
#include "../Motion.h"

USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalTraverseEdge::GoalTraverseEdge(
	AbstCharacter* bot,
	PathEdge edge,
	float radius,
	bool last_edge) 
	:
	Goal<AbstCharacter>(bot, GOAL_TRAVERSE_EDGE),
	_edge(edge),
	_time_expected(0.0),
	_last_edge_in_path(last_edge),
	_radius(radius)
{}



//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalTraverseEdge::activate()
{
	_status = ACTIVE;

	//the edge behavior flag may specify a type of movement that necessitates a 
	//change in the bot's max possible speed as it follows this edge
	switch (_edge.getBehavior())
	{
	case NavGraphEdge::SWIM:
	{
		_owner->setMaxSpeed(_owner->getMaxSpeed() * 0.2);
	}

	break;

	case NavGraphEdge::CRAWL:
	{
		_owner->setMaxSpeed(_owner->getMaxSpeed() * 0.6);
	}

	break;
	}

	//record the time the bot starts this goal
	_start_time = Clock.getCurrentTime();

	//calculate the expected time required to reach the this waypoint. This value
	//is used to determine if the bot becomes stuck 
	_time_expected = _owner->calculateTimeToReachPosition(_edge.getDestination());

	//factor in a margin of error for any reactive behavior
	static const double margin_of_error = 2.0;

	_time_expected += margin_of_error;

	//set the steering target
	_owner->getSteering()->setTarget(_edge.getDestination());
	_owner->setHeading((_edge.getDestination() - _owner->getPos()).getNormalized());
	_owner->setMotionDirect("Walk");
	_owner->getMotion()->setActive(true);

	//Set the appropriate steering behavior. If this is the last edge in the path
	//the bot should arrive at the position it points to, else it should seek
	
	_owner->getSteering()->seekOn();
}



//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
int GoalTraverseEdge::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();

	//if the bot has become stuck return failure
	if (isStuck())
	{
		_status = FAILED;
	}

	//if the bot has reached the end of the edge return COMPLETED
	else
	{
		if (_owner->isAtPosition(_edge.getDestination(), _radius))
		{
			_status = COMPLETED;
		}
	}

	return _status;
}

//--------------------------- isBotStuck --------------------------------------
//
//  returns true if the bot has taken longer than expected to reach the 
//  currently ACTIVE waypoint
//-----------------------------------------------------------------------------
bool GoalTraverseEdge::isStuck()const
{
	double TimeTaken = Clock.getCurrentTime() - _start_time;

	if (TimeTaken > _time_expected)
	{
		CCLOG("BOT %u  IS STUCK!!", _owner->getId());

		return true;
	}

	return false;
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalTraverseEdge::terminate()
{
	//turn off steering behaviors.
	_owner->getSteering()->seekOff();

	//return max speed back to NORMAL
	_owner->setMaxSpeed(_owner->getMaxSpeed());
}

//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalTraverseEdge::render()
{}