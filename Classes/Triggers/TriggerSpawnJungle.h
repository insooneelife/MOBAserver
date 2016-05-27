#ifndef TRIGGER_SPAWN_JUNGLE_H
#define TRIGGER_SPAWN_JUNGLE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:     TriggerSpawnLine.h
//
//  Author:   Insub Im(insooneelife@naver.com)
//
//  Desc:     This trigger generates Jungle Goal Character with given period. 
//
//-----------------------------------------------------------------------------
#include "Triggers/TriggerRespawning.h"
#include "../AbstCharacter.h"
#include "cocos2d.h"

class GameWorld;

class TriggerSpawnJungle : public TriggerRespawning<AbstCharacter>
{
public:
	TriggerSpawnJungle(
		GameWorld* const world,
		geometry::Shape* const bounding_shape,
		const std::string& name,
		const cocos2d::Vec2& pos,
		double radius,
		int graph_node_index,
		std::string spawn_character_name,
		int spawn_type,
		double num_update_for_respawn,
		int team);

	virtual ~TriggerSpawnJungle() override;

	//methods
	//if triggered, the bot's health will be incremented
	virtual void      actIfOverlap(AbstCharacter* bot) override;

	//virtual functions

	virtual void      update(double time_elapsed) override;

	//draws a box with a red cross at the trigger's location
	virtual void      render() override;

private:

	const cocos2d::Vec2 _pos;

	int _team;

	std::string _spawn_character_name;

	int _spawn_type;

	unsigned int _spawned_character_ID;

};



#endif