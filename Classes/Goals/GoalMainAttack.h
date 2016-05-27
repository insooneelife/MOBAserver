#ifndef GOAL_MAIN_ATTACK_H
#define GOAL_MAIN_ATTACK_H
#pragma warning (disable:4786)

#include "Goal.h"
#include "cocos2d.h"
#include "../AbstCharacter.h"


class GoalMainAttack : public Goal<AbstCharacter>
{
public:

	explicit GoalMainAttack(AbstCharacter* owner);

	virtual ~GoalMainAttack() override
	{}

	//the usual suspects
	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override;

	virtual void render() override;

};




#endif

