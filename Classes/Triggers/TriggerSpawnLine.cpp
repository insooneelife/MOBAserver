#include "TriggerSpawnLine.h"

USING_NS_CC;

TriggerSpawnLine::TriggerSpawnLine(
	GameWorld* const world,
	geometry::Shape* const bounding_shape,
	const std::string& name,
	const Vec2&   pos,
	double radius,
	int graph_node_index,
	std::string spawn_character_name,
	int spawn_character_type,
	double num_update_for_respawn,
	int team,
	int line)
	:
	TriggerRespawning<AbstCharacter>(world, bounding_shape),
	_pos(pos),
	_team(team),
	_spawn_character_name(spawn_character_name),
	_spawn_character_type(spawn_character_type),
	_line(line)
{
	CCLOG("SPAWN CREATE\n");

	setName(name);
	setPos(pos);
	setBRadius(radius);
	setGraphNodeIndex(graph_node_index);
	setRespawnDelay(num_update_for_respawn);  // respawn delay.
	setInactive();
}

TriggerSpawnLine::~TriggerSpawnLine()
{
	CCLOG("REMOVE SPAWN TRIGGER\n");
}

void TriggerSpawnLine::update(double time_elapsed)
{
	if (isActive())
	{
		_world->createCharacter(_spawn_character_name, _spawn_character_type, _pos, _team, _line);
		deactivate();
	}
	TriggerRespawning::update(time_elapsed);
}

void TriggerSpawnLine::actIfOverlap(AbstCharacter* bot)
{
	// do nothing in this trigger

	/*   if (isActive() && isTouchingTrigger(bot->getPos(), bot->getBoundingRadius()))
	{
	//put event in here

	deactivate();
	}
	*/
}

void TriggerSpawnLine::render()
{
}