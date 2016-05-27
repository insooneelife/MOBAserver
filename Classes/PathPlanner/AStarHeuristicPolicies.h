#ifndef ASTAR_HEURISTIC_POLICIES_H
#define ASTAR_HEURISTIC_POLICIES_H
//-----------------------------------------------------------------------------
//
//  Name:   AStarHeuristicPolicies.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class templates defining a heuristic policy for use with the A*
//          search algorithm
//-----------------------------------------------------------------------------
#include "cocos2d.h"
#include "../Util.h"

//-----------------------------------------------------------------------------
//the euclidian heuristic (straight-line distance)
//-----------------------------------------------------------------------------
class HeuristicEuclid
{
public:

	HeuristicEuclid() {}

	//calculate the straight line distance from node nd1 to node nd2
	template <class Graph>
	static double calculate(const Graph& graph, int nd1, int nd2)
	{
		return (graph.getNode(nd1).getPos() - graph.getNode(nd2).getPos()).getLength();
	}
};

//-----------------------------------------------------------------------------
//this uses the euclidian distance but adds in an amount of noise to the 
//result. You can use this heuristic to provide imperfect paths. This can
//be handy if you find that you frequently have lots of agents all following
//each other in single file to get from one place to another
//-----------------------------------------------------------------------------
class HeuristicNoisyEuclidian
{
public:

	HeuristicNoisyEuclidian() {}

	//calculate the straight line distance from node nd1 to node nd2
	template <class Graph>
	static double calculate(const Graph& graph, int nd1, int nd2)
	{
		return (graph.getNode(nd1).getPos() - graph.getNode(nd2).getPos()).getLength() 
			* util::genRand(0.9f, 1.1f);
	}
};

//-----------------------------------------------------------------------------
//you can use this class to turn the A* algorithm into Dijkstra's search.
//this is because Dijkstra's is equivalent to an A* search using a heuristic
//value that is always equal to zero.
//-----------------------------------------------------------------------------
class HeuristicDijkstra
{
public:

	template <class Graph>
	static double calculate(const Graph& graph, int nd1, int nd2)
	{
		return 0;
	}
};






#endif