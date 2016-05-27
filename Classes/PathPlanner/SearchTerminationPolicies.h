#ifndef TERMINATION_POLICIES_H
#define TERMINATION_POLICIES_H
//-----------------------------------------------------------------------------
//
//  Name:   SearchTerminationPolicies.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class templates to define termination policies for Dijkstra's
//          algorithm
//-----------------------------------------------------------------------------



//--------------------------- FindNodeIndex -----------------------------------

//the search will terminate when the currently examined graph node
//is the same as the target node.
class FindNodeIndex
{
public:

	template <class Graph>
	static bool isSatisfied(const Graph& graph, int target, int current_node_idx)
	{
		return current_node_idx == target;
	}
};

//--------------------------- FindActiveTrigger ------------------------------

//the search will terminate when the currently examined graph node
//is the same as the target node.
template <class Trigger>
class FindActiveTrigger
{
public:

	template <class Graph>
	static bool isSatisfied(const Graph& graph, int target, int current_node_idx)
	{
		bool satisfied = false;

		//get a reference to the node at the given node index
		const Graph::Node& node = graph.getNode(current_node_idx);

		//if the extrainfo field is pointing to a giver-trigger, test to make sure 
		//it is ACTIVE and that it is of the correct type.
		if ((node.getExtraInfo() != nullptr) &&
			node.getExtraInfo()->isActive() &&
			(node.getExtraInfo()->getType() == target))
		{
			satisfied = true;
		}

		return satisfied;
	}
};



#endif