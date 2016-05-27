#include <fstream>
#include <map>
#include <vector>
#include <tuple>
#include <algorithm>
#include "GameMap.h"
#include "MovingEntity.h"
#include "GameWorld.h"
#include "AbstCharacter.h"
#include "CollisionArea.h"
#include "Structure.h"
#include "EntityManager.h"
#include "EntityFunctionTemplates.h"
#include "Util.h"
#include "Triggers\TriggerHealthGiver.h"
#include "Triggers\TriggerSpawnLine.h"
#include "Triggers\TriggerSystem.h"
#include "Triggers\TriggerSpawnJungle.h"
#include "PathPlanner\HandyGraphFunctions.h"
#include "BaseCharacter.h"


USING_NS_CC;

GameMap::GameMap(GameWorld* const game_world)
	:
	_game_world(game_world),
	_trigger_system(new TriggerSystem())
{

	_tile_map = cocos2d::TMXTiledMap::create("tilemap/100_160_2.tmx");
	//_tile_map = cocos2d::TMXTiledMap::create("tilemap/diablo2_outdoor_remove_walls.tmx");
	
	auto ms = _tile_map->getMapSize();
	auto ts = _tile_map->getTileSize();
	_sizeX = (ms.width + 0.5f) * ts.width;
	_sizeY = (ms.height + 1) * ts.height / 2;

	CCLOG("%f %f %f %f", _sizeX, _sizeY, ts.width, ts.height);

	_agent_cell_space.reset(
		new AgentCellSpace(
			_sizeX,
			_sizeY,
			Prm.NumCellsX,
			Prm.NumCellsY,
			Prm.NumMaxAgentCellSpace));

	createCollisionAreas();
	createStructures();
	
	/*_trigger_system->registerTrigger(
		new TriggerHealthGiver(
		_game_world,
		new geometry::Circle(Vec2(500, 500), 300),
		"health_trigger1",
		Vec2(500, 500), Vec2(1,1), 200, 0, 0, 1000));*/


	//first of all read and create the navgraph. This must be done before
	//the entities are read from the map file because many of the entities
	//will be linked to a graph node (the graph node will own a pointer
	//to an instance of the entity)
	_nav_graph.reset(new NavGraph(false));
	createGraph();

	//_nav_graph->render();
	CCLOG("Node : %d   Edge : %d", _nav_graph->numActiveNodes(), _nav_graph->numEdges());

	//determine the average distance between graph nodes so that we can
	//partition them efficiently
	_cellspace_neighbor_range = calculateAverageGraphEdgeLength(*_nav_graph) + 1;

	//partition the graph nodes
	partitionNavGraph();
	createTriggers();
	partitionCollisionArea();
}

GameMap::~GameMap()
{}


void GameMap::createStructures()
{
	//read structure data from tmx file
	auto ob_group = _tile_map->getObjectGroup("structures");
	CCASSERT(ob_group, "no object group structures");

	for (auto iter = std::begin(ob_group->getObjects()); 
		iter != std::end(ob_group->getObjects()); iter++)
	{
		float width = iter->asValueMap()["width"].asFloat();
		float height = iter->asValueMap()["height"].asFloat();
		float x = iter->asValueMap()["x"].asFloat() + width * 0.5f;
		float y = iter->asValueMap()["y"].asFloat() 
			- getSizeY() + _tile_map->getTileSize().height + height * 1.5f;
		
		float mx = iter->asValueMap()["marginX"].asFloat();
		float my = iter->asValueMap()["marginY"].asFloat();

		std::string name = iter->asValueMap()["name"].asString();
		float radius = iter->asValueMap()["radius"].asFloat();
		bool shadow = iter->asValueMap()["shadow"].asBool();

		_structures.emplace_back(
			Structure::create(
				_game_world,
				new geometry::Circle(Vec2(x, y) - Vec2(mx, my), radius),
				name,
				Vec2(x, y),
				radius));
	}
}


void GameMap::createCollisionAreas()
{
	auto ob_group = _tile_map->getObjectGroup("structures_no_image");
	CCASSERT(ob_group, "no object group (structures_no_image)");

	for (auto iter = std::begin(ob_group->getObjects());
	iter != std::end(ob_group->getObjects()); iter++)
	{
		float width = iter->asValueMap()["width"].asFloat();
		float height = iter->asValueMap()["height"].asFloat();
		float x = iter->asValueMap()["x"].asFloat();
		float y = iter->asValueMap()["y"].asFloat()
			- getSizeY() + _tile_map->getTileSize().height;
		std::string type = iter->asValueMap()["type"].asString();

		if (type == "Polygon")
		{
			ValueVector points = iter->asValueMap()["polylinePoints"].asValueVector();
			std::vector<cocos2d::Vec2> poly;

			for (int i = 0; i < points.size(); i++)
			{
				ValueMap a = points[i].asValueMap();

				float xy = a["x"].asFloat();
				float yy = a["y"].asFloat();

				poly.push_back(cocos2d::Vec2(xy + x, y - yy));
			}
			poly.pop_back();

			geometry::Circle min_circle = geometry::enclosingCircleNaive(poly);

			_collision_areas.emplace_back(
				CollisionArea::create(
					_game_world,
					new geometry::Polygon(poly),
					min_circle.origin,
					min_circle.radius));
		}
		else if (type == "Circle")
		{
			float radius = width / 2;
			_collision_areas.emplace_back(
				CollisionArea::create(
					_game_world,
					new geometry::Circle(Vec2(x + width * 0.5f, y + height * 0.5f), radius),
					Vec2(x + width * 0.5f, y + height * 0.5f),
					radius));
		}
	}
}


void GameMap::createGraph()
{
	//SparseGraph uses special mechanism in addChild,
	//then the index will be changed after insert.
	//So we have to remember them.
	std::map<int, int> index_change_map;
	auto node_group = _tile_map->getObjectGroup("nodes");
	for (auto iter = std::begin(node_group->getObjects());
	iter != std::end(node_group->getObjects()); iter++)
	{
		int index = iter->asValueMap()["index"].asInt();
		int extra = iter->asValueMap()["extra"].asInt();
		float x = iter->asValueMap()["x"].asFloat();
		float y = iter->asValueMap()["y"].asFloat()
			- getSizeY() + _tile_map->getTileSize().height;

		int real_index = (*_nav_graph).getNextFreeNodeIndex();
		(*_nav_graph).addNode(NavGraph::Node(real_index, cocos2d::Vec2(x, y)));

		index_change_map.insert(std::make_pair(index, real_index));
	}

	auto edge_group = _tile_map->getObjectGroup("edges");
	for (auto iter = std::begin(edge_group->getObjects());
	iter != std::end(edge_group->getObjects()); iter++)
	{
		int from = iter->asValueMap()["from"].asInt();
		int to = iter->asValueMap()["to"].asInt();
		int flag = iter->asValueMap()["flag"].asInt();

		int real_from = index_change_map[from];
		int real_to = index_change_map[to];

		//calculate the distance to this node
		cocos2d::Vec2 pos_from_node = (*_nav_graph).getNode(real_from).getPos();
		cocos2d::Vec2 pos_to_node = (*_nav_graph).getNode(real_to).getPos();

		double dist = (pos_from_node - pos_to_node).getLength();

		//this neighbour is okay so it can be added
		NavGraph::Edge new_edge(real_from, real_to, dist);
		(*_nav_graph).addEdge(new_edge);

		//if graph is not a diagraph then an edge needs to be added going
		//in the other direction
		if (!(*_nav_graph).isDigraph())
		{
			NavGraph::Edge new_edge(real_to, real_from, dist);
			(*_nav_graph).addEdge(new_edge);
		}
	}
}

void  GameMap::createTriggers()
{
	auto trigger_group = _tile_map->getObjectGroup("triggers");
	for (auto iter = std::begin(trigger_group->getObjects());
	iter != std::end(trigger_group->getObjects()); iter++)
	{
		std::string name = iter->asValueMap()["name"].asString();
		float width = iter->asValueMap()["width"].asFloat();
		float height = iter->asValueMap()["height"].asFloat();

		float x = iter->asValueMap()["x"].asFloat() + width * 0.5f;
		float y = iter->asValueMap()["y"].asFloat()
			- getSizeY() + _tile_map->getTileSize().height + height * 0.5f;
		float radius = width / 2;

		std::string type = iter->asValueMap()["type"].asString();

		if (type == "Generate")
		{
			int team = iter->asValueMap()["team"].asInt();
			std::string line_type = iter->asValueMap()["line_type"].asString();
			std::string character_name = iter->asValueMap()["character"].asString();
			std::string character_type = iter->asValueMap()["character_type"].asString();
			double respawn_delay = iter->asValueMap()["respawn_delay"].asFloat();

			int line_type_enum = BaseGameEntity::LINE_CHARACTER;

			if (character_type == "LINE_CHARACTER")
			{
				line_type_enum = BaseGameEntity::LINE_CHARACTER;
			}
			else if (character_type == "TOWER_CHARACTER")
			{
				line_type_enum = BaseGameEntity::TOWER_CHARACTER;
			}

			if (line_type == "line_top")
			{
				_trigger_system->registerTrigger(
					new TriggerSpawnLine(
						_game_world, new geometry::Circle(Vec2(x, y), radius),
						name, Vec2(x, y), 0, 0,
						character_name, line_type_enum, respawn_delay, team, LINE_TOP));
			}
			else if (line_type == "line_bottom")
			{
				_trigger_system->registerTrigger(
					new TriggerSpawnLine(
						_game_world, new geometry::Rect(Vec2(x, y), Vec2(x + width, y + height)),
						name, Vec2(x, y), 0, 0,
						character_name, line_type_enum, respawn_delay, team, LINE_BOTTOM));
			}
		}
		else if(type == "HealthGiver")
		{
			int team = iter->asValueMap()["team"].asInt();
			int hp_gain = iter->asValueMap()["hp_gain"].asInt();
			int mp_gain = iter->asValueMap()["mp_gain"].asInt();
			double respawn_delay = iter->asValueMap()["respawn_delay"].asFloat();

			_trigger_system->registerTrigger(
				new TriggerHealthGiver(
					_game_world, new geometry::Circle(Vec2(x, y), radius),
					name, Vec2(x, y), radius, 0,
					hp_gain, mp_gain, respawn_delay, team));
		}
		else if(type == "GenerateJungle")
		{
			int team = iter->asValueMap()["team"].asInt();
			std::string character_name = iter->asValueMap()["character"].asString();
			std::string character_type = iter->asValueMap()["character_type"].asString();
			double respawn_delay = iter->asValueMap()["respawn_delay"].asFloat();

			_trigger_system->registerTrigger(
				new TriggerSpawnJungle(
					_game_world, new geometry::Rect(Vec2(x, y), Vec2(x + width, y + height)),
					name, Vec2(x, y), 0, 0,
					character_name, BaseGameEntity::JUNGLE_CHARACTER, respawn_delay, team));
		}
	}
}


void  GameMap::tagObstaclesWithinViewRange(BaseGameEntity* const entity, double range)
{
	tagNeighbors(entity, _collision_areas, range);
}

void GameMap::render()
{
	for (auto iter = std::begin(_collision_areas); 
		iter != std::end(_collision_areas); iter++)
	{
		(*iter)->render();
	}

	for (auto iter = std::begin(_structures); iter != std::end(_structures); iter++)
	{
		(*iter)->render();
	}

	if(Prm.RenderCellSpaceType == "agent")
		_agent_cell_space->render();

	else if(Prm.RenderCellSpaceType == "graph")
		_graph_cell_space->render();

	else if (Prm.RenderCellSpaceType == "collision")
		_collision_area_cell_space->render();

	_trigger_system->render();
}


//-------------------------- partitionNavGraph -----------------------------
//-----------------------------------------------------------------------------
void GameMap::partitionNavGraph()
{
	CCLOG("graph : %d %d %d",
		Prm.NumCellsX,
		Prm.NumCellsY,
		_nav_graph->numNodes());

	_graph_cell_space.reset(new GraphCellSpace(
		_sizeX,
		_sizeY,
		Prm.NumCellsX,
		Prm.NumCellsY,
		_nav_graph->numNodes()));

	//add the graph nodes to the space partition
	NavGraph::NodeIterator node_iter(*_nav_graph);
	for (NavGraph::Node* node = node_iter.begin();
		!node_iter.end();
		node = node_iter.next())
	{
		_graph_cell_space->addEntity(node);
	}
}

//-------------------------- partitionCollisionArea -----------------------------
//-----------------------------------------------------------------------------
void GameMap::partitionCollisionArea()
{
	CCLOG("collision : %d %d %d",
		Prm.NumCellsX,
		Prm.NumCellsY,
		_collision_areas.size() * 2);

	_collision_area_cell_space.reset(new CollisionAreaCellSpace(
		_sizeX,
		_sizeY,
		Prm.NumCellsX,
		Prm.NumCellsY,
		//Collision areas are spacious so it can be in many cells.
		//So when calculate neighbors of the area, its size can be
		//bigger than normal size.
		//So give cellspace enough spaces for neighborhoods.
		_collision_areas.size() * 2));

	for (auto iter = std::begin(_collision_areas); 
		iter != std::end(_collision_areas); iter++)
	{
		_collision_area_cell_space->addEntityByArea(
			(*iter).get(), (*iter)->getShape().get());
	}
}

void GameMap::updateTriggerSystem(
	std::vector<std::unique_ptr<AbstCharacter> >& characters,
	double time_elapsed)
{
	_trigger_system->update(characters, time_elapsed);
}


float GameMap::getSizeX()const
{
	return _sizeX;
}

float GameMap::getSizeY()const
{
	return _sizeY;
}

GameWorld* GameMap::getGameWorld() const
{
	return _game_world;
}

const GameMap::CollisionAreaVector& GameMap::getCollisionAreas() const
{
	return _collision_areas;
}

const GameMap::StructureVector& GameMap::getStructures() const
{
	return _structures;
}

cocos2d::TMXTiledMap* GameMap::getTileMap() const
{
	return _tile_map;
}

double GameMap::getCellSpaceNeighborhoodRange() const
{
	return _cellspace_neighbor_range;
}

const std::unique_ptr<GameMap::AgentCellSpace>& GameMap::getAgentCellSpace() const
{
	return _agent_cell_space;
}

const std::unique_ptr<GameMap::GraphCellSpace>& GameMap::getGraphCellSpace() const
{
	return _graph_cell_space;
}

const std::unique_ptr<GameMap::CollisionAreaCellSpace>&
GameMap::getCollisionAreaCellSpace() const
{
	return _collision_area_cell_space;
}

const std::unique_ptr<GameMap::NavGraph>& GameMap::getNavGraph()const
{
	return _nav_graph;
}
