#include "GameWorld.h"
#include "Util.h"
#include "GlobalValues.h"
#include "SteeringBehavior.h"
#include "cocos2d.h"
#include "EntityManager.h"
#include "BaseCharacter.h"
#include "UserCharacter.h"
#include "ParameterLoader.h"
#include "CollisionArea.h"
#include "GameMap.h"
#include "Timer\PrecisionTimer.h"
#include "EntityFunctionTemplates.h"
#include "CollisionTestTemplates.h"
#include "Goals\GoalThink.h"

#include "Skills\CollisionSplashShootingSkill.h"
#include "Skills\PenetrateShootingSkill.h"
#include "Skills\TargetSplashSkill.h"
#include "Skills\TargetSkill.h"
#include "Skills\TargetDirectContinuousSkill.h"
#include "Skills\TargetDirectSkill.h"
#include "Skills\SplashContinuousSkill.h"
#include "Skills\SplashDirect.h"

#include "AbstWeapon.h"
#include "AbstWeaponSystem.h"
#include <list>

#include "Motion.h"
#include "Database.h"

#include "Network\GameServer\GameServerManager.h"
#include "Network\Packet\PacketFactory.h"

USING_NS_CC;


//------------------------------- ctor -----------------------------------
//------------------------------------------------------------------------
GameWorld::GameWorld()
	:
	_map(new GameMap(this)),
	_cross_hair(Vec2(0, 0)),
	_path_manager(new PathManager<PathPlanner>(1000))
{
	ParameterLoader::getInstance();
	PacketFactory::getInstance().setGameWorld(this);

	createDestinations();
	createTowers();
}


//-------------------------------- dtor ----------------------------------
//------------------------------------------------------------------------
GameWorld::~GameWorld() 
{
	CCLOG("~GameWorld()");
	PacketFactory::getInstance().setGameWorld(nullptr);
}

#include "IDGenerator.h"
//----------------------------- update -----------------------------------
//------------------------------------------------------------------------
void GameWorld::update(float time_elapsed) 
{
	int begin = clock();
	GameServerManager::getInstance().sendAll(
		PacketFactory::getInstance().createPacketRenderCharacterSet());

	GameServerManager::getInstance().sendAll(
		PacketFactory::getInstance().createPacketRenderSkillSet());

	GameServerManager::getInstance().update();

	int send_packets = clock();

	//update any current skills
	auto sk = _skills.begin();
	while (sk != _skills.end())
	{
		//test for any dead skills and remove them if necessary
		if (!(*sk)->isDead())
		{
			(*sk)->update(time_elapsed);
			++sk;
		}
		else
		{
			GameServerManager::getInstance().sendAll(
				PacketFactory::getInstance().createPacketDeleteSkill((*sk)->getId()));
			(*sk).reset();
			sk = _skills.erase(sk);
		}
	}

	//update characters
	auto ch = std::begin(_characters);
	while (ch != std::end(_characters))
	{
		//test for any dead characters and remove them if necessary
		if (!(*ch)->isDead())
		{
			(*ch)->update(time_elapsed);
			++ch;
		}
		else
		{
			GameServerManager::getInstance().sendAll(
				PacketFactory::getInstance().createPacketDeleteCharacter((*ch)->getId()));

			(*ch).reset();
			ch = _characters.erase(ch);
		}
	}
	
	int updates = clock();

	// send packets that creates animations to clients
	while (!_animation_que.empty())
	{
		std::string name = std::get<0>(_animation_que.front());
		float x = std::get<1>(_animation_que.front());
		float y = std::get<2>(_animation_que.front());

		_animation_que.pop();

		GameServerManager::getInstance().sendAll(
			PacketFactory::getInstance().createPacketCreateAnimationOnPos(name, x, y));
	}

	int animation = clock();
	//update all the queued searches in the path manager
	_path_manager->updateSearches();
	
	int pathfind = clock();
	_map->updateTriggerSystem(_characters, time_elapsed);

	int trigger = clock();

	//CCLOG("send_packets : %d   updates : %d  animation : %d  pathfind : %d  others : %d",
	//	send_packets - begin, updates - send_packets, animation - updates, 
	//	pathfind - animation, trigger - pathfind);
}

void GameWorld::render() 
{
	//render any skills
	for (auto sk = std::begin(_skills); sk != std::end(_skills); ++sk)
	{
		(*sk)->render();
	}

	//render characters
	for (auto ch = std::begin(_characters); ch != std::end(_characters); ch++)
	{
		(*ch)->render();
	}

	_map->render();
}


void GameWorld::worldDataToClient()
{
	GameServerManager::getInstance().sendAll(
		PacketFactory::getInstance().createPacketWorldData());

}


unsigned int GameWorld::createCharacter(
	std::string name, 
	int spawn_character_type,
	Vec2 pos,
	int team, 
	int line)
{
	AbstCharacter* temp_character = 
		BaseCharacter::create(this, name, spawn_character_type, pos, team, line);

	unsigned int ret_id = temp_character->getId();

	GameServerManager::getInstance().sendAll(
		PacketFactory::getInstance().createPacketCreateCharacter(
			temp_character->getId(),
			temp_character->getName(),
			temp_character->getPos().x,
			temp_character->getPos().y,
			-1,
			-1,
			temp_character->getBRadius()));

	//_map->getAgentCellSpace()->addEntity(temp_character);
	_characters.emplace_back(temp_character);

	return ret_id;
}

void GameWorld::createSkill(
	AbstCharacter* shooter,
	Vec2 target,
	const std::string& skill_name,
	int team)
{
	AbstSkill* skill = nullptr;

	auto data = DB.getSkillData()[skill_name];

	std::string type = std::get<0>(data);
	int damage = std::get<1>(data);
	int mana = std::get<2>(data);
	int radius = std::get<3>(data);
	int speed = std::get<4>(data);
	bool emitter_flag = std::get<5>(data);
	bool explosion_flag = std::get<6>(data);

	int period = std::get<7>(data);
	int splash_radius = std::get<8>(data);
	double survive_time = std::get<9>(data);

	int emitter_emission_rate = std::get<10>(data);
	double emitter_duration = std::get<11>(data);
	double emitter_scale = std::get<12>(data);
	int emitter_speed = std::get<13>(data);

	int explosion_emission_rate = std::get<14>(data);
	double explosion_duration = std::get<15>(data);
	double explosion_scale = std::get<16>(data);
	int explosion_speed = std::get<17>(data);

	CCLOG("non - target  skill : %s   type : %s", skill_name.c_str(), type.c_str());

	if (type == "SplashContinuous")
	{
		skill = new SplashContinuousSkill(
			shooter, target, period, damage, radius,
			speed, survive_time, splash_radius, skill_name, team);
	}
	else if (type == "Penetrate")
	{
		skill = new PenetrateShootingSkill(
			shooter, target,damage, radius, speed, survive_time, skill_name, team);
	}
	else if (type == "DirectSpread")
	{
		skill = new SplashDirect(shooter, shooter->getPos(), damage, radius, speed, survive_time, splash_radius, skill_name);
		//skill = new SplashDirect(shooter, target, damage, radius, speed, survive_time, skill_name);
	}
	else if (type == "TargetSkill")
	{
		skill = new PenetrateShootingSkill(
			shooter, target, damage, radius, speed, survive_time, skill_name, team);
	}
	else if (type == "CollisionSplashToAny")
	{
		skill = new CollisionSplashShootingSkill(
			shooter, target, splash_radius, damage, radius, speed, survive_time, skill_name, team);
	}

	if (skill == nullptr)
	{
		CCLOG("no skill type!!");
		return;
	}

	GameServerManager::getInstance().sendAll(
		PacketFactory::getInstance().createPacketCreateSkill(
			skill->getId(),
			skill->getName(),
			skill->getPos().x,
			skill->getPos().y,
			-1,
			-1,
			skill->getBRadius()));

	_skills.emplace_back(skill);
}


void GameWorld::createSkill(
	AbstCharacter* shooter,
	AbstCharacter* target,
	const std::string& skill_name,
	int team)
{
	AbstSkill* skill = nullptr;

	auto data = DB.getSkillData()[skill_name];

	std::string type = std::get<0>(data);
	int damage = std::get<1>(data);
	int mana = std::get<2>(data);
	int radius = std::get<3>(data);
	int speed = std::get<4>(data);
	bool emitter_flag = std::get<5>(data);
	bool explosion_flag = std::get<6>(data);

	int period = std::get<7>(data);
	int splash_radius = std::get<8>(data);
	double survive_time = std::get<9>(data);

	int emitter_emission_rate = std::get<10>(data);
	double emitter_duration = std::get<11>(data);
	double emitter_scale = std::get<12>(data);
	int emitter_speed = std::get<13>(data);

	int explosion_emission_rate = std::get<14>(data);
	double explosion_duration = std::get<15>(data);
	double explosion_scale = std::get<16>(data);
	int explosion_speed = std::get<17>(data);
	
	CCLOG("target  skill : %s   type : %s", skill_name.c_str(), type.c_str());
	
	if (type == "TargetSkill")
	{
		skill = new TargetSkill(
			shooter, target, splash_radius, damage, radius, speed, survive_time, skill_name, team);
	}
	else if (type == "TargetDirect")
	{
		skill = new TargetDirectSkill(
			shooter, target, splash_radius, damage, radius, speed, survive_time, skill_name, team);
	}
	else if (type == "TargetDirectContinuous")
	{
		skill = new TargetDirectContinuousSkill(
			shooter, target, period, damage, radius, 
			speed, survive_time, splash_radius, skill_name, team);
	}
	else if (type == "TargetSplash")
	{
		skill = new TargetSplashSkill(
			shooter, target, splash_radius, damage, radius, speed, survive_time, skill_name, team);
	}

	if (skill == nullptr)
	{
		CCLOG("no skill type!!");
		return;
	}

	GameServerManager::getInstance().sendAll(
		PacketFactory::getInstance().createPacketCreateSkill(
			skill->getId(),
			skill->getName(),
			skill->getPos().x,
			skill->getPos().y,
			-1,
			-1,
			skill->getBRadius()));

	_skills.emplace_back(skill);
}

void GameWorld::createUserCharacter(
	unsigned int id, 
	const std::string& character_name,
	int team,
	const std::string& client_name)
{
	auto iter = _user_characters.find(id);

	if (iter != std::end(_user_characters))
		return;

	Vec2 start_pos(util::genRand(0, 50) , util::genRand(0, 50));
	if (team == TEAM_BLUE)
		start_pos += Vec2(Prm.BlueStartX, Prm.BlueStartY);
	else if(team == TEAM_RED)
		start_pos += Vec2(Prm.RedStartX, Prm.RedStartY);

	auto temp_user =
		UserCharacter::create(
			this,
			character_name,
			BaseGameEntity::USER_CHARACTER,
			start_pos,
			team,
			client_name);

	_user_characters.insert(std::make_pair(id, temp_user));
	_characters.emplace_back(std::move(temp_user));
}

void GameWorld::createDestinations()
{
	//read destinations data from tmx file
	auto ob_group = _map->getTileMap()->getObjectGroup("destinations");
	CCASSERT(ob_group, "no object group destinations");

	for (auto iter = std::begin(ob_group->getObjects());
	iter != std::end(ob_group->getObjects()); iter++)
	{
		float width = iter->asValueMap()["width"].asFloat();
		float height = iter->asValueMap()["height"].asFloat();
		float x = iter->asValueMap()["x"].asFloat() + width * 0.5f;
		float y = iter->asValueMap()["y"].asFloat()
			- _map->getSizeY() + _map->getTileMap()->getTileSize().height + height * 0.5f;
		float radius = width / 2;

		std::string name = iter->asValueMap()["name"].asString();
		std::string line = iter->asValueMap()["line"].asString();
		int seq = iter->asValueMap()["sequence"].asInt();
		int team = iter->asValueMap()["team"].asInt();

		if (line == "top")
			_destinations[seq - 1][LINE_TOP][team] = geometry::Circle(Vec2(x, y), radius);
		else
			_destinations[seq - 1][LINE_BOTTOM][team] = geometry::Circle(Vec2(x, y), radius);

	}
}

void GameWorld::createTowers()
{
	//read structure data from tmx file
	auto ob_group = _map->getTileMap()->getObjectGroup("towers");
	CCASSERT(ob_group, "no object group structures");

	for (auto iter = std::begin(ob_group->getObjects());
	iter != std::end(ob_group->getObjects()); iter++)
	{
		float width = iter->asValueMap()["width"].asFloat();
		float height = iter->asValueMap()["height"].asFloat();
		float x = iter->asValueMap()["x"].asFloat() + width * 0.5f;
		float y = iter->asValueMap()["y"].asFloat()
			- _map->getSizeY() + _map->getTileMap()->getTileSize().height + height * 1.5f;

		float mx = iter->asValueMap()["marginX"].asFloat();
		float my = iter->asValueMap()["marginY"].asFloat();

		std::string name = iter->asValueMap()["name"].asString();
		float radius = iter->asValueMap()["radius"].asFloat();
		bool shadow = iter->asValueMap()["shadow"].asBool();
		int team = iter->asValueMap()["team"].asInt();

		_characters.emplace_back(
			BaseCharacter::create(
				this, name, BaseGameEntity::TOWER_CHARACTER, Vec2(x, y), team, LINE_BOTTOM));
	}
}


void GameWorld::tagVehiclesWithinViewRange(AbstCharacter* const entity, double range)
{
	tagNeighbors(entity, _characters, range);
}

void GameWorld::nonPenetrationContraint(AbstCharacter* const v)
{
	enforceNonPenetration(v, _characters);
}


//------------------------- isPathObstructed ----------------------------------
//
//  returns true if a bot cannot move from A to B without bumping into 
//  world geometry. It achieves this by stepping from A to B in steps of
//  size BoundingRadius and testing for intersection with world geometry at
//  each point.
//-----------------------------------------------------------------------------
bool GameWorld::isPathObstructed(
	Vec2 from,
	Vec2 to,
	double bounding_radius,
	Vec2& closest_pos) const
{
	Vec2 out1_p1, out1_p2, out2_p1, out2_p2;
	Vec2 dummy;
	Vec2 intersect_pos;
	bool flag = false;

	geometry::findTangentsInTwoCircle(
		from, bounding_radius, to, bounding_radius,
		out1_p1, out1_p2, out2_p1, out2_p2,
		dummy, dummy, dummy, dummy);

	for (auto iter = std::begin(_map->getCollisionAreas());
	iter != std::end(_map->getCollisionAreas()); iter++)
	{
		
		if (geometry::getSegmentCircleClosestIntersectPoint(
			out1_p1, out1_p2,
			(*iter)->getPos(),
			(*iter)->getBRadius(),
			intersect_pos))
		{
			if (util::dis2(closest_pos.x, closest_pos.y, from.x, from.y) >
				util::dis2(intersect_pos.x, intersect_pos.y, from.x, from.y))
			{
				closest_pos = intersect_pos;
			}
			flag = true;
		}

		if (geometry::getSegmentCircleClosestIntersectPoint(
			out2_p1, out2_p2,
			(*iter)->getPos(),
			(*iter)->getBRadius(),
			intersect_pos))
		{
			if (util::dis2(closest_pos.x, closest_pos.y, from.x, from.y) >
				util::dis2(intersect_pos.x, intersect_pos.y, from.x, from.y))
			{
				closest_pos = intersect_pos;
			}
			flag = true;
		}


		if (geometry::getSegmentCircleClosestIntersectPoint(
			from, to,
			(*iter)->getPos(),
			(*iter)->getBRadius(),
			intersect_pos))
		{
			if (util::dis2(closest_pos.x, closest_pos.y, from.x, from.y) >
				util::dis2(intersect_pos.x, intersect_pos.y, from.x, from.y))
			{
				closest_pos = intersect_pos;
			}
			flag = true;
		}

		//between


	}
	return flag;
}

//---------------------------- isLOSOkay --------------------------------------
//
//  returns true if the ray between A and B is unobstructed.
//------------------------------------------------------------------------------
bool GameWorld::isLOSOkay(Vec2 A, Vec2 B) const
{
	return !collisionToOthers(&geometry::Segment(A, B), _map->getCollisionAreas());
}


Vec2 GameWorld::closestPosToDestination(Vec2 source, Vec2 destination, float ent_rad)
{
	double min_dis_in_structures = std::numeric_limits<float>::max();
	Vec2 closest_pos_in_structures = destination;

	if (closestIntersectPointWithCollisionAreas(
		source,
		destination,
		min_dis_in_structures,
		closest_pos_in_structures,
		_map->getCollisionAreas()))
	{}

	Vec2 anti_heading = (source - destination).getNormalized();
	closest_pos_in_structures += anti_heading * ent_rad * 2;

	return closest_pos_in_structures;
}


const GameWorld::CharacterVector& GameWorld::getCharacters() const
{
	return _characters; 
}

const std::list<std::unique_ptr<AbstSkill> >& GameWorld::getSkills() const
{
	return _skills;
}

std::map<unsigned int, UserCharacter*>& GameWorld::getUserCharacters()
{
	return _user_characters;
}

const std::unique_ptr<GameMap>& GameWorld::getGameMap() const 
{
	return _map;
}

const std::unique_ptr<PathManager<PathPlanner> >& GameWorld::getPathManager() const
{
	return _path_manager;
}

std::queue<std::tuple<std::string, float, float> >& GameWorld::getAnimationQ()
{
	return _animation_que;
}

const geometry::Circle& GameWorld::getDestination(size_t i, int line, int team)
{
	return _destinations[i][line][team];
}

const cocos2d::Vec2& GameWorld::getCrossHair() const
{
	return _cross_hair; 
}

void GameWorld::setCrossHair(const cocos2d::Vec2& v) 
{
	_cross_hair = v; 
}


void GameWorld::clickRightMouseButton(
	unsigned int id, cocos2d::Vec2 p, int command, unsigned int target_id)
{
	std::cout << "server : clickRightMouseButton" << std::endl;

	auto iter = _user_characters.find(id);
	if (iter != std::end(_user_characters))
	{
		iter->second->getBrain()->removeAllSubgoals();
		iter->second->getBrain()->addGoal_MoveToPosition(p);
	}
	else
	{
		auto target = EntityMgr.getEntityFromID(target_id);
		if (target)
		{
			iter->second->getTargetSys()->setTarget(dynamic_cast<AbstCharacter*>(target));
			iter->second->getBrain()->removeAllSubgoals();
			iter->second->getBrain()->addGoal_AttackTarget();
		}
	}
}

void GameWorld::clickLeftMouseButton(
	unsigned int id, cocos2d::Vec2 p, int command, unsigned int target_id)
{
	std::cout << "server : clickLeftMouseButton  command : " << command << std::endl;

	auto iter = _user_characters.find(id);
	if (iter != std::end(_user_characters))
	{
		if (util::dis2(iter->second->getPos().x, iter->second->getPos().y, p.x, p.y) 
			<= iter->second->getSteering()->getViewDistance() 
			* iter->second->getSteering()->getViewDistance())
		{
			std::string name = iter->second->getName();
			auto data = DB.getCharacterSkill()[name];
			
			if (command == SKILL1)
			{
				std::string sname = std::get<1>(data);
				int m = std::get<2>(DB.getSkillData()[sname]);
				if (iter->second->getMana() >= m)
					iter->second->setMana(iter->second->getMana() - m);
				else
					return;
			}
			else if (command == SKILL2)
			{
				std::string sname = std::get<2>(data);
				int m = std::get<2>(DB.getSkillData()[sname]);
				if (iter->second->getMana() >= m)
					iter->second->setMana(iter->second->getMana() - m);
				else
					return;
			}
			else if (command == SKILL3)
			{
				std::string sname = std::get<3>(data);
				int m = std::get<2>(DB.getSkillData()[sname]);
				if (iter->second->getMana() >= m)
					iter->second->setMana(iter->second->getMana() - m);
				else
					return;
			}
			else if (command == SKILL4)
			{
				std::string sname = std::get<4>(data);
				int m = std::get<2>(DB.getSkillData()[sname]);
				if (iter->second->getMana() >= m)
					iter->second->setMana(iter->second->getMana() - m);
				else
					return;
			}
			
			if (iter->second->getWeaponSys()->getCurrentWeapon()->isReadyForNextShot())
			{
				iter->second->getBrain()->removeAllSubgoals();
				iter->second->getBrain()->addGoal_BasicCasting(p, (CastingType)command);
			}
			
		}
	}
	else
	{
		CCLOG("no user founded int map!!");
	}
}
