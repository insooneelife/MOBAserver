#ifndef MAP_H
#define MAP_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Map.h
//
//  Desc:   This class creates and stores all the entities that make up the
//          map environment. (walls, structures)
//			For rendering, it references a view layer and has a tilemap.
//			For working system, it has trigger system for control game flows,
//			cell space partition for make more performance interacting logic,
//			and has navigation graph for agent to find path.
//			It also has reference to world, so it can interact with it. 
//
//  Author: Insub Im (insooneelife@naver.com)
//
//-----------------------------------------------------------------------------
#include <vector>
#include <string>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "cocos2d.h"
#include "CellSpacePartition\CellSpacePartition.h"
#include "PathPlanner\SparseGraph.h"
#include "PathPlanner\GraphNodeTypes.h"
#include "PathPlanner\GraphEdgeTypes.h"
#include "Triggers\Trigger.h"

class BaseGameEntity;
class Wall;
class GameWorld;
class MovingEntity;
class AbstCharacter;
class CollisionArea;
class Structure;
template <class Trigger> class TriggerSystem;

class GameMap
{
public:
	typedef NavGraphNode<Trigger<AbstCharacter>*>		GraphNode;
	typedef SparseGraph<GraphNode, NavGraphEdge>		NavGraph;
	typedef CellSpacePartition<NavGraph::Node*>			GraphCellSpace;
	typedef CellSpacePartition<AbstCharacter*>			AgentCellSpace;
	typedef CellSpacePartition<CollisionArea*>			CollisionAreaCellSpace;

	typedef Trigger<AbstCharacter>						Trigger;
	typedef TriggerSystem<Trigger>						TriggerSystem;

	typedef std::vector<std::unique_ptr<CollisionArea> >	CollisionAreaVector;
	typedef std::vector<std::unique_ptr<Structure> >		StructureVector;

	GameMap(GameWorld* const game_world);

	~GameMap();


	GameWorld*						getGameWorld() const;

	const CollisionAreaVector&		getCollisionAreas() const;

	const StructureVector&			getStructures() const;

	cocos2d::TMXTiledMap*			getTileMap() const;

	float							getSizeX() const;

	float							getSizeY() const;

	double							getCellSpaceNeighborhoodRange() const;

	const std::unique_ptr<NavGraph>&		getNavGraph()const;

	const std::unique_ptr<AgentCellSpace>& 	getAgentCellSpace() const;

	const std::unique_ptr<GraphCellSpace>&	getGraphCellSpace() const;

	const std::unique_ptr<CollisionAreaCellSpace>&	getCollisionAreaCellSpace() const;

	void updateTriggerSystem(
		std::vector<std::unique_ptr<AbstCharacter> >& characters,
		double time_elapsed);

	void createStructures();

	void createCollisionAreas();

	void createGraph();

	void createTriggers();

	void tagObstaclesWithinViewRange(BaseGameEntity* const entity, double range);

	void render();

	void partitionNavGraph();

	void partitionCollisionArea();
	
private:

	GameMap(const GameMap&) = delete; // no copies
	GameMap& operator=(const GameMap&) = delete; // no self-assignments
	GameMap() = delete;

	float				_sizeX;
	float				_sizeY;

	//references game world
	GameWorld* const						_game_world;

	//collision areas
	CollisionAreaVector						_collision_areas;

	//structures
	StructureVector							_structures;

	//tile map
	cocos2d::TMXTiledMap*					_tile_map;

	std::unique_ptr<AgentCellSpace>			_agent_cell_space;

	//the graph nodes will be partitioned enabling fast lookup
	std::unique_ptr<GraphCellSpace>			_graph_cell_space;

	std::unique_ptr<CollisionAreaCellSpace>	_collision_area_cell_space;

	//this map's accompanying navigation graph
	std::unique_ptr<NavGraph>				_nav_graph;
	
	//the size of the search radius the cellspace partition uses 
	//when looking for neighbors 
	double									_cellspace_neighbor_range;

	//trigger are objects that define a region of space. When a raven bot
	//enters that area, it 'triggers' an event. That event may be anything
	//from increasing a bot's health to opening a door or requesting a lift.
	std::unique_ptr<TriggerSystem>			_trigger_system;

};



#endif