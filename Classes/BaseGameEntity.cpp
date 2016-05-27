#include "BaseGameEntity.h"
#include "IDGenerator.h"
#include "EntityManager.h"
#include "Geometry\Geometry.h"

USING_NS_CC;

const BaseGameEntity::bm_entity_type BaseGameEntity::entity_type_to_string =
boost::assign::list_of< bm_entity_type::relation >
(LINE_CHARACTER, "LINE_CHARACTER")
(TOWER_CHARACTER, "TOWER_CHARACTER")
(JUNGLE_CHARACTER, "JUNGLE_CHARACTER")
(BUILDING, "BUILDING")
(STRUCTURE, "STRUCTURE")
(MAP, "MAP")
(ITEM, "ITEM")
(SKILL, "SKILL")
(WALL, "WALL");

BaseGameEntity::BaseGameEntity(
	GameWorld* const world,
	geometry::Shape* const bounding_shape,
	int type,
	const std::string& name,
	double bradius,
	const Vec2& pos)
	:
	_game_world(world),
	_id(IDGen.genID()),
	_type(type),
	_name(name),
	_tag(false),
	_bounding_radius(bradius),
	_pos(pos),
	_bounding_shape(bounding_shape),
	_status(ALIVE),
	_team(TEAM_NON)
{
	EntityMgr.registerEntity(this);
}

BaseGameEntity::~BaseGameEntity() 
{
	EntityMgr.removeEntity(this);
}


bool BaseGameEntity::handleMessage(const Telegram& msg) 
{ 
	return false; 
}


//getters & setters
GameWorld* BaseGameEntity::getWorld() const
{
	return _game_world;
}

const std::unique_ptr<geometry::Shape>&	BaseGameEntity::getShape() const
{
	return _bounding_shape;
}

unsigned int BaseGameEntity::getId() const
{
	return _id;
}

const Vec2& BaseGameEntity::getPos() const
{
	return _pos;
}

void BaseGameEntity::setPos(const Vec2& new_pos)
{
	_bounding_shape->move(new_pos - _pos);
	_pos = new_pos;
}

void BaseGameEntity::movePos(cocos2d::Vec2 move_pos)
{
	_pos += move_pos;
	_bounding_shape->move(move_pos);
}

double BaseGameEntity::getBRadius() const 
{
	return _bounding_radius;
}

void BaseGameEntity::setBRadius(double r) 
{
	_bounding_radius = r;
}

const bool BaseGameEntity::isTagged() const 
{
	return _tag;
}

void BaseGameEntity::tag()
{
	_tag = true;
}

void BaseGameEntity::unTag() 
{
	_tag = false;
}

int BaseGameEntity::getType() const 
{
	return _type;
}

const std::string& BaseGameEntity::getName() const {
	return _name;
}

void BaseGameEntity::setType(int new_type) 
{
	_type = new_type;
}

void BaseGameEntity::setName(const std::string& name) 
{
	_name = name;
}

bool BaseGameEntity::isDead()const
{
	return _status == DEAD;
}

bool BaseGameEntity::isAlive()const
{
	return _status == ALIVE;
}

void BaseGameEntity::setDead()
{
	_status = DEAD;
}
void BaseGameEntity::setDying()
{
	_status = DYING;
}

void BaseGameEntity::setTeam(int team)
{
	_team = team;
}

int BaseGameEntity::getTeam() const
{
	return _team;
}