#include "RangeWeapon.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "GameMap.h"

USING_NS_CC;

//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
RangeWeapon::RangeWeapon(AbstCharacter* owner, const std::string& name)
	:
	AbstWeapon(
		1.5,
		150,
		50,
		owner),
	_name(name)
{}

//------------------------------ attack --------------------------------------
//-----------------------------------------------------------------------------
void RangeWeapon::attack(Vec2 pos)
{
	if (isReadyForNextShot())
	{
		_owner->rotateFacingTowardPosition(pos);

		_owner->getWorld()->createSkill(_owner, pos, _name, _owner->getTeam());

		updateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		//_owner->getWorld()->getGameMap()->addSoundTrigger(_owner, 400);
	}
}

void RangeWeapon::attack()
{
	if (!_owner->getTargetSys()->isTargetPresent())
		return;

	if (isReadyForNextShot())
	{
		_owner->rotateFacingTowardPosition(_owner->getTargetSys()->getTarget()->getPos());

		_owner->getWorld()->createSkill(
			_owner, _owner->getTargetSys()->getTarget(), _name, _owner->getTeam());

		updateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		//_owner->getWorld()->getGameMap()->addSoundTrigger(_owner, 400);
	}
}