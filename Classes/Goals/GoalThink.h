#ifndef GOAL_THINK_H
#define GOAL_THINK_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GoalThink .h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to arbitrate between a collection of high level goals, and
//          to process those goals.
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <memory>
#include "cocos2d.h"
#include "GoalComposite.h"
#include "../AbstCharacter.h"
#include "GoalEvaluator.h"

class GoalThink : public GoalComposite<AbstCharacter>
{
protected:
	typedef std::vector<std::unique_ptr<GoalEvaluator> >   GoalEvaluators;

public:

	explicit GoalThink(AbstCharacter* const	bot);

	virtual ~GoalThink() override;

	//this method iterates through each goal evaluator and selects the one
	//that has the highest score as the current goal
	virtual void arbitrate();

	//returns true if the given goal is not at the front of the subgoal list
	bool notPresent(unsigned int goal_type)const;

	//the usual suspects
	virtual int  process() override;

	virtual void activate() override;

	virtual void terminate() override
	{}

	virtual void render() override;

	//top level goal types
	virtual void addGoal_MoveToPosition(cocos2d::Vec2 pos);

	virtual void addGoal_BasicCasting(cocos2d::Vec2 pos, CastingType type);

	//virtual void AddGoal_GetItem(unsigned int item_type);

	virtual void addGoal_Explore();

	virtual void addGoal_Death();

	virtual void addGoal_AttackTarget();

	virtual void addGoal_HoldPosition(const cocos2d::Vec2& destination);

	virtual void addGoal_PersistanceAttack();


	//this adds the MoveToPosition goal to the *back* of the subgoal list.
	virtual void queueGoal_MoveToPosition(cocos2d::Vec2 pos);

protected:

	GoalEvaluators			_evaluators;

};


#endif