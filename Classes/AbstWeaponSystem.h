#ifndef ABSTWEAPONSYSTEM
#define ABSTWEAPONSYSTEM
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:	AbstWeaponSystem.h
//
//  Author:	Insub Im (insooneelife@naver.com)
//
//  Desc:   class to manage all different kind of spell action.
//
//-----------------------------------------------------------------------------

#include <map>
#include <memory>
#include <array>
#include "cocos2d.h"
#include "GlobalValues.h"

class AbstCharacter;
class AbstWeapon;


class AbstWeaponSystem
{
public:

	AbstWeaponSystem(
		AbstCharacter* owner,
		double reactionTime,
		const std::string& attack,
		const std::string& skill1,
		const std::string& skill2,
		const std::string& skill3,
		const std::string& skill4);

	~AbstWeaponSystem();

	//this method aims the bot's current weapon at the target 
	//(if there is a target) and, if aimed correctly, fires a round. (Called each update-step
	//from Raven_Bot::Update)
	virtual void	takeAimAndShoot()const;

	//this method determines the most appropriate weapon to use given the current
	//game state. (Called every n update-steps from Raven_Bot::Update)
	virtual void	selectWeapon();

	//changes the current weapon to one of the specified type 
	//(provided that type is in the bot's possession)
	void			changeWeapon(CastingType type);

	//attack the current weapon at the given position
	void			attack(cocos2d::Vec2 pos) const;

	//attack the current weapon at the given position
	void			attack() const;

	//returns a pointer to the current weapon
	AbstWeapon*	getCurrentWeapon() const { return _current_weapon; }

	//returns a pointer to the specified weapon type 
	//(if in inventory, null if not)
	const std::unique_ptr<AbstWeapon>&	getWeaponFromInventory(CastingType type);

	double			reactionTime() const { return _reaction_time; }

protected:

	AbstCharacter* const	_owner;

	//pointers to the weapons the bot is carrying 
	//(a bot may only carry one instance of each weapon)
	std::array<std::unique_ptr<AbstWeapon>, 5>		_weapon_map;

	//a pointer to the weapon the bot is currently holding
	AbstWeapon*	_current_weapon;

	//this is the minimum amount of time a bot needs to see an opponent before
	//it can react to it. This variable is used to prevent a bot shooting at
	//an opponent the instant it becomes visible.
	double			_reaction_time;

	//adds a random deviation to the firing angle not greater than _aim_accuracy rads
	void addNoiseToAim(cocos2d::Vec2& aiming_pos)const;

};

#endif