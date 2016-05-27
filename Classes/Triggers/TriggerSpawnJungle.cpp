#include "TriggerSpawnJungle.h"
#include "EntityManager.h"

USING_NS_CC;

TriggerSpawnJungle::TriggerSpawnJungle(
	GameWorld* const world,
	geometry::Shape* const bounding_shape,
	const std::string& name,
	const Vec2&   pos,
	double radius,
	int graph_node_index,
	std::string spawn_character_name,
	int spawn_type,
	double num_update_for_respawn,
	int team
	)
	:
	TriggerRespawning<AbstCharacter>(world, bounding_shape),
	_pos(pos),
	_team(team),
	_spawn_character_name(spawn_character_name),
	_spawn_type(spawn_type),
	_spawned_character_ID(0)
{
	CCLOG("SPAWN CREATE\n");

	setName(name);
	setPos(pos);
	setBRadius(radius);
	setGraphNodeIndex(graph_node_index);
	setRespawnDelay(num_update_for_respawn);  // respawn delay.
}

TriggerSpawnJungle::~TriggerSpawnJungle()
{}

void TriggerSpawnJungle::update(double time_elapsed)
{
	bool exist = EntityMgr.exists(_spawned_character_ID);

	if (!exist)
	{
		if (isActive())
		{
			_spawned_character_ID
				= _world->createCharacter(_spawn_character_name, _spawn_type, _pos, _team, LINE_NONE);
			deactivate();
		}
		TriggerRespawning::update(time_elapsed);
	}
}

void TriggerSpawnJungle::actIfOverlap(AbstCharacter* bot)
{
	// do nothing in this trigger

   /*   if (isActive() && isTouchingTrigger(bot->getPos(), bot->getBoundingRadius()))
	{
	   //put event in here

	   deactivate();
	}
	*/
}

void TriggerSpawnJungle::render()
{
}