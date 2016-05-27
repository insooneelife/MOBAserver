#ifndef GOAL_FOLLOWPATH_H
#define GOAL_FOLLOWPATH_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GoalFollowPath.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:
//-----------------------------------------------------------------------------
#include "Goals/GoalComposite.h"
#include "../GlobalValues.h"
#include "../AbstCharacter.h"
#include "../PathPlanner/PathPlanner.h"
#include "../PathPlanner/PathEdge.h"



class GoalFollowPath : public GoalComposite<AbstCharacter>
{
public:

	GoalFollowPath(
		AbstCharacter* const bot,
		std::list<PathEdge> path,
		float radius = 10);	

	virtual ~GoalFollowPath() override
	{}

	//the usual suspects
	virtual void	activate() override;

	virtual int		process() override;

	virtual void	terminate() override;

	virtual void	render() override;

protected:

	//a local copy of the path returned by the path planner
	std::list<PathEdge>  _path;

	float _radius;
};

#endif

