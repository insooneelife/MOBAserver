#include "GoalAttackTarget.h"
#include "GoalMainAttack.h"
#include "GoalHuntTarget.h"
#include "../AbstCharacter.h"


//------------------------------- Activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalAttackTarget::activate()
{
	_status = ACTIVE;

	//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
	removeAllSubgoals();

	//it is possible for a bot's target to die whilst this goal is active so we
	//must test to make sure the bot always has an active target
	if (!_owner->getTargetSys()->isTargetPresent())
	{
		_status = COMPLETED;

		return;
	}

	//if the bot is able to shoot the target (there is LOS between bot and target),
	//then select a tactic to follow while shooting
	if (_owner->getTargetSys()->isTargetAttackable())
	{
		addSubgoal(new GoalMainAttack(_owner));
	}

	//if the target is not visible, go hunt it.
	else
	{
		if (_owner->getType() != BaseGameEntity::TOWER_CHARACTER)
			addSubgoal(new GoalHuntTarget(_owner));
		//addSubgoal(new GoalHuntTarget(_owner));
	}
}

//-------------------------- Process ------------------------------------------
//-----------------------------------------------------------------------------
int GoalAttackTarget::process()
{
	//if status is inactive, call Activate()
	activateIfInactive();

	//process the subgoals
	_status = processSubgoals();

	reactivateIfFailed();

	return _status;
}

void GoalAttackTarget::terminate()
{
	_status = COMPLETED;
}

void GoalAttackTarget::render()
{
	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}


