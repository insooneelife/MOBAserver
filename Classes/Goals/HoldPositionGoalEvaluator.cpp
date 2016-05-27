#include "HoldPositionGoalEvaluator.h"
#include "GoalThink.h"
#include "../GlobalValues.h"


//------------------------- calculateDesirability -----------------------------
//-----------------------------------------------------------------------------
double HoldPositionGoalEvaluator::calculateDesirability(AbstCharacter* const bot)
{
  double desirability = 0.05;

  desirability *= _character_bias;

  return desirability;
}

//----------------------------- setGoal ---------------------------------------
//-----------------------------------------------------------------------------
void HoldPositionGoalEvaluator::setGoal(AbstCharacter* const bot)
{
	bot->getBrain()->addGoal_HoldPosition(_destination);
}
