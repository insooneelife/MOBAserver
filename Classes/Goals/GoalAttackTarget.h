#ifndef GOAL_ATTACK_TARGET_H
#define GOAL_ATTACK_TARGET_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GoalAttackTarget.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   
//
//-----------------------------------------------------------------------------
#include "GoalComposite.h"
#include "../AbstCharacter.h"


class GoalAttackTarget : public GoalComposite<AbstCharacter>
{
public:

	explicit GoalAttackTarget(AbstCharacter* owner)
		:
		GoalComposite<AbstCharacter>(owner, GOAL_ATTACK_TARGET)
	{}

	virtual ~GoalAttackTarget() override
	{}

	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override;

	virtual void render() override;

};

#endif
