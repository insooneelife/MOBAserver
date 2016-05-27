#ifndef GRAPH_NODE_TYPES_H
#define GRAPH_NODE_TYPES_H
//-----------------------------------------------------------------------------
//
//  Name:   GraphNodeTypes.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Node classes to be used with graphs
//-----------------------------------------------------------------------------
#include <list>
#include <sstream>
#include "cocos2d.h"
#include "json\reader.h"
#include "json\document.h"
#include "NodeTypeEnumerations.h"

class GraphNode
{
public:

	GraphNode() :_index(INVALID_NODE_INDEX) {}
	explicit GraphNode(int idx) :_index(idx) {}
	explicit GraphNode(const rapidjson::GenericValue<rapidjson::UTF8<>>& json)
		:
		_index(json["index"].GetInt())
	{}

	virtual ~GraphNode() 
	{}

	int  getIndex() const			{ return _index; }
	void setIndex(int new_index)	{ _index = new_index; }

	//	{
	//		"index": 2
	//	}
	friend std::stringstream& operator<<(std::stringstream& ss, const GraphNode& n)
	{
		ss << "{" << std::endl;
		ss << "\"index\": " << n._index << std::endl;
		ss << "}";
		return ss;
	}

protected:

	//every node has an index. A valid index is >= 0
	int        _index;

};



//-----------------------------------------------------------------------------
//
//  Graph node for use in creating a navigation graph.This node contains
//  the position of the node and a pointer to a BaseGameEntity... useful
//  if you want your nodes to represent health packs, gold mines and the like
//-----------------------------------------------------------------------------
template <class ExtraInfo = void*>
class NavGraphNode : public GraphNode
{
protected:

	//the node's position
	cocos2d::Vec2     _position;

	//often you will require a navgraph node to contain additional information.
	//For example a node might represent a pickup such as armor in which
	//case _extra_info could be an enumerated value denoting the pickup type,
	//thereby enabling a search algorithm to search a graph for specific items.
	//Going one step further, _extra_info could be a pointer to the instance of
	//the item type the node is twinned with. This would allow a search algorithm
	//to test the status of the pickup during the search. 
	ExtraInfo  _extra_info;

public:

	//ctors
	NavGraphNode() :_extra_info(ExtraInfo()) {}

	NavGraphNode(int idx, cocos2d::Vec2 pos) 
		:
		GraphNode(idx),
		_position(pos),
		_extra_info(ExtraInfo())
	{}

	NavGraphNode(const rapidjson::GenericValue<rapidjson::UTF8<>>& json)
		:
		GraphNode(json["index"].GetInt()),
		_position(cocos2d::Vec2(json["x"].GetDouble(), json["y"].GetDouble())),
		_extra_info(ExtraInfo())
	{}

	virtual ~NavGraphNode() override
	{}

	cocos2d::Vec2   getPos() const						{ return _position; }
	void			setPos(cocos2d::Vec2 new_position)	{ _position = new_position; }

	ExtraInfo	getExtraInfo() const					{ return _extra_info; }
	void		setExtraInfo(ExtraInfo info)			{ _extra_info = info; }

	//	{
	//		"index": 2,
	//		"x" : 11,
	//		"y" : 55.125
	//	}
	friend std::stringstream& operator<<(std::stringstream& ss, const NavGraphNode& n)
	{
		ss << "{" << std::endl;
		ss << "\"index\": " << n._index << "," << std::endl;
		ss << "\"x\": " << n._position.x << "," << std::endl;
		ss << "\"y\": " << n._position.y << std::endl;
		ss << "}" << std::endl;
		return ss;
	}
};

#endif
