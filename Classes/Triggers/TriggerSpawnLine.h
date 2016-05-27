#ifndef TRIGGER_SPAWN_LINE_H
#define TRIGGER_SPAWN_LINE_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:     TriggerSpawnLine.h
//
//  Author:   Insub Im(insooneelife@naver.com)
//
//  Desc:     This trigger generates Line Goal Character with given period. 
//
//-----------------------------------------------------------------------------
#include "Triggers/TriggerRespawning.h"
#include "../AbstCharacter.h"
#include "cocos2d.h"

class GameWorld;

class TriggerSpawnLine : public TriggerRespawning<AbstCharacter>
{
public:
	TriggerSpawnLine(
		GameWorld* const world,
		geometry::Shape* const bounding_shape,
		const std::string& name,
		const cocos2d::Vec2& pos,
		double radius,
		int graph_node_index,
		std::string spawn_character_name,
		int spawn_character_type,
		double num_update_for_respawn,
		int team,
		int line);

	virtual ~TriggerSpawnLine() override;

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

	int _spawn_character_type;

	int _line;
};

#endif