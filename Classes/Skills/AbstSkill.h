#ifndef ABST_SKILL_H
#define ABST_SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   AbstSkill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Base class to define skill type. 
//
//-----------------------------------------------------------------------------

#include <memory>
#include "MovingEntity.h"
#include "cocos2d.h"
#include "timer/CrudeTimer.h"

class GameWorld;
class ActivateStrategy;
class ImpactStrategy;
class AbstCharacter;

class AbstSkill : public MovingEntity
{
public:

	AbstSkill(
		GameWorld* const		world,		//a pointer to the world data
		cocos2d::Vec2			target_pos,
		int						shooter_id,	//the ID of the bot that spelled this skill
		cocos2d::Vec2			origin,		
		cocos2d::Vec2			heading,	//the heading of the skill
		int						damage,		//how much damage it inflicts
		float					radius,
		float					max_speed,
		float					mass,
		float					max_force,
		std::string				name,
		int						type,
		float					survive_time);

	virtual ~AbstSkill();


	//set to true if the skill has impacted and has finished any explosion 
	//sequence. When true the skill will be removed from the game
	bool isDead() const;
	void setDead();

	int getShooterId() const;

	cocos2d::Vec2 getTargetPos() const;

	cocos2d::Vec2 getOrigin() const;

	float getTimeOfCreation() const;

	float getSurviveTime() const;

	int getDamage() const;
	void setDamage(int damage);

	bool getActivated() const;
	void setActivated(bool activated);

	//must be implemented
	virtual void				update(double time_elapsed) = 0;

	virtual void				updateMovement(double time_elapsed) = 0;

	virtual void				render() = 0;

	virtual bool				handleMessage(const Telegram& telegram) = 0;

protected:

	//the strategy for activation
	std::unique_ptr<ActivateStrategy>	_activate_strategy;

	//the strategy for impacting
	std::unique_ptr<ImpactStrategy>		_impact_strategy;

	//the position of target
	cocos2d::Vec2		_target_pos;

	//the ID of the entity that fired this
	int					_shooter_id;

	cocos2d::Vec2		_origin;

	//is it dead? A dead collision skill is one that has come to the end of its
	//trajectory and cycled through any explosion sequence. A dead skill effect
	//can be removed from the world environment and deleted.
	bool				_dead;

	//this is stamped with the time this collision effect was instantiated.
	//This is to enable the shot to be rendered for a specific length of time
	float				_time_of_creation;

	float				_survive_time;

	//how much damage the skill inflicts
	int					_damage;

	bool _activated;
};

#endif