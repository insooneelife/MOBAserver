#ifndef GOAL_EVALUATOR_H
#define GOAL_EVALUATOR_H
#pragma warning (disable : 4786)
//-----------------------------------------------------------------------------
//
//  Name:   GoalEvaluator.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class template that defines an interface for objects that are
//          able to evaluate the desirability of a specific strategy level goal
//-----------------------------------------------------------------------------

#include "cocos2d.h"

class AbstCharacter;

class GoalEvaluator
{
public:

	explicit GoalEvaluator(double character_bias)
		:
		_character_bias(character_bias)
	{}

	virtual ~GoalEvaluator()
	{}

	//returns a score between 0 and 1 representing the desirability of the
	//strategy the concrete subclass represents
	virtual double calculateDesirability(AbstCharacter* const bot) = 0;

	//adds the appropriate goal to the given bot's brain
	virtual void  setGoal(AbstCharacter* const bot) = 0;

protected:

	//when the desirability score for a goal has been evaluated it is multiplied 
	//by this value. It can be used to create bots with preferences based upon
	//their personality
	double       _character_bias;

};




#endif

