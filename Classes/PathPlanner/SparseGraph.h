#ifndef SPARSEGRAPH_H
#define SPARSEGRAPH_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   SparseGraph.h
//
//  Desc:   Graph class using the adjacency list representation.
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <vector>
#include <list>
#include <cassert>
#include <string>
#include <iostream>
#include <map>
#include "cocos2d.h"
#include "NodeTypeEnumerations.h"
#include "../ParameterLoader.h"


template <class Node, class Edge>
class SparseGraph
{
public:

	//enable easy client access to the edge and node types used in the graph
	typedef Edge					Edge;
	typedef Node					Node;

	//a couple more typedefs to save my fingers and to help with the formatting
	//of the code on the printed page
	typedef std::vector<Node>		NodeVector;
	typedef std::list<Edge>			EdgeList;
	typedef std::vector<EdgeList>	EdgeListVector;

private:
	//the nodes that comprise this graph
	NodeVector			_nodes;

	//a vector of adjacency edge lists. (each node index keys into the 
	//list of edges associated with that node)
	EdgeListVector		_edges;

	//is this a directed graph?
	bool				_is_digraph;
	
	//the index of the next node to be added
	int					_next_node_index;

	//returns true if an edge is not already present in the graph. Used
	//when adding edges to make sure no duplicates are created.
	bool uniqueEdge(int from, int to) const;

	//iterates through all the edges in the graph and removes any that point
	//to an invalidated node
	void cullInvalidEdges();

public:

	//ctor
	explicit SparseGraph(bool digraph)
		:
		_next_node_index(0),
		_is_digraph(digraph) 
	{}

	//dtor
	~SparseGraph()
	{}

	//returns the node at the given index
	const Node& getNode(int idx)const;

	//non const version
	Node& getNode(int idx);

	//const method for obtaining a reference to an edge
	const Edge& getEdge(int from, int to)const;

	//non const version
	Edge& getEdge(int from, int to);

	//retrieves the next free node index
	int getNextFreeNodeIndex()const { return _next_node_index; }

	//adds a node to the graph and returns its index
	int addNode(Node node);

	//removes a node by setting its index to INVALID_NODE_INDEX
	void removeNode(int node);

	//Use this to add an edge to the graph. The method will ensure that the
	//edge passed as a parameter is valid before adding it to the graph. If the
	//graph is a digraph then a similar edge connecting the nodes in the opposite
	//direction will be automatically added.
	void addEdge(Edge edge);

	//removes the edge connecting from and to from the graph (if present). If
	//a digraph then the edge connecting the nodes in the opposite direction 
	//will also be removed.
	void removeEdge(int from, int to);

	//sets the cost of an edge
	void setEdgeCost(int from, int to, double cost);

	//returns the number of ACTIVE + INACTIVE nodes present in the graph
	int numNodes()const { return _nodes.size(); }

	//returns the number of ACTIVE nodes present in the graph (this method's
	//performance can be improved greatly by caching the value)
	int numActiveNodes()const
	{
		int count = 0;

		for (unsigned int n = 0; n < _nodes.size(); ++n) 
			if (_nodes[n].getIndex() != INVALID_NODE_INDEX) ++count;

		return count;
	}

	//returns the total number of edges present in the graph
	int numEdges()const
	{
		int tot = 0;

		for (EdgeListVector::const_iterator eg = _edges.begin();
			 eg != _edges.end(); ++eg)
		{
			tot += eg->size();
		}

		return tot;
	}

	//returns true if the graph is directed
	bool isDigraph()const { return _is_digraph; }

	//returns true if the graph contains no nodes
	bool isEmpty()const { return _nodes.empty(); }

	//returns true if a node with the given index is present in the graph
	bool isNodePresent(int nd)const;

	//returns true if an edge connecting the nodes 'to' and 'from'
	//is present in the graph
	bool isEdgePresent(int from, int to)const;

	void save(const char* filename) const;
	void load(const char* filename);

	void render();

	//clears the graph ready for new node insertions
	void clear() { _next_node_index = 0; _nodes.clear(); _edges.clear(); }

	void removeEdges()
	{
		for (EdgeListVector::iterator it = _edges.begin(); it != _edges.end(); ++it)
		{
			it->clear();
		}
	}

	//non const class used to iterate through all the edges connected to a specific node. 
	class EdgeIterator
	{
	private:
		typename EdgeList::iterator			_iterator;
		SparseGraph<Node, Edge>&	_graph;
		const int                           _node_index;

	public:

		EdgeIterator(SparseGraph<Node, Edge>& graph, int node) 
			:
			_graph(graph),
			_node_index(node)
		{
			/* we don't need to check for an invalid node index since if the node is
			   invalid there will be no associated edges*/
			_iterator = _graph._edges[_node_index].begin();
		}

		Edge*  begin()
		{
			_iterator = _graph._edges[_node_index].begin();
			return &(*_iterator);
		}

		Edge*  next()
		{
			++_iterator;
			if (end()) return nullptr;
			return &(*_iterator);
		}

		//return true if we are at the end of the edge list
		bool end()
		{
			return (_iterator == _graph._edges[_node_index].end());
		}
	};

	friend class EdgeIterator;

	//const class used to iterate through all the edges connected to a specific node. 
	class ConstEdgeIterator
	{
	private:
		typename EdgeList::const_iterator		_iterator;
		const SparseGraph<Node, Edge>&	_graph;
		const int								_node_index;

	public:

		ConstEdgeIterator(const SparseGraph<Node, Edge>& graph, int node) 
			:
			_graph(graph),
			_node_index(node)
		{
			// we don't need to check for an invalid node index since if the node is
			// invalid there will be no associated edges
			_iterator = _graph._edges[_node_index].begin();
		}

		const Edge*  begin()
		{
			_iterator = _graph._edges[_node_index].begin();
			return &(*_iterator);
		}

		const Edge*  next()
		{
			++_iterator;
			if (end()) return nullptr;
			return &(*_iterator);
		}

		//return true if we are at the end of the edge list
		bool end()
		{
			return (_iterator == _graph._edges[_node_index].end());
		}
	};

	friend class ConstEdgeIterator;

	//non const class used to iterate through the nodes in the graph
	class NodeIterator
	{
	private:
		typename NodeVector::iterator		_iterator;
		SparseGraph<Node, Edge>&    _graph;

		//if a graph node is removed, it is not removed from the 
		//vector of nodes (because that would mean changing all the indices of 
		//all the nodes that have a higher index). This method takes a node
		//iterator as a parameter and assigns the next valid element to it.
		void getNextValidNode(typename NodeVector::iterator& it)
		{
			if (_iterator == _graph._nodes.end() || it->getIndex() != INVALID_NODE_INDEX)
				return;

			while ((it->getIndex() == INVALID_NODE_INDEX))
			{
				++it;
				if (_iterator == _graph._nodes.end()) break;
			}
		}

	public:

		explicit NodeIterator(SparseGraph<Node, Edge>& graph) 
			:
			_graph(graph)
		{
			_iterator = _graph._nodes.begin();
		}


		Node* begin()
		{
			_iterator = _graph._nodes.begin();
			getNextValidNode(_iterator);
			return &(*_iterator);
		}

		Node* next()
		{
			++_iterator;
			getNextValidNode(_iterator);
			if (end()) return nullptr;
			return &(*_iterator);
		}

		bool end()
		{
			return (_iterator == _graph._nodes.end());
		}
	};


	friend class NodeIterator;

	//const class used to iterate through the nodes in the graph
	class ConstNodeIterator
	{
	private:
		typename NodeVector::const_iterator		_iterator;
		const SparseGraph<Node, Edge>&	_graph;

		//if a graph node is removed or switched off, it is not removed from the 
		//vector of nodes (because that would mean changing all the indices of 
		//all the nodes that have a higher index. This method takes a node
		//iterator as a parameter and assigns the next valid element to it.
		void getNextValidNode(typename NodeVector::const_iterator& it)
		{
			if (_iterator == _graph._nodes.end() || it->getIndex() != INVALID_NODE_INDEX) return;

			while ((it->getIndex() == INVALID_NODE_INDEX))
			{
				++it;
				if (_iterator == _graph._nodes.end()) break;
			}
		}

	public:

		explicit ConstNodeIterator(const SparseGraph<Node, Edge> &graph)
			:
			_graph(graph)
		{
			_iterator = _graph._nodes.begin();
		}


		const Node* begin()
		{
			_iterator = _graph._nodes.begin();
			getNextValidNode(_iterator);
			return &(*_iterator);
		}

		const Node* next()
		{
			++_iterator;
			getNextValidNode(_iterator);
			if (end()) return nullptr;
			return &(*_iterator);
		}

		bool end()
		{
			return (_iterator == _graph._nodes.end());
		}
	};

	friend class ConstNodeIterator;
};


//--------------------------- isNodePresent --------------------------------
//
//  returns true if a node with the given index is present in the graph
//--------------------------------------------------------------------------
template <class Node, class Edge>
bool SparseGraph<Node, Edge>::isNodePresent(int nd)const
{
	if ((nd >= _nodes.size()) || (_nodes[nd].getIndex() == INVALID_NODE_INDEX))
	{
		return false;
	}
	else return true;
}

//--------------------------- isEdgePresent --------------------------------
//
//  returns true if an edge with the given from/to is present in the graph
//--------------------------------------------------------------------------
template <class Node, class Edge>
bool SparseGraph<Node, Edge>::isEdgePresent(int from, int to)const
{
	if (isNodePresent(from) && isNodePresent(to))
	{
		for (EdgeList::const_iterator eg = _edges[from].begin();
			eg != _edges[from].end(); ++eg)
		{
			if (eg->getTo() == to) return true;
		}

		return false;
	}
	else return false;
}
//------------------------------ getNode -------------------------------------
//
//  const and non const methods for obtaining a reference to a specific node
//----------------------------------------------------------------------------
template <class Node, class Edge>
const Node&  SparseGraph<Node, Edge>::getNode(int idx)const
{
	assert((idx < (int)_nodes.size()) && (idx >= 0) &&
		"<SparseGraph::getNode>: invalid index");

	return _nodes[idx];
}

//non const version
template <class Node, class Edge>
Node&  SparseGraph<Node, Edge>::getNode(int idx)
{
	assert((idx < (int)_nodes.size()) &&
		(idx >= 0) &&
		"<SparseGraph::getNode>: invalid index");

	return _nodes[idx];
}

//------------------------------ getEdge -------------------------------------
//
//  const and non const methods for obtaining a reference to a specific edge
//----------------------------------------------------------------------------
template <class Node, class Edge>
const Edge& SparseGraph<Node, Edge>::getEdge(int from, int to)const
{
	assert((from < _nodes.size()) &&
		(from >= 0) &&
		_nodes[from].getIndex() != INVALID_NODE_INDEX &&
		"<SparseGraph::getEdge>: invalid 'from' index");

	assert((to < _nodes.size()) &&
		(to >= 0) &&
		_nodes[to].getIndex() != INVALID_NODE_INDEX &&
		"<SparseGraph::getEdge>: invalid 'to' index");

	for (EdgeList::const_iterator eg = _edges[from].begin();
		eg != _edges[from].end(); ++eg)
	{
		if (eg->getTo() == to) return *eg;
	}

	assert(0 && "<SparseGraph::getEdge>: edge does not exist");
}

//non const version
template <class Node, class Edge>
Edge& SparseGraph<Node, Edge>::getEdge(int from, int to)
{
	assert((from < _nodes.size()) &&
		(from >= 0) &&
		_nodes[from].getIndex() != INVALID_NODE_INDEX &&
		"<SparseGraph::getEdge>: invalid 'from' index");

	assert((to < _nodes.size()) &&
		(to >= 0) &&
		_nodes[to].getIndex() != INVALID_NODE_INDEX &&
		"<SparseGraph::getEdge>: invalid 'to' index");

	for (EdgeList::iterator eg = _edges[from].begin();
		eg != _edges[from].end(); ++eg)
	{
		if (eg->getTo() == to) return *eg;
	}

	assert(0 && "<SparseGraph::getEdge>: edge does not exist");
}

//-------------------------- addEdge ------------------------------------------
//
//  Use this to add an edge to the graph. The method will ensure that the
//  edge passed as a parameter is valid before adding it to the graph. If the
//  graph is a digraph then a similar edge connecting the nodes in the opposite
//  direction will be automatically added.
//-----------------------------------------------------------------------------
template <class Node, class Edge>
void SparseGraph<Node, Edge>::addEdge(Edge edge)
{
	//first make sure the from and to nodes exist within the graph 
	assert((edge.getFrom() < _next_node_index) && (edge.getTo() < _next_node_index) &&
		"<SparseGraph::addEdge>: invalid node index");

	//make sure both nodes are ACTIVE before adding the edge
	if ((_nodes[edge.getTo()].getIndex() != INVALID_NODE_INDEX) &&
		(_nodes[edge.getFrom()].getIndex() != INVALID_NODE_INDEX))
	{
		//add the edge, first making sure it is unique
		if (uniqueEdge(edge.getFrom(), edge.getTo()))
		{
			_edges[edge.getFrom()].push_back(edge);
		}

		//if the graph is undirected we must add another connection in the opposite
		//direction
		if (!_is_digraph)
		{
			//check to make sure the edge is unique before adding
			if (uniqueEdge(edge.getTo(), edge.getFrom()))
			{
				Edge new_edge = edge;

				new_edge.setTo(edge.getFrom());
				new_edge.setFrom(edge.getTo());

				_edges[edge.getTo()].push_back(new_edge);
			}
		}
	}
}


//----------------------------- removeEdge ---------------------------------
template <class Node, class Edge>
void SparseGraph<Node, Edge>::removeEdge(int from, int to)
{
	assert((from < (int)_nodes.size()) && (to < (int)_nodes.size()) &&
		"<SparseGraph::removeEdge>:invalid node index");

	EdgeList::iterator eg;

	if (!_is_digraph)
	{
		for (eg = _edges[to].begin();
			eg != _edges[to].end(); ++eg)
		{
			if (eg->getTo() == from) { eg = _edges[to].erase(eg); break; }
		}
	}

	for (eg = _edges[from].begin();
		eg != _edges[from].end(); ++eg)
	{
		if (eg->getTo() == to) { eg = _edges[from].erase(eg); break; }
	}
}

//-------------------------- addNode -------------------------------------
//
//  Given a node this method first checks to see if the node has been added
//  previously but is now innactive. If it is, it is reactivated.
//
//  If the node has not been added previously, it is checked to make sure its
//  index matches the next node index before being added to the graph
//------------------------------------------------------------------------
template <class Node, class Edge>
int SparseGraph<Node, Edge>::addNode(Node node)
{
	if (node.getIndex() < (int)_nodes.size())
	{
		//make sure the client is not trying to add a node with the same ID as
		//a currently ACTIVE node
		assert(_nodes[node.getIndex()].getIndex() == INVALID_NODE_INDEX &&
			"<SparseGraph::addNode>: Attempting to add a node with a duplicate ID");

		_nodes[node.getIndex()] = node;

		return _next_node_index;
	}

	else
	{
		//make sure the new node has been indexed correctly
		assert(node.getIndex() == _next_node_index && 
			"<SparseGraph::addNode>:invalid index");

		_nodes.push_back(node);
		_edges.push_back(EdgeList());

		return _next_node_index++;
	}
}

//----------------------- cullInvalidEdges ------------------------------------
//
//  iterates through all the edges in the graph and removes any that point
//  to an invalidated node
//-----------------------------------------------------------------------------
template <class Node, class Edge>
void SparseGraph<Node, Edge>::cullInvalidEdges()
{
	for (EdgeListVector::iterator edge_list = _edges.begin(); edge_list != _edges.end(); ++edge_list)
	{
		for (EdgeList::iterator eg = (*edge_list).begin(); eg != (*edge_list).end(); ++eg)
		{
			if (_nodes[eg->getTo()].getIndex() == INVALID_NODE_INDEX ||
				_nodes[eg->getFrom()].getIndex() == INVALID_NODE_INDEX)
			{
				eg = (*edge_list).erase(eg);
			}
		}
	}
}


//------------------------------- removeNode -----------------------------
//
//  Removes a node from the graph and removes any links to neighbouring
//  nodes
//------------------------------------------------------------------------
template <class Node, class Edge>
void SparseGraph<Node, Edge>::removeNode(int node)
{
	assert(node != INVALID_NODE_INDEX && node < (int)_nodes.size() && "<SparseGraph::removeNode>: invalid node index");

	//set this node's index to INVALID_NODE_INDEX
	_nodes[node].setIndex(INVALID_NODE_INDEX);

	//if the graph is not directed remove all edges leading to this node and then
	//clear the edges leading from the node
	if (!_is_digraph)
	{
		//visit each neighbour and erase any edges leading to this node
		for (EdgeList::iterator eg = _edges[node].begin();
			eg != _edges[node].end(); ++eg)
		{
			for (EdgeList::iterator eg2 = _edges[eg->getTo()].begin();
			eg2 != _edges[eg->getTo()].end(); ++eg2)
			{
				if (eg2->getTo() == node)
				{
					eg2 = _edges[eg->getTo()].erase(eg2);

					break;
				}
			}
		}

		//finally, clear this node's edges
		_edges[node].clear();
	}

	//if a digraph remove the edges the slow way
	else
	{
		cullInvalidEdges();
	}
}

//-------------------------- setEdgeCost ---------------------------------
//
//  Sets the cost of a specific edge
//------------------------------------------------------------------------
template <class Node, class Edge>
void SparseGraph<Node, Edge>::setEdgeCost(int from, int to, double NewCost)
{
	//make sure the nodes given are valid
	assert((from < _nodes.size()) && (to < _nodes.size()) &&
		"<SparseGraph::setEdgeCost>: invalid index");

	//visit each neighbour and erase any edges leading to this node
	for (EdgeList::iterator eg = _edges[from].begin();
		eg != _edges[from].end(); ++eg)
	{
		if (eg->getTo() == to)
		{
			eg->setCost(NewCost);
			break;
		}
	}
}

//-------------------------------- uniqueEdge ----------------------------
//
//  returns true if the edge is not present in the graph. Used when adding
//  edges to prevent duplication
//------------------------------------------------------------------------
template <class Node, class Edge>
bool SparseGraph<Node, Edge>::uniqueEdge(int from, int to)const
{
	for (EdgeList::const_iterator eg = _edges[from].begin();
		eg != _edges[from].end(); ++eg)
	{
		if (eg->getTo() == to)
		{
			return false;
		}
	}

	return true;
}

//-------------------------------- save ---------------------------------------
template <class Node, class Edge>
void SparseGraph<Node, Edge>::save(const char* filename)const
{
	std::ofstream out(filename);

	if (!out)
	{
		CCASSERT(false, ("Cannot open file: " + std::string(filename)).c_str());
		return;
	}

	std::stringstream ss;
	std::string json = "";

	ss << "{" << std::endl;
	ss << "\"property\": {}," << std::endl;
	ss << "\"node\": [" << std::endl;
	for (NodeVector::const_iterator iter = _nodes.begin();
	iter != _nodes.end(); ++iter)
	{
		ss << *iter << "," << std::endl;
	}
	json += ss.str();
	json.pop_back();
	json.pop_back();

	ss.str("");
	ss << "]," << std::endl;
	ss << "\"edge\": [" << std::endl;
	for (int nodeIdx = 0; nodeIdx < _nodes.size(); ++nodeIdx)
	{
		for (EdgeList::const_iterator iter = _edges[nodeIdx].begin();
		iter != _edges[nodeIdx].end(); ++iter)
		{
			ss << *iter << "," << std::endl;
		}
	}
	json += ss.str();
	json.pop_back();
	json.pop_back();

	ss.str("");
	ss << "]" << std::endl;
	ss << "}" << std::endl;
	json += ss.str();

	out << json;
}

//------------------------------- load ----------------------------------------
//-----------------------------------------------------------------------------
template <class Node, class Edge>
void SparseGraph<Node, Edge>::load(const char* filename)
{
	rapidjson::Document json;
	util::readJSON(filename, json);

	auto nbegin = json["node"].Begin();
	auto nend = json["node"].End();

	for (auto iter = nbegin; iter != nend; iter++)
	{
		_nodes.emplace_back(Node(*iter));
	}

	_edges.assign(_nodes.size(), EdgeList());
	auto ebegin = json["edge"].Begin();
	auto eend = json["edge"].End();

	for (auto iter = ebegin; iter != eend; iter++)
	{
		int from = (*iter)["from"].GetInt();
		_edges[from].emplace_back(Edge(*iter));
	}
}


template <class Node, class Edge>
void SparseGraph<Node, Edge>::render()
{
	CCLOG("Node : %d", numActiveNodes());
	ConstNodeIterator node_iter(*this);
	for (const Node* node = node_iter.begin();
		!node_iter.end();
		node = node_iter.next())
	{
		//Debug.getGraphPanel()->
			//drawCircle(node->getPos(), 10, 2 * M_PI, 40, false, Color4F::MAGENTA);
	}

	CCLOG("Edge : %d", numEdges());
	for (int nodeIdx = 0; nodeIdx < _nodes.size(); ++nodeIdx)
	{
		for (EdgeList::const_iterator eg = _edges[nodeIdx].begin();
		eg != _edges[nodeIdx].end(); ++eg)
		{
			//Debug.getGraphPanel()->
				//drawLine(
				//getNode(eg->getFrom()).getPos(),
				//getNode(eg->getTo()).getPos(),
				//Color4F::GRAY);
		}
	}
}

#endif
