#ifndef BASE_GAME_ENTITY_H
#define BASE_GAME_ENTITY_H

//------------------------------------------------------------------------
//
//  Name:	BaseGameEntity.h
//
//  Desc:	Base class to define a common interface for all game
//			entities.
//			This class has three main methods,
//			update => every logic for this entity
//			render => every rendering changes for this entity
//			handleMessage => exchange messages with other entities 
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <vector>
#include <string>
#include <iosfwd>
#include "cocos2d.h"
#include "boost\bimap.hpp"
#include "boost\assign.hpp"
#include "Geometry\Geometry.h"

struct Telegram;
class GameWorld;

class BaseGameEntity 
{
public:
	enum entity_type 
	{
		LINE_CHARACTER,
		TOWER_CHARACTER,
		JUNGLE_CHARACTER,
		USER_CHARACTER,
		BUILDING,
		STRUCTURE,
		MAP,
		ITEM,
		SKILL,
		WALL,
		INVERTEDAABBOX2D,
		TRIGGER
	};

	enum Status { ALIVE, DEAD, DYING };

	typedef boost::bimaps::bimap< entity_type, std::string > bm_entity_type;
	static const bm_entity_type entity_type_to_string;

	//getters && setters
	GameWorld*						getWorld() const;
	const std::unique_ptr<geometry::Shape>&	getShape() const;
	unsigned int					getId() const;

	const cocos2d::Vec2&			getPos() const;
	void							setPos(const cocos2d::Vec2& new_pos);
	void							movePos(cocos2d::Vec2 move_pos);

	double							getBRadius() const;
	void							setBRadius(double r);

	const bool						isTagged() const;
	void							tag();
	void							unTag();

	int								getType() const;
	void							setType(int new_type);

	const std::string&				getName() const;
	void							setName(const std::string& name);

	bool							isDead() const;
	bool							isAlive() const;
	void							setDead();
	void							setDying();

	int getTeam() const;
	void setTeam(int team);

	//virtual functions
	virtual void					update(double time_elapsed) = 0;
	virtual void					render() = 0;
	virtual bool					handleMessage(const Telegram& msg) = 0;

protected:
	//A pointer for reference the world data.
	//So a vehicle can access any obstacle, path, wall or agent data.
	//Weak reference for remove circular reference.
	GameWorld* const	_game_world;

	//its location in the environment
	cocos2d::Vec2	_pos;

	//each entity has a unique ID
	unsigned int	_id;

	//every entity has a type associated with it (health, troll, ammo etc)
	int				_type;

	//this is a generic flag. 
	bool			_tag;

	//entity name
	std::string		_name;

	//the magnitude of this object's bounding radius
	double			_bounding_radius;

	std::unique_ptr<geometry::Shape> _bounding_shape;

	//ALIVE, DEAD or DYING?
	Status						_status;

	int							_team;


	BaseGameEntity::BaseGameEntity(
		GameWorld* const world,
		geometry::Shape* const bounding_shape,
		int type,
		const std::string& name,
		double bradius,
		const cocos2d::Vec2& pos);

	virtual ~BaseGameEntity();

private:
	BaseGameEntity(const BaseGameEntity&) = delete; // no copies
	BaseGameEntity& operator=(const BaseGameEntity&) = delete; // no self-assignments
	BaseGameEntity() = delete;
};




#endif




