#ifndef TRIGGER_HEALTH_GIVER_H
#define TRIGGER_HEALTH_GIVER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:     TriggerHealthGiver.h
//
//  Author:   Mat Buckland
//
//  Desc:     If a bot runs over an instance of this class its health is
//            increased. 
//
//-----------------------------------------------------------------------------
#include "Triggers/TriggerRespawning.h"
#include "../AbstCharacter.h"
#include "cocos2d.h"

class GameWorld;

class TriggerHealthGiver : public TriggerRespawning<AbstCharacter>
{
public:
	TriggerHealthGiver(
		GameWorld* const world,
		geometry::Shape* const bounding_shape,
		const std::string& name,
		const cocos2d::Vec2& pos,
		double radius,
		int graph_node_index,
		int hp_gain,
		int mp_gain,
		int respawn_delay,
		int team);

	virtual ~TriggerHealthGiver() override;

	//methods
	//if triggered, the bot's health will be incremented
	virtual void      actIfOverlap(AbstCharacter* bot) override;

	//virtual functions

	virtual void      update(double time_elapsed) override;

	//draws a box with a red cross at the trigger's location
	virtual void      render() override;

private:

	//the amount of health an entity receives when it runs over this trigger
	int               _hp_gain;
	int               _mp_gain;
	int               _respawn_delay;
	int               _team;
};



#endif