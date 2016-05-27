#ifndef GameWorld_H
#define GameWorld_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   GameWorld.h
//
//  Desc:   All the environment data and methods for the game 
//			entity behaviors. This class is the root of the project's
//          update and render calls.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------


#include <vector>
#include <memory>
#include "cocos2d.h"
#include "PathPlanner\PathManager.h"
#include "PathPlanner\PathPlanner.h"

class BaseGameEntity;
class Path;
class AbstCharacter;
class UserCharacter;
class CollisionArea;
class GameMap;
class AbstSkill;

class GameWorld {
public:
	typedef std::vector<std::unique_ptr<AbstCharacter> >	CharacterVector;

	GameWorld();

	~GameWorld();
	
	void					setCrossHair(const cocos2d::Vec2& v);

	const cocos2d::Vec2&	getCrossHair() const;

	const CharacterVector&								getCharacters() const;

	const std::list<std::unique_ptr<AbstSkill> >&		getSkills() const;

	std::map<unsigned int, UserCharacter*>&				getUserCharacters();

	const std::unique_ptr<GameMap>&						getGameMap() const;

	const std::unique_ptr<PathManager<PathPlanner> >&	getPathManager() const;

	std::queue<std::tuple<std::string, float, float> >& getAnimationQ();

	const geometry::Circle&								getDestination(size_t i, int line, int team);

	void update(float time_elapsed);

	void render();

	void nonPenetrationContraint(AbstCharacter* const v);

	void tagVehiclesWithinViewRange(AbstCharacter* const entity, double range);

	void tagObstaclesWithinViewRange(BaseGameEntity* const entity, double range);

	void worldDataToClient();

	bool isPathObstructed(
		cocos2d::Vec2 from,
		cocos2d::Vec2 to,
		double bounding_radius,
		cocos2d::Vec2& closest_pos) const;

	bool isLOSOkay(cocos2d::Vec2 A, cocos2d::Vec2 B) const;

	cocos2d::Vec2 closestPosToDestination(
		cocos2d::Vec2 source,
		cocos2d::Vec2 destination,
		float ent_rad = 0);

	void clickRightMouseButton(unsigned int id, cocos2d::Vec2 p, int command, unsigned int target_id);

	void clickLeftMouseButton(unsigned int id, cocos2d::Vec2 p, int command, unsigned int target_id);

	void createSkill(
		AbstCharacter* shooter,
		cocos2d::Vec2 target, 
		const std::string& skill_name,
		int team);

	void createSkill(
		AbstCharacter* shooter,
		AbstCharacter* target, 
		const std::string& skill_name,
		int team);

	unsigned int createCharacter(
		std::string name, int spawn_character_type, cocos2d::Vec2 pos, int team, int line);

	void createUserCharacter(
		unsigned int id,
		const std::string& character_name,
		int team,
		const std::string& client_name);

	void createDestinations();

	void createTowers();

private:
	GameWorld(const GameWorld&) = delete; // no copies
	GameWorld& operator=(const GameWorld&) = delete; // no self-assignments

	std::unique_ptr<GameMap>					_map;

	//this class manages all the path planning requests
	//note : if PathPlanner destructs before characters,
	//it will make problem in the destructor of PathPlanner 
	//which calls getReadyForNewSearch() => removes self from the PathManager.
	std::unique_ptr<PathManager<PathPlanner> >	_path_manager;

	//a container of all the moving entities
	CharacterVector				_characters;

	std::map<unsigned int, UserCharacter*>	_user_characters;

	std::list<std::unique_ptr<AbstSkill> >	_skills;

	std::queue<std::tuple<std::string, float, float> > _animation_que;

	//destinations
	std::array<std::array<std::array<geometry::Circle, 2>, 2>, 7>	_destinations;

	//the position of the crosshair
	cocos2d::Vec2				_cross_hair;
};



#endif