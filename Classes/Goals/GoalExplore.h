#ifndef GOAL_EXPLORE_H
#define GOAL_EXPLORE_H
#pragma warning (disable:4786)

#include "Goals/GoalComposite.h"
#include "../GlobalValues.h"
#include "cocos2d.h"


class AbstCharacter;
class LineGoalThink;

class GoalExplore : public GoalComposite<AbstCharacter>
{
public:

	GoalExplore(AbstCharacter* const owner);

	virtual ~GoalExplore() override
	{}

	virtual void activate() override;

	virtual int process() override;

	virtual void terminate() override
	{}

	virtual void render() override;
};


#endif
