#ifndef TIME_SLICED_GRAPHALGORITHMS_H
#define TIME_SLICED_GRAPHALGORITHMS_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   TimeSlicedGraphAlgorithms.h
//
//  Desc:   classes to implement graph algorithms that can be distributed
//          over multiple update-steps
//
//          Any graphs passed to these functions must conform to the
//          same interface used by the SparseGraph
//          
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <memory>

#include "SparseGraph.h"
#include "PriorityQueue.h"
#include "AStarHeuristicPolicies.h"
#include "SearchTerminationPolicies.h"
#include "PathEdge.h"



//these enums are used as return values from each search update method
enum { TARGET_FOUND, TARGET_NOT_FOUND, SEARCH_INCOMPLETE };



//------------------------ SearchTimeSliced -----------------------------
//
// base class to define a common interface for graph search algorithms
//-----------------------------------------------------------------------------
template <class Edge>
class SearchTimeSliced
{
public:
	enum SearchType { ASTAR, DIJKSTRA };

public:

	explicit SearchTimeSliced(SearchType type)
		:
		_search_type(type)
	{}

	virtual ~SearchTimeSliced()
	{}

	//When called, this method runs the algorithm through one search cycle. The
	//method returns an enumerated value (TARGET_FOUND, TARGET_NOT_FOUND,
	//SEARCH_INCOMPLETE) indicating the status of the search
	virtual int								cycleOnce() = 0;

	//returns the vector of edges that the algorithm has examined
	virtual std::vector<const Edge*>		getSPT() const = 0;

	//returns the total cost to the target
	virtual double							getCostToTarget() const = 0;

	//returns a list of node indexes that comprise the shortest path
	//from the source to the target
	virtual std::list<int>					getPathToTarget() const = 0;

	//returns the path as a list of PathEdges
	virtual std::list<PathEdge>				getPathAsPathEdges() const = 0;

	SearchType getType() const 
	{
		return _search_type; 
	}

private:
	SearchType _search_type;
};




//-------------------------- SearchAStarTS -----------------------------
//
//  a A* class that enables a search to be COMPLETED over multiple update-steps
//-----------------------------------------------------------------------------
template <class Graph, class HeuristicPolicy>
class SearchAStarTS : public SearchTimeSliced<typename Graph::Edge>
{
private:

	//create typedefs for the node and edge types used by the graph
	typedef typename Graph::Edge Edge;
	typedef typename Graph::Node Node;

public:

	SearchAStarTS(
		const Graph&		graph,
		int					source,
		int					target) 
		:
		SearchTimeSliced<Edge>(ASTAR),
		_graph(graph),
		_shortest_path_tree(graph.numNodes()),
		_search_frontier(graph.numNodes()),
		_g_costs(graph.numNodes(), 0.0),
		_f_costs(graph.numNodes(), 0.0),
		_source(source),
		_target(target),
		_pq(new IndexedPriorityQLow<double>(_f_costs, _graph.numNodes()))
	{
		//put the source node on the queue
		_pq->insert(_source);
	}

	virtual ~SearchAStarTS() override
	{}


	//When called, this method pops the next node off the PQ and examines all
	//its edges. The method returns an enumerated value (TARGET_FOUND,
	//TARGET_NOT_FOUND, SEARCH_INCOMPLETE) indicating the status of the search
	virtual int							cycleOnce() override;

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	virtual std::list<int>				getPathToTarget() const override;

	//returns the path as a list of PathEdges
	virtual std::list<PathEdge>			getPathAsPathEdges() const override;

	//returns the total cost to the target
	virtual double getCostToTarget() const override
	{
		return _g_costs[_target]; 
	}

	//returns the vector of edges that the algorithm has examined
	virtual std::vector<const Edge*> getSPT() const override
	{
		return _shortest_path_tree;
	}

private:

	const Graph&				_graph;

	//indexed into my node. Contains the 'real' accumulative cost to that node
	std::vector<double>			_g_costs;

	//indexed into by node. Contains the cost from adding _g_costs[n] to
	//the HeuristicPolicy cost from n to the target node. This is the vector the
	//iPQ indexes into.
	std::vector<double>			_f_costs;

	std::vector<const Edge*>	_shortest_path_tree;
	std::vector<const Edge*>	_search_frontier;

	int							_source;
	int							_target;

	//create an indexed priority queue of nodes. The nodes with the
	//lowest overall F cost (G+H) are positioned at the front.
	std::unique_ptr<IndexedPriorityQLow<double> >    _pq;
};

//-----------------------------------------------------------------------------
template <class Graph, class HeuristicPolicy>
int SearchAStarTS<Graph, HeuristicPolicy>::cycleOnce()
{
	//if the PQ is empty the target has not been found
	if (_pq->empty())
	{
		return TARGET_NOT_FOUND;
	}

	//get lowest cost node from the queue
	int next_closest_node = _pq->Pop();

	//put the node on the SPT
	_shortest_path_tree[next_closest_node] = _search_frontier[next_closest_node];

	//if the target has been found exit
	if (next_closest_node == _target)
	{
		return TARGET_FOUND;
	}

	//now to test all the edges attached to this node
	Graph::ConstEdgeIterator const_edge_iter(_graph, next_closest_node);
	for (const Edge* eg = const_edge_iter.begin();
		!const_edge_iter.end();
		eg = const_edge_iter.next())
	{
		//calculate the HeuristicPolicy cost from this node to the target (H)                       
		double h_cost = HeuristicPolicy::calculate(_graph, _target, eg->getTo());

		//calculate the 'real' cost to this node from the source (G)
		double g_cost = _g_costs[next_closest_node] + eg->getCost();

		//if the node has not been added to the frontier, add it and update
		//the G and F costs
		if (_search_frontier[eg->getTo()] == nullptr)
		{
			_f_costs[eg->getTo()] = g_cost + h_cost;
			_g_costs[eg->getTo()] = g_cost;

			_pq->insert(eg->getTo());

			_search_frontier[eg->getTo()] = eg;
		}

		//if this node is already on the frontier but the cost to get here
		//is cheaper than has been found previously, update the node
		//costs and frontier accordingly.
		else if ((g_cost < _g_costs[eg->getTo()]) && 
				(_shortest_path_tree[eg->getTo()] == nullptr))
		{
			_f_costs[eg->getTo()] = g_cost + h_cost;
			_g_costs[eg->getTo()] = g_cost;

			_pq->ChangePriority(eg->getTo());

			_search_frontier[eg->getTo()] = eg;
		}
	}

	//there are still nodes to explore
	return SEARCH_INCOMPLETE;
}

//-----------------------------------------------------------------------------
template <class Graph, class HeuristicPolicy>
std::list<int>
SearchAStarTS<Graph, HeuristicPolicy>::getPathToTarget()const
{
	std::list<int> path;

	//just return an empty path if no target or no path found
	if (_target < 0)  return path;

	int nd = _target;

	path.push_back(nd);

	while ((nd != _source) && (_shortest_path_tree[nd] != 0))
	{
		nd = _shortest_path_tree[nd]->getFrom();

		path.push_front(nd);
	}

	return path;
}


//-------------------------- getPathAsPathEdges -------------------------------
//
//  returns the path as a list of PathEdges
//-----------------------------------------------------------------------------
template <class Graph, class HeuristicPolicy>
std::list<PathEdge>
SearchAStarTS<Graph, HeuristicPolicy>::getPathAsPathEdges()const
{
	std::list<PathEdge> path;

	//just return an empty path if no target or no path found
	if (_target < 0)  return path;

	int nd = _target;

	while ((nd != _source) && (_shortest_path_tree[nd] != 0))
	{
		path.push_front(PathEdge(
			_graph.getNode(_shortest_path_tree[nd]->getFrom()).getPos(),
			_graph.getNode(_shortest_path_tree[nd]->getTo()).getPos(),
			_shortest_path_tree[nd]->getFlags(),
			_shortest_path_tree[nd]->getIDofIntersectingEntity()));

		nd = _shortest_path_tree[nd]->getFrom();
	}

	return path;
}

//-------------------------- SearchDijkstrasTS -------------------------
//
//  Dijkstra's algorithm class modified to spread a search over multiple
//  update-steps
//-----------------------------------------------------------------------------
template <class Graph, class TerminationCondition>
class SearchDijkstrasTS 
	: public SearchTimeSliced<typename Graph::Edge>
{
private:

	//create typedefs for the node and edge types used by the graph
	typedef typename Graph::Edge Edge;
	typedef typename Graph::Node Node;

public:

	SearchDijkstrasTS(
		const Graph&	graph,
		int				source,
		int				target) 
		:
		SearchTimeSliced<Edge>(DIJKSTRA),
		_graph(graph),
		_shortest_path_tree(graph.numNodes()),
		_search_frontier(graph.numNodes()),
		_cost_to_this_node(graph.numNodes(), 0.0),
		_source(source),
		_target(target),
		_pq(new IndexedPriorityQLow<double>(_cost_to_this_node, _graph.numNodes()))
	{
		//put the source node on the queue
		_pq->insert(_source);
	}

	//let the search class take care of tidying up memory (the wary amongst
	//you may prefer to use std::auto_ptr or similar to replace the pointer
	//to the termination condition)
	virtual ~SearchDijkstrasTS() override
	{}

	//When called, this method pops the next node off the PQ and examines all
	//its edges. The method returns an enumerated value (TARGET_FOUND,
	//TARGET_NOT_FOUND, SEARCH_INCOMPLETE) indicating the status of the search
	virtual int							cycleOnce() override;

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	virtual std::list<int>				getPathToTarget() const override;

	//returns the path as a list of PathEdges
	virtual std::list<PathEdge>			getPathAsPathEdges() const override;

	//returns the total cost to the target
	virtual double getCostToTarget() const override
	{
		return _cost_to_this_node[_target]; 
	}

	//returns the vector of edges that the algorithm has examined
	virtual std::vector<const Edge*> getSPT() const override
	{
		return _shortest_path_tree; 
	}


private:

	const Graph&					_graph;

	//indexed into my node. Contains the accumulative cost to that node
	std::vector<double>				_cost_to_this_node;

	std::vector<const Edge*>		_shortest_path_tree;
	std::vector<const Edge*>		_search_frontier;

	int								_source;
	int								_target;

	//create an indexed priority queue of nodes. The nodes with the
	//lowest overall F cost (G+H) are positioned at the front.
	std::unique_ptr<IndexedPriorityQLow<double> >     _pq;
};

//-----------------------------------------------------------------------------
template <class Graph, class TerminationCondition>
int SearchDijkstrasTS<Graph, TerminationCondition>::cycleOnce()
{
	//if the PQ is empty the target has not been found
	if (_pq->empty())
	{
		return TARGET_NOT_FOUND;
	}

	//get lowest cost node from the queue
	int next_closest_node = _pq->Pop();

	//move this node from the frontier to the spanning tree
	_shortest_path_tree[next_closest_node] = _search_frontier[next_closest_node];

	//if the target has been found exit
	if (TerminationCondition::isSatisfied(_graph, _target, next_closest_node))
	{
		//make a note of the node index that has satisfied the condition. This
		//is so we can work backwards from the index to extract the path from
		//the shortest path tree.
		_target = next_closest_node;

		return TARGET_FOUND;
	}

	//now to test all the edges attached to this node
	Graph::ConstEdgeIterator const_edge_iter(_graph, next_closest_node);
	for (const Edge* eg = const_edge_iter.begin();
	!const_edge_iter.end();
		eg = const_edge_iter.next())
	{
		//the total cost to the node this edge points to is the cost to the
		//current node plus the cost of the edge connecting them.
		double new_cost = _cost_to_this_node[next_closest_node] + eg->getCost();

		//if this edge has never been on the frontier make a note of the cost
		//to get to the node it points to, then add the edge to the frontier
		//and the destination node to the PQ.
		if (_search_frontier[eg->getTo()] == 0)
		{
			_cost_to_this_node[eg->getTo()] = new_cost;

			_pq->insert(eg->getTo());

			_search_frontier[eg->getTo()] = eg;
		}

		//else test to see if the cost to reach the destination node via the
		//current node is cheaper than the cheapest cost found so far. If
		//this path is cheaper, we assign the new cost to the destination
		//node, update its entry in the PQ to reflect the change and add the
		//edge to the frontier
		else if ((new_cost < _cost_to_this_node[eg->getTo()]) &&
			(_shortest_path_tree[eg->getTo()] == 0))
		{
			_cost_to_this_node[eg->getTo()] = new_cost;

			//because the cost is less than it was previously, the PQ must be
			//re-sorted to account for this.
			_pq->ChangePriority(eg->getTo());

			_search_frontier[eg->getTo()] = eg;
		}
	}

	//there are still nodes to explore
	return SEARCH_INCOMPLETE;
}

//-----------------------------------------------------------------------------
template <class Graph, class TerminationCondition>
std::list<int>
SearchDijkstrasTS<Graph, TerminationCondition>::getPathToTarget()const
{
	std::list<int> path;

	//just return an empty path if no target or no path found
	if (_target < 0)  return path;

	int nd = _target;

	path.push_back(nd);

	while ((nd != _source) && (_shortest_path_tree[nd] != 0))
	{
		nd = _shortest_path_tree[nd]->getFrom();

		path.push_front(nd);
	}

	return path;
}


//-------------------------- getPathAsPathEdges -------------------------------
//
//  returns the path as a list of PathEdges
//-----------------------------------------------------------------------------
template <class Graph, class TerminationCondition>
std::list<PathEdge>
SearchDijkstrasTS<Graph, TerminationCondition>::getPathAsPathEdges()const
{
	std::list<PathEdge> path;

	//just return an empty path if no target or no path found
	if (_target < 0)  return path;

	int nd = _target;

	while ((nd != _source) && (_shortest_path_tree[nd] != 0))
	{
		path.push_front(PathEdge(
			_graph.getNode(_shortest_path_tree[nd]->getFrom()).getPos(),
			_graph.getNode(_shortest_path_tree[nd]->getTo()).getPos(),
			_shortest_path_tree[nd]->getFlags(),
			_shortest_path_tree[nd]->getIDofIntersectingEntity()));

		nd = _shortest_path_tree[nd]->getFrom();
	}

	return path;
}

#endif