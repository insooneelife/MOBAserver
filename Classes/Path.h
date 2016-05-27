#ifndef PATH_H
#define PATH_H

//------------------------------------------------------------------------
//
//  Name:   Path.h
//
//  Desc:   class to define, manage, and traverse a path (defined by a series of 2D vectors)
//          
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <list>
#include <cassert>
#include "cocos2d.h"

class Path
{
public:
	Path();

	//constructor for creating a path with initial random waypoints. MinX/Y
	//& MaxX/Y define the bounding box of the path.
	Path(int	num_waypoints,
		 double minX,
		 double minY,
		 double maxX,
		 double maxY,
		 bool   looped);


	//returns the current waypoint
	const cocos2d::Vec2&	currentWaypoint() const;

	//returns true if the end of the list has been reached
	bool					finished();

	//moves the iterator on to the next waypoint in the list
	void					setNextWaypoint();

	//creates a random path which is bound by rectangle described by
	//the min/max values
	const std::list<cocos2d::Vec2>& createRandomPath(
		int    num_waypoints,
		double minX,
		double minY,
		double maxX,
		double maxY);

	void addPath(const cocos2d::Vec2& pos);

	void loopOn();
	void loopOff();

	//methods for setting the path with either another Path or a list of vectors
	void setPath(const std::list<cocos2d::Vec2>& new_path);
	void setPath(const Path& path);

	void clear();

	const std::list<cocos2d::Vec2>& getPath() const;

private:
	std::list<cocos2d::Vec2>            _way_points;

	//points to the current waypoint
	std::list<cocos2d::Vec2>::iterator  _cur_way_point;

	//flag to indicate if the path should be looped
	//(The last waypoint connected to the first)
	bool								_looped;
};


#endif