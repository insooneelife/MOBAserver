#include "GoalThink.h"
#include <list>

#include "GoalMoveToPosition.h"
#include "GoalExplore.h"
#include "../GlobalValues.h"
#include "GoalAttackTarget.h"
#include "GoalSpecialCasting.h"
#include "GoalDeath.h"
#include "GoalPersistanceAttack.h"

#include "../AbstCharacter.h"
#include "../Util.h"
#include "../AbstWeaponSystem.h"
#include "../AbstWeapon.h"
#include "../MeleeWeapon.h"

#include "GoalHoldPosition.h"

#include "ExploreGoalEvaluator.h"
#include "AttackTargetGoalEvaluator.h"
#include "HoldPositionGoalEvaluator.h"

GoalThink::GoalThink(AbstCharacter* const bot)
	:
	GoalComposite<AbstCharacter>(bot, GOAL_THINK)
{}

//----------------------------- dtor ------------------------------------------
//-----------------------------------------------------------------------------
GoalThink::~GoalThink()
{}

//------------------------------- activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalThink::activate()
{
	if (!_owner->isPossessed())
	{
		arbitrate();
	}

	_status = ACTIVE;
}

//------------------------------ process --------------------------------------
//
//  processes the subgoals
//-----------------------------------------------------------------------------
int GoalThink::process()
{
	activateIfInactive();

	int subgoal_status = processSubgoals();

	if (subgoal_status == COMPLETED || subgoal_status == FAILED)
	{
		if (!_owner->isPossessed())
		{
			_status = INACTIVE;
		}
	}

	return _status;
}

//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalThink::render()
{
	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}

//----------------------------- update ----------------------------------------
// 
//  this method iterates through each goal option to determine which one has
//  the highest desirability.
//-----------------------------------------------------------------------------
void GoalThink::arbitrate()
{
	double best = 0;
	GoalEvaluator* most_desirable = nullptr;

	//iterate through all the evaluators to see which produces the highest score
	for (auto des = std::begin(_evaluators); des != std::end(_evaluators); ++des)
	{
		double desirabilty = (*des)->calculateDesirability(_owner);

		if (desirabilty >= best)
		{
			best = desirabilty;
			most_desirable = (*des).get();
		}
	}

	CCASSERT(most_desirable, "<GoalThink::arbitrate>: no evaluator selected");

	most_desirable->setGoal(_owner);
}


//---------------------------- notPresent --------------------------------------
//
//  returns true if the goal type passed as a parameter is the same as this
//  goal or any of its subgoals
//-----------------------------------------------------------------------------
bool GoalThink::notPresent(unsigned int goal_type)const
{
	if (!_subgoals.empty())
	{
		return _subgoals.front()->getType() != goal_type;
	}

	return true;
}

void GoalThink::addGoal_MoveToPosition(cocos2d::Vec2 pos)
{
	addSubgoal(new GoalMoveToPosition(_owner, pos));
}

void GoalThink::addGoal_BasicCasting(cocos2d::Vec2 pos, CastingType type)
{
	addSubgoal(new GoalSpecialCasting(_owner, pos, type));
}

void GoalThink::addGoal_Explore()
{
	if (notPresent(GOAL_EXPLORE))
	{
		removeAllSubgoals();
		addSubgoal(new GoalExplore(_owner));
	}
}

void GoalThink::addGoal_Death()
{
	addSubgoal(new GoalDeath(_owner));
}

void GoalThink::addGoal_AttackTarget()
{
	if (notPresent(GOAL_ATTACK_TARGET))
	{
		removeAllSubgoals();
		addSubgoal(new GoalAttackTarget(_owner));
	}
}

void GoalThink::addGoal_HoldPosition(const cocos2d::Vec2& destination)
{
	if (notPresent(GOAL_HOLD_POSITION))
	{
		removeAllSubgoals();
		addSubgoal(new GoalHoldPosition(_owner, destination));
	}
}

void GoalThink::addGoal_PersistanceAttack()
{
	if (_owner->getTargetSys()->isTargetPresent())
	{
		if (notPresent(GOAL_PERSISTANCE_ATTACK))
		{
			removeAllSubgoals();
			addSubgoal(new GoalPersistanceAttack(_owner));
		}
	}
	else
	{
		CCLOG("no target!");
	}
}

//-------------------------- Queue Goals --------------------------------------
//-----------------------------------------------------------------------------
void GoalThink::queueGoal_MoveToPosition(cocos2d::Vec2 pos)
{
	_subgoals.push_back(new GoalMoveToPosition(_owner, pos));
}
