#include "Network\GameServer\GameServerManager.h"
#include "Network\Packet\PacketFactory.h"

#include <list>
#include "BaseCharacter.h"
#include "GlobalValues.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "Util.h"
#include "Message/Telegram.h"
#include "Motion.h"
#include "SteeringBehavior.h"
#include "EntityFunctionTemplates.h"
#include "SensoryMemory.h"
#include "EntityManager.h"
#include "Message/MessageDispatcher.h"
#include "AbstTargetingSystem.h"
#include "Goals/GoalThink.h"
#include "Goals\JungleGoalThink.h"
#include "Goals\LineGoalThink.h"
#include "Goals\TowerGoalThink.h"
#include "AbstWeaponSystem.h"
#include "Database.h"
#include "UserCharacter.h"

USING_NS_CC;

BaseCharacter::BaseCharacter(
	const std::string&			name,
	GameWorld* const			world,
	int							type,
	const cocos2d::Vec2&		position,
	int							team,
	int							line,
	double						rotation,
	double						mass,
	double						max_force,
	double						max_turn_rate,
	double						bounding_radius,
	int							max_health,
	int							max_mana,
	int							damage,
	int							mdamage,
	int							defence,
	int							mdefence,
	double						attack_range,
	double						view_range,
	double						move_speed,
	double						attack_speed,
	int							gold_worth,
	int							exp_worth)
	:
	AbstCharacter(
		name,
		world,
		type,
		position,
		team,
		line,
		rotation,
		mass,
		max_force,
		max_turn_rate,
		bounding_radius,
		max_health,
		max_mana,
		damage,
		mdamage,
		defence,
		mdefence,
		attack_range,
		view_range,
		move_speed,
		attack_speed,
		gold_worth,
		exp_worth)
{

	_steering.reset(new SteeringBehavior(world, this));
	_steering->setViewDistance(view_range);

	_path_planner.reset(new PathPlanner(this));
	
	if(type == BaseGameEntity::JUNGLE_CHARACTER)
		_brain.reset(new JungleGoalThink(this, position));
	else if(type == BaseGameEntity::LINE_CHARACTER)
		_brain.reset(new LineGoalThink(this));
	else if(type == BaseGameEntity::TOWER_CHARACTER)
		_brain.reset(new TowerGoalThink(this));

	_target_system.reset(new AbstTargetingSystem(this));
	_sensory_memory.reset(new SensoryMemory(this, 5));

	_weapon_system.reset(
		new AbstWeaponSystem(
			this,
			0.2,
			std::get<0>(DB.getCharacterSkill()[_name]),
			std::get<1>(DB.getCharacterSkill()[_name]),
			std::get<2>(DB.getCharacterSkill()[_name]),
			std::get<3>(DB.getCharacterSkill()[_name]),
			std::get<4>(DB.getCharacterSkill()[_name])
			));

	_goal_arbitration_regulator.reset(new Regulator(4));
	_trigger_test_regulator.reset(new Regulator(8));
	_target_selection_regulator.reset(new Regulator(2));
	_vision_update_regulator.reset(new Regulator(4));
}

BaseCharacter* BaseCharacter::create(
	GameWorld* const		world,
	const std::string&		cname,
	int						type,
	const cocos2d::Vec2&	position,
	int						team,
	int						line)
{
	int hp = std::get<0>(DB.getCharacterData()[cname]);
	int mp = std::get<1>(DB.getCharacterData()[cname]);
	int damage = std::get<2>(DB.getCharacterData()[cname]);
	int mdamage = std::get<3>(DB.getCharacterData()[cname]);
	int defence = std::get<4>(DB.getCharacterData()[cname]);
	int mdefence = std::get<5>(DB.getCharacterData()[cname]);
	double attack_range = std::get<6>(DB.getCharacterData()[cname]);
	double view_range = std::get<7>(DB.getCharacterData()[cname]);
	double move_speed = std::get<8>(DB.getCharacterData()[cname]);
	double attack_speed = std::get<9>(DB.getCharacterData()[cname]);
	int gold_worth = std::get<10>(DB.getCharacterData()[cname]);
	int exp_worth = std::get<11>(DB.getCharacterData()[cname]);

	CCLOG("%s %d %d %d %d %d %d %lf %lf %lf %lf %d %d",
		cname.c_str(), hp, mp, damage, mdamage, defence, mdefence, attack_range, view_range,
		move_speed, attack_speed, gold_worth, exp_worth);

	auto temp =  new BaseCharacter(
		cname, 
		world, 
		type,  
		position, 
		team,
		line,
		util::genRand<0, 1>() * M_PI * 2, // rotation
		Prm.EntityMass, // mass
		Prm.SteeringForce * Prm.SteeringForceTweaker, // max_force
		M_PI / 2, // turn rate
		Prm.EntityRadius, // bounding radius
		hp, 
		mp,  
		damage,  
		mdamage,
		defence,
		mdefence,
		attack_range,
		view_range,
		move_speed,
		attack_speed,
		gold_worth,
		exp_worth);

	return temp;
}


BaseCharacter::~BaseCharacter()
{}

void BaseCharacter::update(double time_elapsed)
{
	int begin = clock();
	_brain->process();
	int brain = clock();

	if (isReadyForGoalArbitration() && isAlive())
	{
		_brain->arbitrate();

		if (_target_selection_regulator->isReady())
		{
			_target_system->update();
		}

		//update the sensory memory with any visual stimulus
		if (_vision_update_regulator->isReady())
		{
			_sensory_memory->updateVision();
		}
	}
	int others = clock();
	updateMovement(time_elapsed);
	int movement = clock();

	updateBuffNerf();
}

bool BaseCharacter::handleMessage(const Telegram& msg)
{
	int damage = 0;
	//first see if the current goal accepts the message
	if (getBrain()->handleMessage(msg)) return true;

	//handle any messages not handles by the goals
	switch (msg.msg)
	{
	case MSG_GET_DAMAGED:  // 데미지 관련

		if (isAlive())
		{
			damage = dereferenceToType<int>(msg.extra_info);

			//the extra info field of the telegram carries the amount of damage
			AbstCharacter* attacker = (AbstCharacter*)EntityMgr.getEntityFromID(msg.sender);
			getSensoryMemory()->updateWithDamageSource(attacker, damage);

			reduceHealth(dereferenceToType<int>(msg.extra_info));

			//if this bot is now dead let the shooter know
			if (!isAlive())
			{
				if (getName() == "Diablo")
				{
					Dispatcher.dispatchMsg(
						SEND_MSG_IMMEDIATELY,
						getId(),
						msg.sender,
						MSG_GAME_OVER,
						(void*)0);
				}

				int gold_worth = getGoldWorth();
				Dispatcher.dispatchMsg(
					SEND_MSG_IMMEDIATELY,
					getId(),
					msg.sender,
					MSG_GIVE_GOLD,
					(void*)&gold_worth);  // 내 가치를 죽인애한테 gold로 준다.

				for (auto iter = std::begin(_game_world->getUserCharacters());
				iter != std::end(_game_world->getUserCharacters()); iter++)
				{
					if (getTeam() != iter->second->getTeam() &&
						util::dis2(getPos().x, getPos().y, iter->second->getPos().x, iter->second->getPos().y) 
						<= Prm.ExpGiveRange * Prm.ExpGiveRange)
					{
						int exp = getExpWorth();
						Dispatcher.dispatchMsg(
							SEND_MSG_IMMEDIATELY,
							getId(),
							iter->second->getId(),
							MSG_GIVE_EXP,
							(void*)&exp);
					}
				}
			}

			// hit animation
			float noise_x = util::genRand(0, 30) - 15;
			float noise_y = util::genRand(0, 30) - 15;
			_game_world->getAnimationQ()
				.push(std::make_tuple("damage", _pos.x + noise_x, _pos.y + noise_y));
		}

		return true;
		
	case MSG_YOU_GOT_ME:  // 너가 날 죽였다.

		setScore(getScore() + 1);

		//the bot this bot has just killed should be removed as the target
		_target_system->clearTarget();

		return true;
		
	case MSG_GUN_SHOT_SOUND:

		//add the source of this sound to the bot's percepts
		getSensoryMemory()->updateWithSoundSource((AbstCharacter*)msg.extra_info);

		return true;

	case MSG_USER_HAS_REMOVED_BOT:
	{

		AbstCharacter* removed_bot = (AbstCharacter*)msg.extra_info;

		getSensoryMemory()->removeBotFromMemory(removed_bot);

		//if the removed bot is the target, make sure the target is cleared
		if (removed_bot == getTargetSys()->getTarget())
		{
			getTargetSys()->clearTarget();
		}

		return true;
	}
	case MSG_GAME_OVER:
		//GameServerManager::getInstance().sendAll(
		//	PacketFactory::getInstance().createPacketGameOver(_name, _team));
		return true;


	default: return false;
	}
}


