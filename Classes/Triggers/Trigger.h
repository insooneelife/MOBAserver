#ifndef TRIGGER_H
#define TRIGGER_H
//-----------------------------------------------------------------------------
//
//  Name:   Trigger.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   base class for a trigger. A trigger is an object that is
//          activated when an entity moves within its region of influence.
//
//-----------------------------------------------------------------------------
#include <memory>
#include "../BaseGameEntity.h"

struct Telegram;
class GameWorld;

template <class Entity>
class Trigger : public BaseGameEntity
{
public:
	Trigger(GameWorld* const world, geometry::Shape* const bounding_shape)
		:
		BaseGameEntity(
			world,
			bounding_shape,
			TRIGGER,
			"trigger",
			0,
			cocos2d::Vec2()),
		_remove_from_game(false),
		_active(true),
		_graph_node_index(-1),
		_world(world)
	{}

	virtual ~Trigger()
	{
#ifdef SHOW_DTOR_INFO
		CCLOG("~Trigger()");
#endif
	}

	//virtual functions

	//called each update-step of the game. This methods updates any internal
	//state the trigger may have
	virtual void update(double time_elapsed) = 0;

	//when this is called the trigger determines if the entity is within the
	//trigger's region of influence. If it is then the trigger will be 
	//triggered and the appropriate action will be taken.
	virtual void actIfOverlap(Entity*) = 0;

	//to be implemented by child classes
	virtual void render() = 0;

	//to be implemented by child classes
	virtual bool handleMessage(const Telegram& msg) = 0;

	int  graphNodeIndex()const { return _graph_node_index; }
	bool isToBeRemoved()const { return _remove_from_game; }
	bool isActive() { return _active; }

protected:

	void setGraphNodeIndex(int idx)	{ _graph_node_index = idx; }

	void setToBeRemovedFromGame()	{ _remove_from_game = true; }

	void setInactive()	{ _active = false; }
	void setActive()	{ _active = true; }

	//returns true if the entity given by a position and bounding radius is
	//overlapping the trigger region
	bool isTouchingTrigger(cocos2d::Vec2 entity_pos, float entity_radius)const;

	GameWorld* const _world;

private:

	//if this is true the trigger will be removed from the game
	bool		_remove_from_game;

	//it's convenient to be able to deactivate certain types of triggers
	//on an event. Therefore a trigger can only be triggered when this
	//value is true (respawning triggers make good use of this facility)
	bool		_active;

	//some types of trigger are twinned with a graph node. This enables
	//the pathfinding component of an AI to search a navgraph for a specific
	//type of trigger.
	int			_graph_node_index;
};


//--------------------- isTouchingTrigger -------------------------------------
//-----------------------------------------------------------------------------
template <class Entity>
bool Trigger<Entity>::isTouchingTrigger(
	cocos2d::Vec2 entity_pos,
	float entity_radius) const
{
	if (_bounding_shape.get())
	{
		return _bounding_shape->overlap(&geometry::Circle(entity_pos, entity_radius));
	}

	return false;
}


#endif