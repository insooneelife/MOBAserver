#ifndef PATHEDGE_H
#define PATHEDGE_H
//-----------------------------------------------------------------------------
//
//  Name:   PathEdge.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   class to represent a path edge. This path can be used by a path
//          planner in the creation of paths. 
//
//-----------------------------------------------------------------------------
#include "cocos2d.h"

class PathEdge
{
public:

	PathEdge(
		cocos2d::Vec2 source,
		cocos2d::Vec2 destination,
		int behavior = 0,
		int door_id = 0)
		:
		_source(source),
		_destination(destination),
		_behavior(behavior),
		_door_id(door_id)
	{}

	cocos2d::Vec2 getDestination() const
	{
		return _destination;
	}

	void setDestination(cocos2d::Vec2 new_dest)
	{
		_destination = new_dest;
	}

	cocos2d::Vec2 getSource() const
	{
		return _source;
	}

	void setSource(cocos2d::Vec2 new_source)
	{
		_source = new_source;
	}

	int getDoorID() const
	{
		return _door_id;
	}

	int getBehavior() const
	{
		return _behavior;
	}

private:

	//positions of the source and destination nodes this edge connects
	cocos2d::Vec2 _source;
	cocos2d::Vec2 _destination;

	//the behavior associated with traversing this edge
	int      _behavior;

	int      _door_id;
};


#endif
