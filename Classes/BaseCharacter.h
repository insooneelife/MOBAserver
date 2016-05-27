#pragma once

#include <memory>
#include <string>
#include "AbstCharacter.h"

//------------------------------------------------------------------------
//
//  Name:   BaseCharacter.h
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

class BaseCharacter : public AbstCharacter 
{
public:

	BaseCharacter(
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

	static BaseCharacter* create(
		GameWorld* const		world,
		const std::string&		cname,
		int						type,
		const cocos2d::Vec2&	position,
		int						team,
		int						line);

	virtual ~BaseCharacter() override;

	//virtual functions
	virtual void	update(double time_elapsed) override;

	virtual bool	handleMessage(const Telegram& msg) override;

private:
	BaseCharacter(const BaseCharacter&) = delete; // no copies
	BaseCharacter& operator=(const BaseCharacter&) = delete; // no self-assignments
	BaseCharacter() = delete;

};




