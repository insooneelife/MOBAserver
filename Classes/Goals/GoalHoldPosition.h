#ifndef GOAL_HOLD_POSITION_H
#define GOAL_HOLD_POSITION_H
#pragma warning (disable:4786)

#include "Goals/GoalComposite.h"
#include "../GlobalValues.h"
#include "cocos2d.h"


class AbstCharacter;


class GoalHoldPosition : public GoalComposite<AbstCharacter>
{
public:

	GoalHoldPosition(
		AbstCharacter* const owner,
		const cocos2d::Vec2& destination,
		float radius = 10)
		:
		GoalComposite<AbstCharacter>(owner, GOAL_HOLD_POSITION),
		_destination(destination),
		_radius(radius)
	{}

	virtual ~GoalHoldPosition() override
	{}

	virtual void activate() override;

	virtual int process() override;

	virtual void terminate() override
	{}

	virtual void render() override;

protected:

	cocos2d::Vec2  _destination;

	float _radius;

private:
};


#endif
