#ifndef PATHPLANNER_H
#define PATHPLANNER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   PathPlanner.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to handle the creation of paths through a navigation graph
//-----------------------------------------------------------------------------
#include <list>
#include <memory>
#include "cocos2d.h"
#include "TimeSlicedGraphAlgorithms.h"
#include "GraphAlgorithms.h"
#include "SparseGraph.h"
#include "PathEdge.h"
#include "../GameMap.h"


class AbstCharacter;

class PathPlanner
{
public:
	//for ease of use typdef the graph edge/node types used by the navgraph
	typedef GameMap::NavGraph::Edge		Edge;
	typedef GameMap::NavGraph::Node		Node;
	typedef std::list<PathEdge>			Path;

public:

	~PathPlanner();

	explicit PathPlanner(AbstCharacter* const owner);

	//returns the index of the closest visible and unobstructed graph node to
	//the given position [CR: made public]
	int		getClosestNodeToPosition(cocos2d::Vec2 pos)const;

	//creates an instance of the A* time-sliced search and registers it with
	//the path manager
	bool	requestPathToItem(unsigned int item_type);

	//creates an instance of the Dijkstra's time-sliced search and registers 
	//it with the path manager
	bool	requestPathToPosition(cocos2d::Vec2 target_pos);

	//called by an agent after it has been notified that a search has terminated
	//successfully. The method extracts the path from _current_algorithm, adds
	//additional edges appropriate to the search type and returns it as a list of
	//PathEdges.
	Path	getPath();

	//the path manager calls this to iterate once though the search cycle
	//of the currently assigned search algorithm. When a search is terminated
	//the method messages the owner with either the msg_NoPathAvailable or
	//msg_PathReady messages
	int		cycleOnce();

	cocos2d::Vec2	getDestination()const { return _destination_pos; }
	void			setDestination(cocos2d::Vec2 new_pos) { _destination_pos = new_pos; }

	//used to retrieve the position of a graph node from its index. (takes
	//into account the enumerations 'non_graph_source_node' and 
	//'non_graph_target_node'
	cocos2d::Vec2	getNodePosition(int idx)const;

	LARGE_INTEGER	getTime() { return _time; }


private:

	//smooths a path by removing extraneous edges. (may not remove all
	//extraneous edges)
	void  smoothPathEdgesQuick(Path& path);

	//smooths a path by removing extraneous edges. (removes *all* extraneous
	//edges)
	void  smoothPathEdgesPrecise(Path& path);

	//called at the commencement of a new search request. It clears up the 
	//appropriate lists and memory in preparation for a new search request
	void  getReadyForNewSearch();


	//for legibility
	enum { NO_CLOSEST_NODE_FOUND = -1 };

	//A pointer to the owner of this class, only references.
	AbstCharacter* const				_owner;

	//a reference to the navgraph
	const GameMap::NavGraph&			_graph;

	//a pointer to an instance of the current graph search algorithm.
	std::unique_ptr<SearchTimeSliced<Edge> >	_current_algorithm;

	//this is the position the bot wishes to plan a path to reach
	cocos2d::Vec2						_destination_pos;

	LARGE_INTEGER						_time;
};


#endif

