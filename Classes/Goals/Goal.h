#ifndef GOAL_H
#define GOAL_H
//-----------------------------------------------------------------------------
//
//  Name:   Goal.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Base goal class.
//-----------------------------------------------------------------------------

#include "cocos2d.h"
struct Telegram;

template <class Entity>
class Goal
{
public:

	enum { ACTIVE, INACTIVE, COMPLETED, FAILED };

public:

	//note how goals start off in the INACTIVE state
	Goal(Entity* const entity, int type) 
		:
		_type(type),
		_owner(entity),
		_status(INACTIVE)
	{}

	virtual ~Goal() {}

	//logic to run when the goal is activated.
	virtual void activate() = 0;

	//logic to run each update-step
	virtual int  process() = 0;

	//logic to run when the goal is satisfied. 
	//(typically used to switch off any active steering behaviors)
	virtual void terminate() = 0;

	//goals can handle messages. Many don't though, so this defines a default
	//behavior
	virtual bool handleMessage(const Telegram& msg) { return false; }

	//used to render any goal specific information
	virtual void render() {}

	//a Goal is atomic and cannot aggregate subgoals yet we must implement
	//this method to provide the uniform interface required for the goal
	//hierarchy.
	virtual void addSubgoal(Goal<Entity>* g)
	{
		throw std::runtime_error("Cannot add goals to atomic goals");
	}

	bool         isComplete() const	{ return _status == COMPLETED; }
	bool         isActive() const	{ return _status == ACTIVE; }
	bool         isInactive() const	{ return _status == INACTIVE; }
	bool         hasFailed() const	{ return _status == FAILED; }
	int          getType() const	{ return _type; }

protected:

	//an enumerated type specifying the type of goal
	int					_type;

	//a pointer to the entity that owns this goal
	Entity* const		_owner;

	//an enumerated value indicating the goal's status (ACTIVE, INACTIVE,
	//COMPLETED, FAILED)
	int					_status;


	/* the following methods were created to factor out some of the commonality
	in the implementations of the process method() */

	//if _status = INACTIVE this method sets it to ACTIVE and calls activate()
	void activateIfInactive();

	//if _status is FAILED this method sets it to INACTIVE so that the goal
	//will be reactivated (and therefore re-planned) on the next update-step.
	void reactivateIfFailed();
};


//if _status is FAILED this method sets it to INACTIVE so that the goal
//will be reactivated (replanned) on the next update-step.
template <class Entity>
void  Goal<Entity>::reactivateIfFailed()
{
	if (hasFailed())
	{
		_status = INACTIVE;
	}
}


template <class Entity>
void  Goal<Entity>::activateIfInactive()
{
	if (isInactive())
	{
		activate();
	}
}


#endif