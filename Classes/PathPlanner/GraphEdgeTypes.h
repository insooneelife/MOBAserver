#ifndef GRAPH_EDGE_TYPES_H
#define GRAPH_EDGE_TYPES_H
//-----------------------------------------------------------------------------
//
//  Name:   GraphEdgeTypes.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Class to define an edge connecting two nodes.
//          
//          An edge has an associated cost.
//-----------------------------------------------------------------------------
#include <sstream>
#include "json\reader.h"
#include "json\document.h"
#include "NodeTypeEnumerations.h"

class GraphEdge
{
public:

	//ctors
	GraphEdge(int from, int to, double cost)
		:
		_cost(cost),
		_from(from),
		_to(to)
	{}

	GraphEdge(int from, int  to)
		:
		_cost(1.0),
		_from(from),
		_to(to)
	{}

	GraphEdge()
		:
		_cost(1.0),
		_from(INVALID_NODE_INDEX),
		_to(INVALID_NODE_INDEX)
	{}

	GraphEdge(const rapidjson::GenericValue<rapidjson::UTF8<>>& json)
		:
		_from(json["from"].GetInt()),
		_to(json["to"].GetInt())
	{}

	virtual ~GraphEdge() {}

	int   getFrom() const			{ return _from; }
	void  setFrom(int new_index)	{ _from = new_index; }

	int   getTo() const				{ return _to; }
	void  setTo(int new_index)		{ _to = new_index; }

	double getCost() const			{ return _cost; }
	void setCost(double new_index)	{ _cost = new_index; }


	//these two operators are required
	bool operator==(const GraphEdge& rhs)
	{
		return rhs._from == this->_from &&
			rhs._to == this->_to   &&
			rhs._cost == this->_cost;
	}

	bool operator!=(const GraphEdge& rhs)
	{
		return !(*this == rhs);
	}


	//	{
	//		"from": 1,
	//		"to": 2,
	//		"cost": 122.5
	//	}
	friend std::stringstream& operator<<(std::stringstream& ss, const GraphEdge& e)
	{
		ss << "{" << std::endl;
		ss << "\"from\": " << e._from << "," << std::endl;
		ss << "\"to\": " << e._to << "," << std::endl;
		ss << "\"cost\": " << e._cost << std::endl;
		ss << "}" << std::endl;

		return ss;
	}

protected:

	//An edge connects two nodes. Valid node indices are always positive.
	int     _from;
	int     _to;

	//the cost of traversing the edge
	double  _cost;
};


class NavGraphEdge : public GraphEdge
{
public:

	//examples of typical flags
	enum
	{
		NORMAL = 0,
		SWIM = 1 << 0,
		CRAWL = 1 << 1,
		CREEP = 1 << 3,
		JUMP = 1 << 3,
		FLY = 1 << 4,
		GRAPPLE = 1 << 5,
		GOES_THROUGH_DOOR = 1 << 6
	};

protected:

	int   _flag;

	//if this edge intersects with an object (such as a door or lift), then
	//this is that object's ID. 
	int  _id_intersect_entity;

public:


	NavGraphEdge(
		int    from,
		int    to,
		double cost,
		int    flags = 0,
		int    id = -1)
		:
		GraphEdge(from, to, cost),
		_flag(flags),
		_id_intersect_entity(id)
	{}

	NavGraphEdge(const rapidjson::GenericValue<rapidjson::UTF8<>>& json)
		:
		GraphEdge(json["from"].GetInt(), json["to"].GetInt(), json["cost"].GetDouble()),
		_flag(json["flag"].GetInt()),
		_id_intersect_entity(json["id"].GetInt())
	{}

	virtual ~NavGraphEdge() override 
	{}

	int  getFlags() const		{ return _flag; }
	void setFlags(int flags)	{ _flag = flags; }

	int  getIDofIntersectingEntity() const	{ return _id_intersect_entity; }
	void setIDofIntersectingEntity(int id)	{ _id_intersect_entity = id; }


	//	{
	//		"from": 1,
	//		"to": 2,
	//		"cost": 122.5,
	//		"flag": 0,
	//		"id": -1
	//	}
	friend std::stringstream& operator<<(std::stringstream& ss, const NavGraphEdge& e)
	{
		ss << "{" << std::endl;
		ss << "\"from\": " << e._from << "," << std::endl;
		ss << "\"to\": " << e._to << "," << std::endl;
		ss << "\"cost\": " << e._cost << "," << std::endl;
		ss << "\"flag\": " << e._flag << "," << std::endl;
		ss << "\"id\": " << e._id_intersect_entity << std::endl;
		ss << "}" << std::endl;

		return ss;
	}
};


#endif