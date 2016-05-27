#include <algorithm>
#include "AbstWeaponSystem.h"
#include "RangeWeapon.h"
#include "MeleeWeapon.h"
#include "AbstCharacter.h"
#include "Util.h"
#include "GameWorld.h"
#include "Geometry\Transformations.h"

USING_NS_CC;

//------------------------- ctor ----------------------------------------------
//-----------------------------------------------------------------------------
AbstWeaponSystem::AbstWeaponSystem(
	AbstCharacter* owner,
	double reactionTime,
	const std::string& attack,
	const std::string& skill1,
	const std::string& skill2,
	const std::string& skill3,
	const std::string& skill4)
	:
	_owner(owner),
	_reaction_time(reactionTime),
	_current_weapon(nullptr)
{
	if (attack == "Melee")
		_current_weapon = new MeleeWeapon(_owner);
	else
		_current_weapon = new RangeWeapon(_owner, attack);

	_weapon_map[ATTACK].reset(_current_weapon);
	_weapon_map[SKILL1].reset(new RangeWeapon(_owner, skill1));
	_weapon_map[SKILL2].reset(new RangeWeapon(_owner, skill2));
	_weapon_map[SKILL3].reset(new RangeWeapon(_owner, skill3));
	_weapon_map[SKILL4].reset(new RangeWeapon(_owner, skill4));
}

//------------------------- dtor ----------------------------------------------
//-----------------------------------------------------------------------------
AbstWeaponSystem::~AbstWeaponSystem()
{}


//-------------------------------- selectWeapon -------------------------------
//
//-----------------------------------------------------------------------------
void AbstWeaponSystem::selectWeapon()
{

}

//------------------------- getWeaponFromInventory ----------------------------
//
//  returns a pointer to any matching weapon.
//
//  returns a null pointer if the weapon is not present
//-----------------------------------------------------------------------------
const std::unique_ptr<AbstWeapon>& AbstWeaponSystem::getWeaponFromInventory(CastingType weapon_type)
{
	return _weapon_map[weapon_type];
}

//--------------------------- changeWeapon ------------------------------------
void AbstWeaponSystem::changeWeapon(CastingType type)
{
	AbstWeapon* w = getWeaponFromInventory(type).get();

	if (w) _current_weapon = w;
}

//--------------------------- takeAimAndShoot ---------------------------------
//
//  this method aims the bots current weapon at the target (if there is a
//  target) and, if aimed correctly, fires a round
//-----------------------------------------------------------------------------
void AbstWeaponSystem::takeAimAndShoot()const
{
	//aim the weapon only if the current target is shootable or if it has only
	//very recently gone out of view (this latter condition is to ensure the 
	//weapon is aimed at the target even if it temporarily dodges behind a wall
	//or other cover)
	if (_owner->getTargetSys()->isTargetAttackable())
	{
		//the position the weapon will be aimed at
		Vec2 aiming_pos = _owner->getTargetSys()->getTarget()->getPos();

		//no need to predict movement, aim directly at target
		
		//if the weapon is aimed correctly and it has been in view for a period
		//longer than the bot's reaction time, shoot the weapon
		if (_owner->rotateFacingTowardPosition(aiming_pos) &&
			(_owner->getTargetSys()->getTimeTargetHasBeenVisible() > _reaction_time))
		{
			addNoiseToAim(aiming_pos);

			getCurrentWeapon()->attack(aiming_pos);
		}
	}

	//no target to shoot at so rotate facing to be parallel with the bot's
	//heading direction
	else
	{
		_owner->rotateFacingTowardPosition(_owner->getPos() + _owner->getHeading());
	}
}

//---------------------------- addNoiseToAim ----------------------------------
//
//  adds a random deviation to the firing angle not greater than _aim_accuracy 
//  rads
//-----------------------------------------------------------------------------
void AbstWeaponSystem::addNoiseToAim(Vec2& aiming_pos)const
{
	Vec2 toPos = aiming_pos - _owner->getPos();

	aiming_pos = toPos + _owner->getPos();
}


//---------------------------- attack ----------------------------------------
//
//  shoots the current weapon at the given position
//-----------------------------------------------------------------------------
void AbstWeaponSystem::attack(cocos2d::Vec2 pos)const
{
	getCurrentWeapon()->attack(pos);
}

void AbstWeaponSystem::attack() const
{
	getCurrentWeapon()->attack();
}
