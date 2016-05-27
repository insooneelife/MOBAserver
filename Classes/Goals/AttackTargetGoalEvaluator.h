#ifndef ATTACK_TARGET_GOAL_EVALUATOR
#define ATTACK_TARGET_GOAL_EVALUATOR
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   AttackTargetGoalEvaluator.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:  class to calculate how desirable the goal of attacking the bot's
//         current target is
//-----------------------------------------------------------------------------

#include "GoalEvaluator.h"
#include "../AbstCharacter.h"


class AttackTargetGoalEvaluator : public GoalEvaluator
{
public:

	explicit AttackTargetGoalEvaluator(double bias)
		:
		GoalEvaluator(bias) 
	{}

	virtual ~AttackTargetGoalEvaluator() override
	{}

	virtual double calculateDesirability(AbstCharacter* const bot) override;

	virtual void  setGoal(AbstCharacter* const ent) override;

};



#endif