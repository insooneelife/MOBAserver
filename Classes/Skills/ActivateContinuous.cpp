#include "ActivateContinuous.h"
#include "../AbstCharacter.h"
#include "../CollisionArea.h"
#include "../CollisionTestTemplates.h"
#include "../Message/MessageDispatcher.h"

ActivateContinuous::ActivateContinuous(
	AbstSkill* const owner_skill, AbstCharacter* const shooter, double period, double survive_time)
	:
	ActivateStrategy(owner_skill),
	_shooter(shooter),
	_period(period),
	_survive_time(survive_time)
{}

//------------------------------ test -------------------------------------
//
//  This function checks through cellspaces to find if any collision occurs 
//  within other characters and collision areas.
//-------------------------------------------------------------------------

bool ActivateContinuous::test()
{

	if (_survive_time-- <= 0)
	{
		_owner_skill->setDead();
		return false;
	}

	if ( (int)_survive_time % (int)_period == 0)
	{
		return true;
	}

	return false;
}

double ActivateContinuous::getPeriod()
{
	return _period;
}

void ActivateContinuous::setPeriod(double period)
{
	_period = period;
}

double ActivateContinuous::getSurviveTime()
{
	return _survive_time;
}

void ActivateContinuous::setSurviveTime(double time)
{
	_survive_time = time;
}
