#include <sstream>
#include <fstream>
#include "TriggerHealthGiver.h"
#include "UserCharacter.h"

USING_NS_CC;

TriggerHealthGiver::TriggerHealthGiver(
	GameWorld* const world,
	geometry::Shape* const bounding_shape,
	const std::string& name,
	const Vec2&   pos,
	double radius,
	int graph_node_index,
	int hp_gain,
	int mp_gain,
	int respawn_delay,
	int team)
	:
	TriggerRespawning<AbstCharacter>(world, bounding_shape),
	_hp_gain(hp_gain),
	_mp_gain(mp_gain),
	_team(team),
	_respawn_delay(respawn_delay)
{
	setName(name);
	setPos(pos);
	setBRadius(radius);
	setGraphNodeIndex(graph_node_index);

	//create this trigger's region of fluence

	setRespawnDelay(respawn_delay);
	//setType(type_health);

	setActive();
	CCLOG("TRIGGER r: %f", _bounding_shape->boundingRadius());
}

TriggerHealthGiver::~TriggerHealthGiver()
{}
void TriggerHealthGiver::update(double time_elapsed)
{
	if (isActive())
	{
		for (auto iter = std::begin(_game_world->getUserCharacters());
		iter != std::end(_game_world->getUserCharacters()); iter++)
		{
			if (_team == iter->second->getTeam() &&
				util::dis2(getPos().x, getPos().y, iter->second->getPos().x, iter->second->getPos().y)
				<= _bounding_radius * _bounding_radius)
			{
				iter->second->setHealth(
					std::min(
						iter->second->getHealth() + _hp_gain,
						iter->second->getMaxHealth()));
				iter->second->setMana(
					std::min(
						iter->second->getMana() + _mp_gain,
						iter->second->getMaxMana()));
			}
		}
		deactivate();
	}
	TriggerRespawning::update(time_elapsed);
}

void TriggerHealthGiver::actIfOverlap(AbstCharacter* bot)
{
}

void TriggerHealthGiver::render()
{

}