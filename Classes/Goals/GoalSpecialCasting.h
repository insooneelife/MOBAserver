#ifndef GOAL_SPECIAL_CASTING_H
#define GOAL_SPECIAL_CASTING_H
#pragma warning (disable:4786)

#include "Goal.h"
#include "cocos2d.h"
#include "../AbstCharacter.h"


class GoalSpecialCasting : public Goal<AbstCharacter>
{
public:

	GoalSpecialCasting(
		AbstCharacter* bot,
		cocos2d::Vec2 target,
		CastingType type);

	virtual ~GoalSpecialCasting() override
	{}

	//the usual suspects
	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override;

	virtual void render() override;


private:
	cocos2d::Vec2 _target;

	CastingType _type;
};




#endif

