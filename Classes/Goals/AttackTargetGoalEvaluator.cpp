#include "AttackTargetGoalEvaluator.h"
#include "GoalThink.h"
#include "Feature.h"
#include "../AbstWeaponSystem.h"


//------------------ CalculateDesirability ------------------------------------
//
//  returns a value between 0 and 1 that indicates the Rating of a bot (the
//  higher the score, the stronger the bot).
//-----------------------------------------------------------------------------
double AttackTargetGoalEvaluator::calculateDesirability(AbstCharacter* const bot)
{
	double desirability = 0.0;

	//only do the calculation if there is a target present
	if (bot->getTargetSys()->isTargetPresent())
	{
		desirability = 1;

		//bias the value according to the personality of the bot
		desirability *= _character_bias;
	}

	return desirability;
}

//----------------------------- SetGoal ---------------------------------------
//-----------------------------------------------------------------------------
void AttackTargetGoalEvaluator::setGoal(AbstCharacter* const bot)
{
	bot->getBrain()->addGoal_AttackTarget();
}
