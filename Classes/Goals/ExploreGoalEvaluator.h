#ifndef EXPLORE_GOAL_EVALUATOR
#define EXPLORE_GOAL_EVALUATOR
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   ExploreGoalEvaluator.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:  class to calculate how desirable the goal of exploring is
//-----------------------------------------------------------------------------

#include "GoalEvaluator.h"
#include "../AbstCharacter.h"
#include "cocos2d.h"

class ExploreGoalEvaluator : public GoalEvaluator
{
public:

	ExploreGoalEvaluator(double bias);

	virtual ~ExploreGoalEvaluator() override
	{}

	virtual double calculateDesirability(AbstCharacter* const bot) override;

	virtual void  setGoal(AbstCharacter* const bot) override;
};

#endif