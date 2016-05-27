#include "MeleeWeapon.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "Message\MessageDispatcher.h"

USING_NS_CC;

//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
MeleeWeapon::MeleeWeapon(AbstCharacter* owner) 
	:
	AbstWeapon(
		1.5,
		150,
		50,
		owner)
{}


//------------------------------ attack --------------------------------------
//-----------------------------------------------------------------------------
void MeleeWeapon::attack(Vec2 pos)
{
	if (isReadyForNextShot())
	{
		_owner->rotateFacingTowardPosition(pos);

		//_owner->getWorld()->createSkill(_owner, pos, _type);

		updateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		//_owner->getWorld()->getGameMap()->addSoundTrigger(_owner, 400);
	}
}

void MeleeWeapon::attack()
{
	if (!_owner->getTargetSys()->isTargetPresent())
		return;

	if (isReadyForNextShot())
	{
		_owner->rotateFacingTowardPosition(_owner->getTargetSys()->getTarget()->getPos());

		int damage = _owner->getDamage();
		Dispatcher.dispatchMsg(
		SEND_MSG_IMMEDIATELY,
		_owner->getId(),
		_owner->getTargetSys()->getTarget()->getId(),
		MSG_GET_DAMAGED,
		(void*)&damage);

		updateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		//_owner->getWorld()->getGameMap()->addSoundTrigger(_owner, 400);
	}
}