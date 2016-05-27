#ifndef GOAL_HUNT_TARGET_H
#define GOAL_HUNT_TARGET_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GoalHuntTarget.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Causes a bot to search for its current target. Exits when target
//          is in view
//-----------------------------------------------------------------------------
#include "GoalComposite.h"
#include "../AbstCharacter.h"


class GoalHuntTarget : public GoalComposite<AbstCharacter>
{
protected:

	//this value is set to true if the last visible position of the target
	//bot has been searched without success
	bool  _last_visit_visible_pos_tried;

public:

	explicit GoalHuntTarget(AbstCharacter* const bot)
		:
		GoalComposite<AbstCharacter>(bot, GOAL_HUNT_TARGET),
		_last_visit_visible_pos_tried(false)
	{}

	virtual ~GoalHuntTarget() override
	{}

	//the usual suspects
	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override
	{}

	virtual void render() override;
};


#endif