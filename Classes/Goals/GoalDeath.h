#ifndef GOAL_DEATH_H
#define GOAL_DEATH_H

#include "Goal.h"
#include "cocos2d.h"

class AbstCharacter;

class GoalDeath : public Goal<AbstCharacter>
{
public:

	GoalDeath(AbstCharacter* bot);

	virtual ~GoalDeath() override
	{}

	//the usual suspects
	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override
	{}

	virtual void render() override
	{}

protected:
};




#endif

