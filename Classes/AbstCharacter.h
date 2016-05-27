#pragma once

#include <memory>
#include <string>
#include <queue>
#include "MovingEntity.h"
#include "PathPlanner/PathPlanner.h"
#include "Timer/Regulator.h"
#include "AbstTargetingSystem.h"

//------------------------------------------------------------------------
//
//  Name:   AbstCharacter.h
//
//  Desc:   This class is abstract class for game agent witch has
//			goal system				(GoalThink),
//			movements				(SteeringBehavior),
//			animations				(Motion),
//			rendering				(cocos2d Sprite)
//			and navigation system	(PathPlanner).
//			This class also has reference for GameWorld,
//			so it can get interact with other entities.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//--------------------------------------------------------------------------

class SteeringBehavior;
class GameWorld;
class Motion;
class GoalThink;
class SensoryMemory;
class AbstWeaponSystem;
class BuffNerf;

class AbstCharacter : public MovingEntity 
{
public:

	AbstCharacter(
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
		int							exp_worth);

	virtual ~AbstCharacter() override;

	//getters && setters

	const std::unique_ptr<SteeringBehavior>& getSteering() const;

	const std::unique_ptr<Motion>& getMotion() const;

	const std::unique_ptr<PathPlanner>& getPathPlanner() const;

	const std::unique_ptr<GoalThink>& getBrain() const;

	const std::unique_ptr<AbstTargetingSystem>& getTargetSys() const;

	const std::unique_ptr<SensoryMemory>& getSensoryMemory() const;

	const std::unique_ptr<AbstWeaponSystem>& getWeaponSys()const;

	const std::unique_ptr<BuffNerf>& getBuffsNerfs() const;
	
	int							getDirection() const;
	void						setDirection(int direction);

	const std::string&			getMotionName() const;
	void						setMotionName(const std::string& motion);

	bool						setMotionDirect(const std::string& motion);

	bool						isPossessed() const;
	bool						isAtPosition(cocos2d::Vec2 pos, float radius = 10) const;
	bool						isReadyForGoalArbitration() const;
	bool						isReadyForTriggerUpdate() const;

	LARGE_INTEGER				getTime() const;
	void						addTime(LONGLONG delta);
	void						addTime(LARGE_INTEGER delta);
	
	int getHealth() const;
	void setHealth(int health);
	int getMana() const;
	void setMana(int mana);
	int getMaxHealth() const;
	void setMaxHealth(int maxhealth);
	int getMaxMana() const;
	void setMaxMana(int max_mana);
	int getDamage() const;
	void setDamage(int power);
	int getMDamage() const;
	void setMDamage(int power);
	int getAttackSpeed() const;
	void setAttackSpeed(int attack_speed);

	int getScore() const;
	void setScore(int score);
	const cocos2d::Vec2& getFacing() const;
	void setFacing(const cocos2d::Vec2 facing);
	double getViewOnField() const;
	void setViewOnField(double view);
	int getNumHitUpdates() const;
	void setNumHitUpdates(int num_hit_updates);
	bool getHit() const;
	void setHit(bool hit);
	int getGold() const;
	void addGold(int gold);
	int getGoldWorth() const;
	void setGoldWorth(int gold_worth);
	int getExpWorth() const;
	void setExpWorth(int gold_worth);
	double getAttackRange() const;
	void setAttackRange(double val);
	int getDefence() const;
	void setDefence(int val);
	int getMDefence() const;
	void setMDefence(int val);

	int getPathNumber() const;
	void setPathNumber(int path_number);
	int getLine() const;
	void setLine(int line);

	int getBaseDamage() const;
	void setBaseDamage(int val);

	int getBaseMDamage() const;
	void setBaseMDamage(int val);

	int getBaseMaxHp() const;
	void setBaseMaxHp(int val);

	int getBaseMaxMp() const;
	void setBaseMaxMp(int val);

	int getBaseDefence() const;
	void setBaseDefence(int val);

	int getBaseMDefence() const;
	void setBaseMDefence(int val);

	int getBaseSpeed() const;
	void setBaseSpeed(int val);

	void reduceHealth(int damage);

	//make functions

	MovingEntity* findEnemyInRange();

	const cocos2d::Vec2 getAttackDirection() const;

	bool targetInRange(double range) const;

	bool inAttackRange() const;

	double calculateTimeToReachPosition(cocos2d::Vec2 pos)const;

	//virtual functions
	virtual void update(double time_elapsed) override;

	virtual void updateMovement(double time_elapsed) override;

	void updateBuffNerf();

	virtual void render() override;

	virtual bool handleMessage(const Telegram& telegram) override;

	//returns true if this bot can move directly to the given position
	//without bumping into any walls
	virtual bool canWalkTo(cocos2d::Vec2 pos) const;

	//similar to above. Returns true if the bot can move between the two
	//given positions without bumping into any walls
	virtual bool canWalkBetween(cocos2d::Vec2 from, cocos2d::Vec2 to) const;

	virtual bool rotateFacingTowardPosition(cocos2d::Vec2 target);

protected:
	AbstCharacter(const AbstCharacter&) = delete; // no copies
	AbstCharacter& operator=(const AbstCharacter&) = delete; // no self-assignments
	AbstCharacter() = delete;

	//this class makes path for target positions.
	std::unique_ptr<PathPlanner>			_path_planner;

	//this object handles the arbitration and processing of high level goals
	std::unique_ptr<GoalThink>				_brain;

	//Motion class for animate image frame.
	std::unique_ptr<Motion>					_motion;
	std::string								_motion_name;
	int										_direction;

	//This class makes appropriate moving force for current steering behaviors.
	std::unique_ptr<SteeringBehavior>		_steering;

	//this is a class that acts as the bots sensory memory. Whenever this
	//bot sees or hears an opponent, a record of the event is updated in the 
	//memory.
	std::unique_ptr<SensoryMemory>			_sensory_memory;

	//this is responsible for choosing the bot's current target
	std::unique_ptr<AbstTargetingSystem>	_target_system;

	std::unique_ptr<AbstWeaponSystem>		_weapon_system;

	std::unique_ptr<BuffNerf>				_buffs_nerfs;

	//These classes regulates the number of times for varius actions
	std::unique_ptr<Regulator>				_goal_arbitration_regulator;
	std::unique_ptr<Regulator>				_trigger_test_regulator;
	std::unique_ptr<Regulator>				_target_selection_regulator;
	std::unique_ptr<Regulator>				_vision_update_regulator;
	LARGE_INTEGER							_time;

	//set to true when a human player takes over control of the bot
	bool						_possessed;

	//the direction the bot is facing (and therefore the direction of aim). 
	//Note that this may not be the same as the bot's heading, which always
	//points in the direction of the bot's movement
	cocos2d::Vec2				_facing;

	//a bot only perceives other bots within this field of view
	double						_view_on_field;

	//to show that a player has been hit it is surrounded by a thick 
	//red circle for a fraction of a second. This variable represents
	//the number of update-steps the circle gets drawn
	int							_num_hit_updates;

	//set to true when the bot is hit, and remains true until 
	//m_iNumUpdatesHitPersistant becomes zero. (used by the render method to
	//draw a thick red circle around a bot to indicate it's been hit)
	bool						_hit;

	//the bot's health. Every time the bot is shot this value is decreased. If
	//it reaches zero then the bot dies (and respawns)
	int							_health;
	int							_mana;

	//the bot's maximum health value. It starts its life with health at this value
	int							_max_health;
	int							_max_mana;

	//the damage of character
	int							_damage;
	int							_mdamage;

	//each time this bot kills another this value is incremented
	int							_score;

	//speed values
	int							_attack_speed;

	int							_defence;
	int							_mdefence;

	double						_attack_range;

	// base attributes
	int							_base_damage;
	int							_base_mdamage;
	int							_base_max_hp;
	int							_base_max_mp;
	int							_base_defence;
	int							_base_mdefence;
	int							_base_speed;

	int							_gold_worth;
	int							_exp_worth;

	int							_line;
	int							_path_number;
};




