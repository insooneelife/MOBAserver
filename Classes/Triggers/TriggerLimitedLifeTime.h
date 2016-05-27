#ifndef TRIGGER_LIMITEDLIFETIME_H
#define TRIGGER_LIMITEDLIFETIME_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:     TriggerLimitedLifetime.h
//
//  Author:   Mat Buckland
//
//  Desc:     defines a trigger that only remains in the game for a specified
//            number of update steps
//
//-----------------------------------------------------------------------------

#include "Trigger.h"

class GameWorld;

template <class Entity>
class TriggerLimitedLifetime : public Trigger<Entity>
{
public:

	TriggerLimitedLifetime(
		GameWorld* const world,
		geometry::Shape* const bounding_shape,
		int lifetime)
		:
		Trigger<Entity>(world, bounding_shape),
		_life_time(lifetime)
	{}

	virtual ~TriggerLimitedLifetime() override
	{}

	//children of this class should always make sure this is called from within
	//their own update method
	virtual void update(double time_elapsed) override
	{
		//if the lifetime counter expires set this trigger to be removed from
		//the game
		if (--_life_time <= 0)
		{
			setToBeRemovedFromGame();
		}
	}

	//to be implemented by child classes
	virtual void  actIfOverlap(Entity*) = 0;

	//to be implemented by child classes
	virtual void render() = 0;

	//to be implemented by child classes
	virtual bool handleMessage(const Telegram& msg) override
	{
		return true;
	}

protected:

	//the lifetime of this trigger in update-steps
	int _life_time;
};




#endif