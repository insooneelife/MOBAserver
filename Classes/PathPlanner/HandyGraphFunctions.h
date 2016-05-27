#ifndef GRAPH_FUNCS
#define GRAPH_FUNCS
//-----------------------------------------------------------------------------
//
//  Name:   HandyGraphFunctions.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   As the name implies, some useful functions you can use with your
//          graphs. 

//          For the function templates, make sure your graph interface complies
//          with the SparseGraph class
//-----------------------------------------------------------------------------
#include <iostream>

#include "GraphAlgorithms.h"
#include "CollisionArea.h"
#include "AStarHeuristicPolicies.h"
#include "../Geometry/Geometry.h"


//--------------------------- validNeighbour -----------------------------
//
//  returns true if x,y is a valid position in the map
//------------------------------------------------------------------------
bool validNeighbour(int x, int y, int num_cellsX, int num_cellsY)
{
	return !((x < 0) || (x >= num_cellsX) || (y < 0) || (y >= num_cellsY));
}

//------------ addAllNeighboursToGridNode ------------------
//
//  use to add he eight neighboring edges of a graph node that 
//  is positioned in a grid layout
//------------------------------------------------------------------------
template <class Graph>
void addAllNeighboursToGridNode(
	Graph& graph,
	int         row,
	int         col,
	int         num_cellsX,
	int         num_cellsY)
{
	for (int i = -1; i < 2; ++i)
	{
		for (int j = -1; j < 2; ++j)
		{
			int nodeX = col + j;
			int nodeY = row + i;

			//skip if equal to this node
			if ((i == 0) && (j == 0)) continue;

			//check to see if this is a valid neighbour
			if (validNeighbour(nodeX, nodeY, num_cellsX, num_cellsY))
			{
				//calculate the distance to this node
				cocos2d::Vec2 pos_node = graph.getNode(row*num_cellsX + col).getPos();
				cocos2d::Vec2 pos_neighbour = graph.getNode(nodeY*num_cellsX + nodeX).getPos();

				double dist = (pos_node - pos_neighbour).getLength();

				//this neighbour is okay so it can be added
				Graph::Edge new_edge(
					row * num_cellsX + col,
					nodeY * num_cellsX + nodeX,
					dist);
				graph.addEdge(new_edge);

				//if graph is not a diagraph then an edge needs to be added going
				//in the other direction
				if (!graph.isDigraph())
				{
					Graph::Edge new_edge(
						nodeY * num_cellsX + nodeX,
						row * num_cellsX + col,
						dist);
					graph.addEdge(new_edge);
				}
			}
		}
	}
}


//--------------------------- generateGrid --------------------------
//
//  creates a graph based on a grid layout. This function requires the 
//  dimensions of the environment and the number of cells required horizontally
//  and vertically 
//-----------------------------------------------------------------------------
template <class Graph>
void generateGrid(
	Graph& graph,
	int cx_size,
	int cy_size,
	int num_cellsX,
	int num_cellsY)
{
	//need some temporaries to help calculate each node center
	double cell_width = (double)cx_size / (double)num_cellsX;
	double cell_height = (double)cy_size / (double)num_cellsY;

	double midX = cell_width / 2;
	double midY = cell_height / 2;


	//first create all the nodes
	for (int row = 0; row < num_cellsY; ++row)
	{
		for (int col = 0; col < num_cellsX; ++col)
		{
			graph.addNode(
				Graph::Node(graph.getNextFreeNodeIndex(),
				cocos2d::Vec2(midX + (col * cell_width), midY + (row * cell_height))));
		}
	}
	//now to calculate the edges. (A position in a 2d array [x][y] is the
	//same as [y*num_cellsX + x] in a 1d array). Each cell has up to eight
	//neighbours.
	for (int row = 0; row < num_cellsY; ++row)
	{
		for (int col = 0; col < num_cellsX; ++col)
		{
			addAllNeighboursToGridNode(graph, row, col, num_cellsX, num_cellsY);
		}
	}
}


//--------------------------- weightNavGraphNodeEdges -------------------------
//
//  Given a cost value and an index to a valid node this function examines 
//  all a node's edges, calculates their length, and multiplies
//  the value with the weight. Useful for setting terrain costs.
//------------------------------------------------------------------------
template <class Graph>
void weightNavGraphNodeEdges(Graph& graph, int node, double weight)
{
	//make sure the node is present
	assert(node < graph.numNodes());

	//set the cost for each edge
	Graph::ConstEdgeIterator const_edge_iter(graph, node);
	for (const Graph::Edge* eg = const_edge_iter.begin();
		!const_edge_iter.end();
		eg = const_edge_iter.next())
	{
		//calculate the distance between nodes
		double dist = (graph.getNode(eg->getFrom()).getPos()
			- graph.getNode(eg->getTo()).getPos()).getLength();

		//set the cost of this edge
		graph.setEdgeCost(eg->getFrom(), eg->getTo(), dist * weight);

		//if not a digraph, set the cost of the parallel edge to be the same
		if (!graph.isDigraph())
		{
			graph.setEdgeCost(eg->getTo(), eg->getFrom(), dist * weight);
		}
	}
}


//---------------------- calculateAverageGraphEdgeLength ----------------------
//
//  determines the average length of the edges in a navgraph (using the 
//  distance between the source & target node positions (not the cost of the 
//  edge as represented in the graph, which may account for all sorts of 
//  other factors such as terrain type, gradients etc)
//------------------------------------------------------------------------------
template <class Graph>
double calculateAverageGraphEdgeLength(const Graph& graph)
{
	double total_length = 0;
	int num_edges_counted = 0;

	Graph::ConstNodeIterator node_iter(graph);
	const Graph::Node* node;
	for (node = node_iter.begin(); !node_iter.end(); node = node_iter.next())
	{
		Graph::ConstEdgeIterator edge_iter(graph, node->getIndex());
		for (const Graph::Edge* eg = edge_iter.begin();
			!edge_iter.end(); eg = edge_iter.next())
		{
			//increment edge counter
			++num_edges_counted;

			//add length of edge to total length
			total_length += (graph.getNode(eg->getFrom()).getPos() 
				- graph.getNode(eg->getTo()).getPos()).getLength();
		}
	}

	return total_length / (double)num_edges_counted;
}

//----------------------------- getCostliestGraphEdge -------------------
//
//  returns the cost of the costliest edge in the graph
//-----------------------------------------------------------------------------
template <class Graph>
double getCostliestGraphEdge(const Graph& graph)
{
	double greatest = std::numeric_limits<double>::min();

	Graph::ConstNodeIterator node_iter(graph);
	const Graph::Node* node;
	for (node = node_iter.begin(); !node_iter.end(); node = node_iter.next())
	{
		Graph::ConstEdgeIterator edge_iter(graph, node->getIndex());
		for (const Graph::Edge* eg = edge_iter.begin();
		!edge_iter.end(); eg = edge_iter.next())
		{
			if (eg->getCost() > greatest)greatest = eg->getCost();
		}
	}

	return greatest;
}


//--------------------------- removeNodeOnCollisionArea -----------------------
//
//  removes nodes which overlapped with CollisionArea
//-----------------------------------------------------------------------------
template <class Graph, typename CollisionArea>
void removeNodeOnCollisionArea(
	Graph& graph,
	const CollisionArea& collision_area,
	double ent_rad)
{
	Graph::ConstNodeIterator node_iter(graph);
	const Graph::Node* node;
	for (auto ca = collision_area.begin(); ca != collision_area.end(); ca++)
	{
		for (node = node_iter.begin(); !node_iter.end(); node = node_iter.next())
		{
			if (geometry::pointInCircle((*ca)->getPos(), (*ca)->getBRadius() + ent_rad, node->getPos()))
			{
				graph.removeNode(node->getIndex());
			}
		}
	}
}


//--------------------------- removeEdgeOnCollisionArea -----------------------
//
//  removes nodes which overlapped with CollisionArea
//-----------------------------------------------------------------------------
template <class Graph, typename CollisionArea>
void removeEdgeOnCollisionArea(
	Graph& graph,
	const CollisionArea& collision_area,
	double ent_rad)
{
	for (auto ca = collision_area.begin(); ca != collision_area.end(); ca++)
	{
		Graph::ConstNodeIterator node_iter(graph);
		const Graph::Node* node;

		for (node = node_iter.begin(); !node_iter.end(); node = node_iter.next())
		{
			Graph::ConstEdgeIterator edge_iter(graph, node->getIndex());
			const Graph::Edge* edge;

			std::vector<std::pair<int, int> > remove_edges;

			for (edge = edge_iter.begin(); !edge_iter.end(); edge = edge_iter.next())
			{
				if (geometry::segmentCircleIntersect(
						graph.getNode(edge->getFrom()).getPos(),
						graph.getNode(edge->getTo()).getPos(),
						(*ca)->getPos(),
						(*ca)->getBRadius() + ent_rad))
				{
					remove_edges.emplace_back(std::make_pair(edge->getFrom(), edge->getTo()));
				}
			}

			for (auto e : remove_edges) 
			{
				graph.removeEdge(e.first, e.second);
			}
		}
	}
}

#endif
