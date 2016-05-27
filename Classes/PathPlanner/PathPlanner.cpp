#include "cocos2d.h"
#include "PathPlanner.h"
#include "../AbstCharacter.h"
#include "../GameWorld.h"
#include "../Util.h"
#include "../Message/MessageDispatcher.h"
#include "../GlobalValues.h"
#include "AStarHeuristicPolicies.h"

#include "GraphAlgorithms.h"

#include "PathManager.h"
#include "SearchTerminationPolicies.h"


//#define SHOW_NAVINFO

USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
PathPlanner::PathPlanner(AbstCharacter* const owner)
	:
	_owner(owner),
	_graph(*_owner->getWorld()->getGameMap()->getNavGraph()),
	_current_algorithm(nullptr)
{}

//-------------------------- dtor ---------------------------------------------
//-----------------------------------------------------------------------------
PathPlanner::~PathPlanner()
{
	getReadyForNewSearch();
#ifdef SHOW_DTOR_INFO
	CCLOG("~PathManager()");
#endif
}

//------------------------------ getReadyForNewSearch -----------------------------------
//
//  called by the search manager when a search has been terminated to free
//  up the memory used when an instance of the search was created
//-----------------------------------------------------------------------------
void PathPlanner::getReadyForNewSearch()
{
	//unregister any existing search with the path manager
	_owner->getWorld()->getPathManager()->unRegisterPath(this);

	//clean up memory used by any existing search
	_current_algorithm.reset();
	_current_algorithm = 0;
	_time.QuadPart = 0I64;
}


//----------------------------- getPath ------------------------------------
//
//  called by an agent after it has been notified that a search has terminated
//  successfully. The method extracts the path from _current_algorithm, adds
//  additional edges appropriate to the search type and returns it as a list of
//  PathEdges.
//-----------------------------------------------------------------------------
PathPlanner::Path PathPlanner::getPath()
{
	CCASSERT(_current_algorithm, "no current search");

	LARGE_INTEGER entry_time;
	LARGE_INTEGER exit_time;
	QueryPerformanceCounter(&entry_time);

	Path path = _current_algorithm->getPathAsPathEdges();

	if (path.size() <= 0)
	{
		CCLOG("path is 0");
		return path;
	}

	int closest = getClosestNodeToPosition(_owner->getPos());

	if (closest != -1)
	{
		path.push_front(PathEdge(
			_owner->getPos(),
			getNodePosition(closest),
			NavGraphEdge::NORMAL));
	}
	else
	{
		CCLOG("cannot find closest node!");
	}

	//if the bot requested a path to a location then an edge leading to the
	//destination must be added
	if (_current_algorithm->getType() == SearchTimeSliced<Edge>::ASTAR)
	{
		path.push_back(PathEdge(path.back().getDestination(),
			_destination_pos,
			NavGraphEdge::NORMAL));
	}

	//smooth paths if required
	if (true)
	{
		smoothPathEdgesQuick(path);
	}

	if (false)
	{
		smoothPathEdgesPrecise(path);
	}

	QueryPerformanceCounter(&exit_time);
	_time.QuadPart += (exit_time.QuadPart - entry_time.QuadPart);

	return path;
}

//--------------------------- smoothPathEdgesQuick ----------------------------
//
//  smooths a path by removing extraneous edges.
//-----------------------------------------------------------------------------
void PathPlanner::smoothPathEdgesQuick(Path& path)
{
	//create a couple of iterators and point them at the front of the path
	Path::iterator e1(path.begin()), e2(path.begin());

	//increment e2 so it points to the edge following e1.
	++e2;

	//while e2 is not the last edge in the path, step through the edges checking
	//to see if the agent can move without obstruction from the source node of
	//e1 to the destination node of e2. If the agent can move between those 
	//positions then the two edges are replaced with a single edge.
	while (e2 != path.end())
	{
		//check for obstruction, adjust and remove the edges accordingly
		if ((e2->getBehavior() == Edge::NORMAL) &&
			_owner->canWalkBetween(e1->getSource(), e2->getDestination()))
		{
			e1->setDestination(e2->getDestination());
			e2 = path.erase(e2);
		}
		else
		{
			e1 = e2;
			++e2;
		}
	}
}


//----------------------- smoothPathEdgesPrecise ---------------------------------
//
//  smooths a path by removing extraneous edges.
//-----------------------------------------------------------------------------
void PathPlanner::smoothPathEdgesPrecise(Path& path)
{
	//create a couple of iterators
	Path::iterator e1, e2;

	//point e1 to the beginning of the path
	e1 = path.begin();

	while (e1 != path.end())
	{
		//point e2 to the edge immediately following e1
		e2 = e1;
		++e2;

		//while e2 is not the last edge in the path, step through the edges
		//checking to see if the agent can move without obstruction from the 
		//source node of e1 to the destination node of e2. If the agent can move
		//between those positions then the any edges between e1 and e2 are
		//replaced with a single edge.
		while (e2 != path.end())
		{
			//check for obstruction, adjust and remove the edges accordingly
			if ((e2->getBehavior() == Edge::NORMAL) &&
				_owner->canWalkBetween(e1->getSource(), e2->getDestination()))
			{
				e1->setDestination(e2->getDestination());
				e2 = path.erase(++e1, ++e2);
				e1 = e2;
				--e1;
			}

			else
			{
				++e2;
			}
		}

		++e1;
	}
}



//---------------------------- cycleOnce --------------------------------------
//
//  the path manager calls this to iterate once though the search cycle
//  of the currently assigned search algorithm.
//-----------------------------------------------------------------------------
int PathPlanner::cycleOnce()
{
	CCASSERT(_current_algorithm, "No search object instantiated");

	LARGE_INTEGER entry_time;
	LARGE_INTEGER exit_time;
	QueryPerformanceCounter(&entry_time);

	int result = _current_algorithm->cycleOnce();

	QueryPerformanceCounter(&exit_time);
	_time.QuadPart += (exit_time.QuadPart - entry_time.QuadPart);

	//let the bot know of the failure to find a path
	if (result == TARGET_NOT_FOUND)
	{
		Dispatcher.dispatchMsg(
			SEND_MSG_IMMEDIATELY,
			SENDER_ID_IRRELEVANT,
			_owner->getId(),
			MSG_NOPATH_AVAILABLE,
			NO_ADDITIONAL_INFO);
	}

	//let the bot know a path has been found
	else if (result == TARGET_FOUND)
	{
		//if the search was for an item type then the final node in the path will
		//represent a giver trigger. Consequently, it's gold_worth passing the pointer
		//to the trigger in the extra info field of the message. (The pointer
		//will just be nullptr if no trigger)
		void* trigger =
			_graph.getNode(_current_algorithm->getPathToTarget().back()).getExtraInfo();

		Dispatcher.dispatchMsg(
			SEND_MSG_IMMEDIATELY,
			SENDER_ID_IRRELEVANT,
			_owner->getId(),
			MSG_PATH_READY,
			trigger);
	}
	_owner->addTime(getTime());
	return result;
}

//------------------------ getClosestNodeToPosition ---------------------------
//
//  returns the index of the closest visible graph node to the given position
//-----------------------------------------------------------------------------
int PathPlanner::getClosestNodeToPosition(cocos2d::Vec2 pos)const
{
	double closest_so_far = std::numeric_limits<double>::max();
	int   closest_node = NO_CLOSEST_NODE_FOUND;

	//when the cell space is queried this the the range searched for neighboring
	//graph nodes. This value is inversely proportional to the density of a 
	//navigation graph (less dense = bigger values)
	const double range = _owner->getWorld()->getGameMap()->getCellSpaceNeighborhoodRange();

	//calculate the graph nodes that are neighboring this position
	_owner->getWorld()->getGameMap()->
		getGraphCellSpace()->calculateNeighborsForSmall(
			pos, &geometry::Circle(pos, range));

	//iterate through the neighbors and sum up all the position vectors
	for (Node* node = _owner->getWorld()->getGameMap()->getGraphCellSpace()->begin();
	!_owner->getWorld()->getGameMap()->getGraphCellSpace()->end();
		node = _owner->getWorld()->getGameMap()->getGraphCellSpace()->next())
	{
		//if the path between this node and pos is unobstructed calculate the
		//distance
		if (_owner->canWalkBetween(pos, node->getPos()))
		{
			double dist = (pos - node->getPos()).getLengthSq();

			//keep a record of the closest so far
			if (dist < closest_so_far)
			{
				closest_so_far = dist;
				closest_node = node->getIndex();
			}
		}
	}

	return closest_node;
}

//--------------------------- requestPathToPosition ------------------------------
//
//  Given a target, this method first determines if nodes can be reached from 
//  the  bot's current position and the target position. If either end point
//  is unreachable the method returns false. 
//
//  If nodes are reachable from both positions then an instance of the time-
//  sliced A* search is created and registered with the search manager. the
//  method then returns true.
//        
//-----------------------------------------------------------------------------
bool PathPlanner::requestPathToPosition(cocos2d::Vec2 target_pos)
{
	getReadyForNewSearch();

	//make a note of the target position.
	_destination_pos = target_pos;

	//if the target is walkable from the bot's position a path does not need to
	//be calculated, the bot can go straight to the position by ARRIVING at
	//the current waypoint
	if (_owner->canWalkTo(target_pos))
	{
		return true;
	}

	//find the closest visible node to the bots position
	int closest_node_to_bot = getClosestNodeToPosition(_owner->getPos());

	//remove the destination node from the list and return false if no visible
	//node found. This will occur if the navgraph is badly designed or if the bot
	//has managed to get itself *inside* the geometry (surrounded by walls),
	//or an obstacle. 
	if (closest_node_to_bot == NO_CLOSEST_NODE_FOUND)
	{
#ifdef SHOW_NAVINFO
		CCLOG("No closest node to bot found! (1)");
#endif

		return false;
	}

#ifdef SHOW_NAVINFO
	CCLOG("Closest node to bot is %d", closest_node_to_bot);
#endif

	//find the closest visible node to the target position
	int closest_node_to_target = getClosestNodeToPosition(target_pos);


	//return false if there is a problem locating a visible node from the target.
	//This sort of thing occurs much more frequently than the above. For
	//example, if the user clicks inside an area bounded by walls or inside an
	//object.

	if (closest_node_to_target == NO_CLOSEST_NODE_FOUND)
	{
		_destination_pos = _owner->getWorld()->closestPosToDestination(
			_owner->getPos(),
			target_pos,
			_owner->getBRadius());
		closest_node_to_target = getClosestNodeToPosition(_destination_pos);

#ifdef SHOW_NAVINFO
		CCLOG("sub destination : %f %f", _destination_pos.x, _destination_pos.y);
#endif
	}

	if (closest_node_to_target == NO_CLOSEST_NODE_FOUND)
	{
#ifdef SHOW_NAVINFO
		CCLOG("No closest node to target (%d) found! (2)", closest_node_to_target);
#endif

		return false;
	}

#ifdef SHOW_NAVINFO
	CCLOG("Closest node to target is %d", closest_node_to_target);
#endif

	//create an instance of a the distributed A* search class
	typedef SearchAStarTS<GameMap::NavGraph, HeuristicEuclid> AStar;

	_current_algorithm.reset(
		new AStar(_graph,
			closest_node_to_bot,
			closest_node_to_target));

	//and register the search with the path manager
	_owner->getWorld()->getPathManager()->registerPath(this);

	return true;
}


//------------------------------ requestPathToItem -----------------------------
//
// Given an item type, this method determines the closest reachable graph node
// to the bot's position and then creates a instance of the time-sliced 
// Dijkstra's algorithm, which it registers with the search manager
//
//-----------------------------------------------------------------------------
bool PathPlanner::requestPathToItem(unsigned int item_type)
{
	//clear the waypoint list and delete any ACTIVE search
	getReadyForNewSearch();

	//find the closest visible node to the bots position
	int closest_node_to_bot = getClosestNodeToPosition(_owner->getPos());

	//remove the destination node from the list and return false if no visible
	//node found. This will occur if the navgraph is badly designed or if the bot
	//has managed to get itself *inside* the geometry (surrounded by walls),
	//or an obstacle
	if (closest_node_to_bot == NO_CLOSEST_NODE_FOUND)
	{
#ifdef SHOW_NAVINFO
		CCLOG("No closest node to bot found!");
#endif

		return false;
}

	//create an instance of the search algorithm
	typedef FindActiveTrigger<Trigger<AbstCharacter> > t_con;
	typedef SearchDijkstrasTS<GameMap::NavGraph, t_con> DijSearch;

	_current_algorithm.reset(new DijSearch(_graph, closest_node_to_bot, item_type));

	//register the search with the path manager
	_owner->getWorld()->getPathManager()->registerPath(this);

	return true;
}

//------------------------------ getNodePosition ------------------------------
//
//  used to retrieve the position of a graph node from its index. (takes
//  into account the enumerations 'non_graph_source_node' and 
//  'non_graph_target_node'
//----------------------------------------------------------------------------- 
Vec2 PathPlanner::getNodePosition(int idx)const
{
	return _graph.getNode(idx).getPos();
}




