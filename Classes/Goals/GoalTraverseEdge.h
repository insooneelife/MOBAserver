#ifndef GOAL_TRAVERSE_EDGE_H
#define GOAL_TRAVERSE_EDGE_H
#pragma warning (disable:4786)

#include "Goal.h"
#include "cocos2d.h"
#include "../PathPlanner/PathPlanner.h"
#include "../PathPlanner/PathEdge.h"

class AbstCharacter;

class GoalTraverseEdge : public Goal<AbstCharacter>
{
public:

	GoalTraverseEdge(
		AbstCharacter* bot,
		PathEdge   edge,
		float radius,
		bool       last_edge);

	virtual ~GoalTraverseEdge() override
	{}

	//the usual suspects
	virtual void activate() override;
	
	virtual int  process() override;

	virtual void terminate() override;

	virtual void render() override;

	PathEdge getEdge() const { return _edge; }

protected:

	//returns true if the bot gets stuck
	bool		isStuck() const;

	//the edge the bot will follow
	PathEdge	_edge;

	//true if _edge is the last in the path.
	bool		_last_edge_in_path;

	//the estimated time the bot should take to traverse the edge
	double		_time_expected;

	//this records the time this goal was activated
	double		_start_time;

	float _radius;
};




#endif

