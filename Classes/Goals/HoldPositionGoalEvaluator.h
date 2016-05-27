#ifndef HOLD_POSITION_GOAL_EVALUATOR_H
#define HOLD_POSITION_GOAL_EVALUATOR_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   HoldPositionGoalEvaluator.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:  class to calculate how desirable the goal of exploring is
//-----------------------------------------------------------------------------

#include "GoalEvaluator.h"
#include "../AbstCharacter.h"
#include "cocos2d.h"


class HoldPositionGoalEvaluator : public GoalEvaluator
{
public:

	explicit HoldPositionGoalEvaluator(double bias, const cocos2d::Vec2& destination)
		:
		GoalEvaluator(bias),
		_destination(destination)
	{}

	virtual ~HoldPositionGoalEvaluator() override
	{}

	virtual double calculateDesirability(AbstCharacter* const bot) override;

	virtual void  setGoal(AbstCharacter* const bot) override;

private:
	cocos2d::Vec2 _destination;
};

#endif