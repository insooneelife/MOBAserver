#ifndef Trigger_Respawning_H
#define Trigger_Respawning_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:     TriggerRespawning.h
//
//  Author:   Mat Buckland
//
//  Desc:     base class to create a trigger that is capable of respawning
//            after a period of inactivity
//
//-----------------------------------------------------------------------------

#include "Trigger.h"

class GameWorld;

template <class Entity>
class TriggerRespawning : public Trigger<Entity>
{
public:

	TriggerRespawning(
		GameWorld* const world,
		geometry::Shape* const bounding_shape) 
		:
		Trigger<Entity>(world, bounding_shape),
		_num_update_for_respawn(0),
		_num_update_remaining(util::genRand(0, 50))
	{}

	virtual ~TriggerRespawning() override
	{}

	//this is called each game-tick to update the trigger's internal state
	virtual void update(double time_elapsed) override
	{
		if (!isActive() && (_num_update_remaining-- <= 0))
		{
			setActive();
		}
	}

	//to be implemented by child classes
	virtual void actIfOverlap(Entity*) = 0;

	//to be implemented by child classes
	virtual void render() = 0;

	virtual bool handleMessage(const Telegram& msg) override
	{
		return true;
	}

	void setRespawnDelay(unsigned int num_ticks)
	{
		_num_update_for_respawn = num_ticks;
	}

protected:

	//When a bot comes within this trigger's area of influence it is triggered
	//but then becomes INACTIVE for a specified amount of time. These values
	//control the amount of time required to pass before the trigger becomes 
	//ACTIVE once more.
	int   _num_update_for_respawn;
	int   _num_update_remaining;
	
	//sets the trigger to be INACTIVE for _num_update_for_respawn 
	//update-steps
	void deactivate()
	{
		setInactive();
		_num_update_remaining = _num_update_for_respawn;
	}
};




#endif