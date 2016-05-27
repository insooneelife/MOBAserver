#include "GoalSeekToPosition.h"
#include "../AbstCharacter.h"
#include "../Timer/CrudeTimer.h"
#include "../SteeringBehavior.h"
#include "../GlobalValues.h"
#include "../Motion.h"
#include "../Message/Telegram.h"

USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalSeekToPosition::GoalSeekToPosition(
	AbstCharacter* const bot,
	cocos2d::Vec2   target,
	float radius)
	:
	Goal<AbstCharacter>(bot, GOAL_SEEK_TO_POS),
	_position(target),
	_time_to_reach(0.0),
	_radius(radius)
{}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalSeekToPosition::activate()
{
	_status = ACTIVE;

	//record the time the bot starts this goal
	_start_time = Clock.getCurrentTime();

	//This value is used to determine if the bot becomes stuck 
	_time_to_reach = _owner->calculateTimeToReachPosition(_position);

	//factor in a margin of error for any reactive behavior
	const double margin_of_error = 0.5f;

	_time_to_reach += margin_of_error;

	_owner->getSteering()->setTarget(_position);
	_owner->setHeading((_position - _owner->getPos()).getNormalized());
	_owner->setMotionDirect("Walk");
	_owner->getMotion()->setActive(true);

	_owner->getSteering()->seekOn();
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
int GoalSeekToPosition::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();

	//test to see if the bot has become stuck
	if (isStuck())
	{
		_status = FAILED;
	}

	//test to see if the bot has reached the waypoint. If so terminate the goal
	else
	{
		if (_owner->isAtPosition(_position, _radius))
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
bool GoalSeekToPosition::isStuck()const
{
	double time_taken = Clock.getCurrentTime() - _start_time;
	if (time_taken > _time_to_reach)
	{
		CCLOG("BOT %u  IS STUCK!!", _owner->getId());

		return true;
	}

	return false;
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalSeekToPosition::terminate()
{
	_owner->getSteering()->seekOff();
	_owner->setMotionDirect("Neutral");
	_owner->setVelocity(Vec2(0, 0));
	_status = COMPLETED;
}

//---------------------------- handleMessage ----------------------------------
//-----------------------------------------------------------------------------
bool GoalSeekToPosition::handleMessage(const Telegram& msg)
{
	switch (msg.msg)
	{
	case MSG_PATH_READY:
		CCLOG("Goal MSG_PATH_READY");
		terminate();
		//msg handled but this case the message has to be
		//more handled with object goal.
		return false;

	case MSG_NOPATH_AVAILABLE:
		CCLOG("Goal MSG_NOPATH_AVAILABLE");
		_status = FAILED;

		return true; //msg handled

	default: return false;
	}


//handled by subgoals
return true;
}


//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalSeekToPosition::render()
{}