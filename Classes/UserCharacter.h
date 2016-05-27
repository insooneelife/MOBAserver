#pragma once

#include <memory>
#include <string>
#include "AbstCharacter.h"

//---------------------------------------------------------------------------
//
//  Name:   UserCharacter.h
//
//  Desc:   This class is describe the character which is controlled by user.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//---------------------------------------------------------------------------



class UserCharacter : public AbstCharacter 
{
public:
	UserCharacter(
		const std::string&			name,
		GameWorld* const			world,
		int							type,
		const cocos2d::Vec2&		position,
		int							team,
		const std::string&			client_name,
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

	static UserCharacter* create(
		GameWorld* const		world,
		const std::string&		cname,
		int						type,
		const cocos2d::Vec2&	position,
		int						team,
		const std::string&		client_name);

	void fireWeapon(cocos2d::Vec2 pos);

	virtual ~UserCharacter() override;

	const std::string& getClientName() const;

	void setClientName(const std::string& name);

	int getGold() const;

	void setGold(int gold);

	int getLevel() const;

	void setLevel(int level);

	int getExp() const;

	void setExp(int exp);

	void levelUp();

	//virtual functions
	virtual void	update(double time_elapsed) override;

	virtual bool	handleMessage(const Telegram& msg) override;

private:
	UserCharacter(const UserCharacter&) = delete; // no copies
	UserCharacter& operator=(const UserCharacter&) = delete; // no self-assignments
	UserCharacter() = delete;

	std::string		_client_name;
	int				_gold;
	int				_level;
	int				_exp;
	int            _level_up_need_exp;
};




