#ifndef GOAL_SEEK_TO_POSITION_H
#define GOAL_SEEK_TO_POSITION_H
#pragma warning (disable:4786)

#include "Goal.h"
#include "cocos2d.h"
#include "../AbstCharacter.h"


class GoalSeekToPosition : public Goal<AbstCharacter>
{
public:

	GoalSeekToPosition(
		AbstCharacter* bot,
		cocos2d::Vec2 target,
		float radius);

	virtual ~GoalSeekToPosition() override
	{}

	//the usual suspects
	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override;

	virtual bool handleMessage(const Telegram& msg) override;

	virtual void render() override;

protected:

	//the position the bot is moving to
	cocos2d::Vec2	_position;

	float _radius;

	//the approximate time the bot should take to travel the target location
	double			_time_to_reach;

	//this records the time this goal was activated
	double			_start_time;

	//returns true if a bot gets stuck
	bool	isStuck() const;
};




#endif

