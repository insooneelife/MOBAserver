#ifndef GOAL_MOVE_POS_H
#define GOAL_MOVE_POS_H
#pragma warning (disable:4786)

#include "Goals/GoalComposite.h"
#include "cocos2d.h"
#include "../AbstCharacter.h"
#include "../GlobalValues.h"


class GoalMoveToPosition : public GoalComposite<AbstCharacter>
{
public:

	GoalMoveToPosition(
		AbstCharacter* const bot,
		cocos2d::Vec2 pos,
		float radius = 10) 
		:
		GoalComposite<AbstCharacter>(bot, GOAL_MOVE_TO_POS),
		_destination(pos),
		_radius(radius)
	{}

	virtual ~GoalMoveToPosition() override
	{}

	//the usual suspects
	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override
	{}

	virtual void render() override;

	//this goal is able to accept messages
	virtual bool handleMessage(const Telegram& msg) override;

protected:

	//the position the bot wants to reach
	cocos2d::Vec2 _destination;

	float _radius;
};


#endif
