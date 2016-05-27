#ifndef GOAL_COMPOSITE_H
#define GOAL_COMPOSITE_H
//-----------------------------------------------------------------------------
//
//  Name:   GoalComposite.h      
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Base composite goal class
//-----------------------------------------------------------------------------
#include <list>
#include "Goal.h"


template <class Entity>
class GoalComposite : public Goal<Entity>
{
private:

	typedef std::list<Goal<Entity>* > SubgoalList;

public:

	GoalComposite(Entity* const entity, int type) 
		:
		Goal<Entity>(entity, type) 
	{}

	//when this object is destroyed make sure any subgoals are terminated
	//and destroyed.
	virtual ~GoalComposite() override
	{
		removeAllSubgoals(); 
	}

	//logic to run when the goal is activated.
	virtual void activate() = 0;

	//logic to run each update-step.
	virtual int process() = 0;

	//logic to run prior to the goal's destruction
	virtual void terminate() = 0;


	//if a child class of GoalComposite does not define a message handler
	//the default behavior is to forward the message to the front-most
	//subgoal
	virtual bool handleMessage(const Telegram& msg) override
	{
		return forwardMessageToFrontMostSubgoal(msg);
	}

	//this is only used to render information for debugging purposes
	virtual void render()  override;

	//adds a subgoal to the front of the subgoal list
	void         addSubgoal(Goal<Entity>* g);

	//this method iterates through the subgoals and calls each one's terminate
	//method before deleting the subgoal and removing it from the subgoal list
	void         removeAllSubgoals();

protected:

	//composite goals may have any number of subgoals
	SubgoalList   _subgoals;


	//processes any subgoals that may be present
	int  processSubgoals();

	//passes the message to the front-most subgoal
	bool forwardMessageToFrontMostSubgoal(const Telegram& msg);
};





//---------------------- removeAllSubgoals ------------------------------------
//-----------------------------------------------------------------------------
template <class Entity>
void GoalComposite<Entity>::removeAllSubgoals()
{
	for (SubgoalList::iterator it = std::begin(_subgoals);
		 it != std::end(_subgoals);
		 ++it)
	{
		(*it)->terminate();

		delete *it;
	}

	_subgoals.clear();
}


//-------------------------- ProcessSubGoals ----------------------------------
//
//  this method first removes any COMPLETED goals from the front of the
//  subgoal list. It then processes the next goal in the list (if there is one)
//-----------------------------------------------------------------------------
template <class Entity>
int GoalComposite<Entity>::processSubgoals()
{
	//remove all COMPLETED and FAILED goals from the front of the subgoal list
	while (!_subgoals.empty() &&
		(_subgoals.front()->isComplete() || _subgoals.front()->hasFailed()))
	{
		_subgoals.front()->terminate();
		delete _subgoals.front();
		_subgoals.pop_front();
	}

	//if any subgoals remain, process the one at the front of the list
	if (!_subgoals.empty())
	{
		//grab the status of the front-most subgoal
		int status_of_subgoals = _subgoals.front()->process();

		//we have to test for the special case where the front-most subgoal
		//reports 'COMPLETED' *and* the subgoal list contains additional goals.When
		//this is the case, to ensure the object keeps processing its subgoal list
		//we must return the 'ACTIVE' status.
		if (status_of_subgoals == COMPLETED && _subgoals.size() > 1)
		{
			return ACTIVE;
		}

		return status_of_subgoals;
	}

	//no more subgoals to process - return 'COMPLETED'
	else
	{
		return COMPLETED;
	}
}

//----------------------------- addSubgoal ------------------------------------
template <class Entity>
void GoalComposite<Entity>::addSubgoal(Goal<Entity>* g)
{
	//add the new goal to the front of the list
	_subgoals.push_front(g);
}



//---------------- forwardMessageToFrontMostSubgoal ---------------------------
//
//  passes the message to the goal at the front of the queue
//-----------------------------------------------------------------------------
template <class Entity>
bool GoalComposite<Entity>::forwardMessageToFrontMostSubgoal(const Telegram& msg)
{
	if (!_subgoals.empty())
	{
		return _subgoals.front()->handleMessage(msg);
	}

	//return false if the message has not been handled
	return false;
}


template <class Entity>
void  GoalComposite<Entity>::render()
{
	if (!_subgoals.empty())
	{
		_subgoals.front()->render();
	}
}



#endif

