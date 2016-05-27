#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   GraphSearches.h
//
//  Desc:   classes to implement graph algorithms, including DFS, BFS,
//          Dijkstra's, A*, Prims etc. (based upon the code created
//          by Robert Sedgewick in his book "Algorithms in C++")
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

#include "SparseGraph.h"
#include "PriorityQueue.h"


//----------------------------- SearchDFS -------------------------------
//
//  class to implement a depth first search. 
//-----------------------------------------------------------------------------
template<class Graph>
class SearchDFS
{
private:

	//to aid legibility
	enum { VISITED, UNVISITED, NO_PARENT_ASSIGNED };
	
	//create a typedef for the edge and node types used by the graph
	typedef typename Graph::Edge Edge;
	typedef typename Graph::Node Node;

public:

	SearchDFS(
		const Graph&	graph,
		int				source,
		int				target = -1) 
		:
		_graph(graph),
		_source(source),
		_target(target),
		_found(false),
		_visited(_graph.numNodes(), UNVISITED),
		_route(_graph.numNodes(), NO_PARENT_ASSIGNED)

	{
		_found = search();
	}


	//returns a vector containing pointers to all the edges the search has examined
	std::vector<const Edge*> getSearchTree() const { return _spanning_tree; }

	//returns true if the target node has been located
	bool   found() const { return _found; }

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	std::list<int> getPathToTarget()const;

private:

	//this method performs the DFS search
	bool search();

	//a reference to the graph to be searched
	const Graph&		_graph;

	//this records the indexes of all the nodes that are VISITED as the
	//search progresses
	std::vector<int>	_visited;

	//this holds the route taken to the target. Given a node index, the value
	//at that index is the node's object. ie if the path to the target is
	//3-8-27, then _route[8] will hold 3 and _route[27] will hold 8.
	std::vector<int>	_route;

	//As the search progresses, this will hold all the edges the algorithm has
	//examined. THIS IS NOT NECESSARY FOR THE SEARCH, IT IS HERE PURELY
	//TO PROVIDE THE USER WITH SOME VISUAL FEEDBACK
	std::vector<const Edge*>  _spanning_tree;

	//the source and target node indices
	int					_source;
	int					_target;

	//true if a path to the target has been found
	bool				_found;
};

//-----------------------------------------------------------------------------
template <class Graph>
bool SearchDFS<Graph>::search()
{
	//create a std stack of edges
	std::stack<const Edge*> stack;

	//create a dummy edge and put on the stack
	Edge dummy(_source, _source, 0);

	stack.push(&dummy);

	//while there are edges in the stack keep searching
	while (!stack.empty())
	{
		//grab the next edge
		const Edge* next = stack.top();

		//remove the edge from the stack
		stack.pop();

		//make a note of the object of the node this edge points to
		_route[next->getTo()] = next->getFrom();

		//put it on the tree. (making sure the dummy edge is not placed on the tree)
		if (next != &dummy)
		{
			_spanning_tree.push_back(next);
		}

		//and mark it VISITED
		_visited[next->getTo()] = VISITED;

		//if the target has been found the method can return success
		if (next->getTo() == _target)
		{
			return true;
		}

		//push the edges leading from the node this edge points to onto
		//the stack (provided the edge does not point to a previously 
		//VISITED node)
		Graph::ConstEdgeIterator const_edge_iter(_graph, next->getTo());

		for (const Edge* eg = const_edge_iter.begin();
			!const_edge_iter.end();
			eg = const_edge_iter.next())
		{
			if (_visited[eg->getTo()] == UNVISITED)
			{
				stack.push(eg);
			}
		}
	}

	//no path to target
	return false;
}

//-----------------------------------------------------------------------------
template <class Graph>
std::list<int> SearchDFS<Graph>::getPathToTarget()const
{
	std::list<int> path;

	//just return an empty path if no path to target found or if
	//no target has been specified
	if (!_found || _target < 0) return path;

	int nd = _target;

	path.push_front(nd);

	while (nd != _source)
	{
		nd = _route[nd];

		path.push_front(nd);
	}

	return path;
}



//----------------------------- SearchBFS -------------------------------
//
//-----------------------------------------------------------------------------
template<class Graph>
class SearchBFS
{
private:

	//to aid legibility
	enum { VISITED, UNVISITED, NO_PARENT_ASSIGNED };

	//create a typedef for the edge type used by the graph
	typedef typename Graph::Edge Edge;

private:

	//a reference to the graph to be searched
	const Graph&		_graph;

	//this records the indexes of all the nodes that are VISITED as the
	//search progresses
	std::vector<int>	_visited;

	//this holds the route taken to the target. Given a node index, the value
	//at that index is the node's object. ie if the path to the target is
	//3-8-27, then _route[8] will hold 3 and _route[27] will hold 8.
	std::vector<int>	_route;

	//the source and target node indices
	int					_source;
	int					_target;

	//true if a path to the target has been found
	bool				_found;

	//As the search progresses, this will hold all the edges the algorithm has
	//examined. THIS IS NOT NECESSARY FOR THE SEARCH, IT IS HERE PURELY
	//TO PROVIDE THE USER WITH SOME VISUAL FEEDBACK
	std::vector<const Edge*>	_spanning_tree;


	//the BFS algorithm is very similar to the DFS except that it uses a
	//FIFO queue instead of a stack.
	bool search();


public:

	SearchBFS(
		const Graph&		graph,
		int					source,
		int					target = -1) 
		:
		_graph(graph),
		_source(source),
		_target(target),
		_found(false),
		_visited(_graph.numNodes(), UNVISITED),
		_route(_graph.numNodes(), NO_PARENT_ASSIGNED)

	{
		_found = search();
	}

	bool   found() const { return _found; }

	//returns a vector containing pointers to all the edges the search has examined
	std::vector<const Edge*> getSearchTree() const { return _spanning_tree; }

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	std::list<int> getPathToTarget() const;
};

//-----------------------------------------------------------------------------

template <class Graph>
bool SearchBFS<Graph>::search()
{
	//create a std queue of edges
	std::queue<const Edge*> que;

	const Edge dummy(_source, _source, 0);

	//create a dummy edge and put on the queue
	que.push(&dummy);

	//mark the source node as VISITED
	_visited[_source] = VISITED;

	//while there are edges in the queue keep searching
	while (!que.empty())
	{
		//grab the next edge
		const Edge* next = que.front();

		que.pop();

		//mark the object of this node
		_route[next->getTo()] = next->getFrom();

		//put it on the tree. (making sure the dummy edge is not placed on the tree)
		if (next != &dummy)
		{
			_spanning_tree.push_back(next);
		}

		//exit if the target has been found
		if (next->getTo() == _target)
		{
			return true;
		}

		//push the edges leading from the node at the end of this edge 
		//onto the queue
		Graph::ConstEdgeIterator const_edge_iter(_graph, next->getTo());

		for (const Edge* eg = const_edge_iter.begin();
		!const_edge_iter.end();
			eg = const_edge_iter.next())
		{
			//if the node hasn't already been VISITED we can push the
			//edge onto the queue
			if (_visited[eg->getTo()] == UNVISITED)
			{
				que.push(eg);

				//and mark it VISITED
				_visited[eg->getTo()] = VISITED;
			}
		}
	}

	//no path to target
	return false;
}


//-----------------------------------------------------------------------------
template <class Graph>
std::list<int> SearchBFS<Graph>::getPathToTarget()const
{
	std::list<int> path;

	//just return an empty path if no path to target found or if
	//no target has been specified
	if (!_found || _target < 0) return path;

	int nd = _target;

	path.push_front(nd);

	while (nd != _source)
	{
		nd = _route[nd];

		path.push_front(nd);
	}

	return path;
}



//----------------------- SearchDijkstra --------------------------------
//
//  Given a graph, source and optional target this class solves for
//  single source shortest paths (without a target being specified) or 
//  shortest path from source to target.
//
//  The algorithm used is a priority queue implementation of Dijkstra's.
//  note how similar this is to the algorithm used in MinSpanningTree.
//  The main difference is in the calculation of the priority in the line:
//  
//  double new_cost = _cost_to_this_node[best] + eg->getCost;
//------------------------------------------------------------------------
template <class Graph>
class SearchDijkstra
{
private:

	//create a typedef for the edge type used by the graph
	typedef typename Graph::Edge Edge;

public:

	SearchDijkstra(
		const Graph&		graph,
		int					source,
		int					target = -1) 
		:
		_graph(graph),
		_shortest_path_tree(graph.numNodes()),
		_search_frontier(graph.numNodes()),
		_cost_to_this_node(graph.numNodes()),
		_source(source),
		_target(target)
	{
		search();
	}

	//returns the vector of edges that defines the SPT. If a target was given
	//in the constructor then this will be an SPT comprising of all the nodes
	//examined before the target was found, else it will contain all the nodes
	//in the graph.
	std::vector<const Edge*> getSPT()const { return _shortest_path_tree; }

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target. It calculates the path by working
	//backwards through the SPT from the target node.
	std::list<int> getPathToTarget()const;

	//returns the total cost to the target
	double getCostToTarget()const { return _cost_to_this_node[_target]; }

	//returns the total cost to the given node
	double getCostToNode(int i)const { return _cost_to_this_node[i]; }

private:

	void search();

private:

	const Graph&					_graph;

	//this vector contains the edges that comprise the shortest path tree -
	//a directed subtree of the graph that encapsulates the best paths from 
	//every node on the SPT to the source node.
	std::vector<const Edge*>		_shortest_path_tree;

	//this is indexed into by node index and holds the total cost of the best
	//path found so far to the given node. For example, _cost_to_this_node[5]
	//will hold the total cost of all the edges that comprise the best path
	//to node 5, found so far in the search (if node 5 is present and has 
	//been VISITED)
	std::vector<double>				_cost_to_this_node;

	//this is an indexed (by node) vector of 'object' edges leading to nodes 
	//connected to the SPT but that have not been added to the SPT yet. This is
	//a little like the stack or queue used in BST and DST searches.
	std::vector<const Edge*>		_search_frontier;

	int								_source;
	int								_target;
};


//-----------------------------------------------------------------------------
template <class Graph>
void SearchDijkstra<Graph>::search()
{
	//create an indexed priority queue that sorts smallest to largest
	//(front to back).Note that the maximum number of elements the iPQ
	//may contain is N. This is because no node can be represented on the 
	//queue more than once.
	IndexedPriorityQLow<double> pq(_cost_to_this_node, _graph.numNodes());

	//put the source node on the queue
	pq.insert(_source);

	//while the queue is not empty
	while (!pq.empty())
	{
		//get lowest cost node from the queue. Don't forget, the return value
		//is a *node index*, not the node itself. This node is the node not already
		//on the SPT that is the closest to the source node
		int next_closest_node = pq.Pop();

		//move this edge from the frontier to the shortest path tree
		_shortest_path_tree[next_closest_node] = _search_frontier[next_closest_node];

		//if the target has been found exit
		if (next_closest_node == _target) return;

		//now to relax the edges.
		Graph::ConstEdgeIterator const_edge_iter(_graph, next_closest_node);

		// CR: guard added to avoid assertion check in VS 8
		if (!const_edge_iter.end()) {
			//for each edge connected to the next closest node
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

					pq.insert(eg->getTo());

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
					pq.ChangePriority(eg->getTo());

					_search_frontier[eg->getTo()] = eg;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
template <class Graph>
std::list<int> SearchDijkstra<Graph>::getPathToTarget()const
{
	std::list<int> path;

	//just return an empty path if no target or no path found
	if (_target < 0)  return path;

	int nd = _target;

	path.push_front(nd);

	while ((nd != _source) && (_shortest_path_tree[nd] != 0))
	{
		nd = _shortest_path_tree[nd]->getFrom();

		path.push_front(nd);
	}

	return path;
}

//------------------------------- SearchAStar --------------------------
//
//  this searchs a graph using the distance between the target node and the 
//  currently considered node as a HeuristicPolicy.
//
//  This search is more commonly known as A* (pronounced Ay-Star)
//-----------------------------------------------------------------------------
template <class Graph, class HeuristicPolicy>
class SearchAStar
{
private:

	//create a typedef for the edge type used by the graph
	typedef typename Graph::Edge Edge;

private:

	const Graph&					_graph;

	//indexed into my node. Contains the 'real' accumulative cost to that node
	std::vector<double>             _g_costs;

	//indexed into by node. Contains the cost from adding _g_costs[n] to
	//the HeuristicPolicy cost from n to the target node. This is the vector the
	//iPQ indexes into.
	std::vector<double>				_f_costs;

	std::vector<const Edge*>		_shortest_path_tree;
	std::vector<const Edge*>		_search_frontier;

	int		_source;
	int		_target;

	//the A* search algorithm
	void search();

public:

	SearchAStar(Graph &graph,
		int   source,
		int   target) :_graph(graph),
		_shortest_path_tree(graph.numNodes()),
		_search_frontier(graph.numNodes()),
		_g_costs(graph.numNodes(), 0.0),
		_f_costs(graph.numNodes(), 0.0),
		_source(source),
		_target(target)
	{
		search();
	}

	//returns the vector of edges that the algorithm has examined
	std::vector<const Edge*> getSPT()const { return _shortest_path_tree; }

	//returns a vector of node indexes that comprise the shortest path
	//from the source to the target
	std::list<int> getPathToTarget()const;

	//returns the total cost to the target
	double getCostToTarget()const { return _g_costs[_target]; }
};

//-----------------------------------------------------------------------------
template <class Graph, class HeuristicPolicy>
void SearchAStar<Graph, HeuristicPolicy>::search()
{
	//create an indexed priority queue of nodes. The nodes with the
	//lowest overall F cost (G+H) are positioned at the front.
	IndexedPriorityQLow<double> pq(_f_costs, _graph.numNodes());

	//put the source node on the queue
	pq.insert(_source);

	//while the queue is not empty
	while (!pq.empty())
	{
		//get lowest cost node from the queue
		int next_closest_node = pq.Pop();

		//move this node from the frontier to the spanning tree
		_shortest_path_tree[next_closest_node] = _search_frontier[next_closest_node];

		//if the target has been found exit
		if (next_closest_node == _target) return;

		//now to test all the edges attached to this node
		Graph::ConstEdgeIterator const_edge_iter(_graph, next_closest_node);

		for (const Edge* eg = const_edge_iter.begin();
			!const_edge_iter.end();
			eg = const_edge_iter.next())
		{
			//calculate the HeuristicPolicy cost from this node to the target (H)                       
			double HCost = HeuristicPolicy::calculate(_graph, _target, eg->getTo());

			//calculate the 'real' cost to this node from the source (G)
			double GCost = _g_costs[next_closest_node] + eg->getCost();

			//if the node has not been added to the frontier, add it and update
			//the G and F costs
			if (_search_frontier[eg->getTo()] == NULL)
			{
				_f_costs[eg->getTo()] = GCost + HCost;
				_g_costs[eg->getTo()] = GCost;

				pq.insert(eg->getTo());

				_search_frontier[eg->getTo()] = eg;
			}

			//if this node is already on the frontier but the cost to get here
			//is cheaper than has been found previously, update the node
			//costs and frontier accordingly.
			else if ((GCost < _g_costs[eg->getTo()]) && 
					(_shortest_path_tree[eg->getTo()] == NULL))
			{
				_f_costs[eg->getTo()] = GCost + HCost;
				_g_costs[eg->getTo()] = GCost;

				pq.ChangePriority(eg->getTo());

				_search_frontier[eg->getTo()] = eg;
			}
		}
	}
}

//-----------------------------------------------------------------------------
template <class Graph, class HeuristicPolicy>
std::list<int> SearchAStar<Graph, HeuristicPolicy>::getPathToTarget()const
{
	std::list<int> path;

	//just return an empty path if no target or no path found
	if (_target < 0)  return path;

	int nd = _target;

	path.push_front(nd);

	while ((nd != _source) && (_shortest_path_tree[nd] != 0))
	{
		nd = _shortest_path_tree[nd]->getFrom();

		path.push_front(nd);
	}

	return path;
}



//---------------------- MinSpanningTree --------------------------------
//
//  given a graph and a source node you can use this class to calculate
//  the minimum spanning tree. If no source node is specified then the 
//  algorithm will calculate a spanning forest starting from node 1 
//
//  It uses a priority first queue implementation of Prims algorithm
//------------------------------------------------------------------------
template <class Graph>
class MinSpanningTree
{
private:

	//create a typedef for the edge type used by the graph
	typedef typename Graph::Edge Edge;

public:

	MinSpanningTree(
		Graph &G,
		int   source = -1) 
		:
		_graph(G),
		_spanning_tree(G.numNodes()),
		_fringe(G.numNodes()),
		_cost_to_this_node(G.numNodes(), -1)
	{
		if (source < 0)
		{
			for (int nd = 0; nd < G.numNodes(); ++nd)
			{
				if (_spanning_tree[nd] == 0)
				{
					search(nd);
				}
			}
		}

		else
		{
			search(source);
		}
	}

	std::vector<const Edge*> getSpanningTree() const { return _spanning_tree; }


private:
	void search(const int source);

	const Graph&				_graph;

	std::vector<double>			_cost_to_this_node;

	std::vector<const Edge*>	_spanning_tree;
	std::vector<const Edge*>	_fringe;
};


//-----------------------------------------------------------------------------
template <class Graph>
void MinSpanningTree<Graph>::search(const int source)
{
	//create a priority queue
	IndexedPriorityQLow<double> pq(_cost_to_this_node, _graph.numNodes());

	//put the source node on the queue
	pq.insert(source);

	//while the queue is not empty
	while (!pq.empty())
	{
		//get lowest cost edge from the queue
		int best = pq.Pop();

		//move this edge from the fringe to the spanning tree
		_spanning_tree[best] = _fringe[best];

		//now to test the edges attached to this node
		Graph::ConstEdgeIterator const_edge_iter(_graph, best);

		for (const Edge* eg = const_edge_iter.beg();
		!const_edge_iter.end();
			eg = const_edge_iter.nxt())
		{
			double priority = eg->getCost;

			if (_fringe[eg->getTo()] == 0)
			{
				_cost_to_this_node[eg->getTo()] = priority;

				pq.insert(eg->getTo());

				_fringe[eg->getTo()] = eg;
			}

			else if ((priority < _cost_to_this_node[eg->getTo()]) && (_spanning_tree[eg->getTo()] == 0))
			{
				_cost_to_this_node[eg->getTo()] = priority;

				pq.ChangePriority(eg->getTo());

				_fringe[eg->getTo()] = eg;
			}
		}
	}
}


#endif