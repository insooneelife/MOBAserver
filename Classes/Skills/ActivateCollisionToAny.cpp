#include "ActivateCollisionToAny.h"
#include "../AbstCharacter.h"
#include "../CollisionArea.h"
#include "../CollisionTestTemplates.h"

ActivateCollisionToAny::ActivateCollisionToAny(AbstSkill* const owner_skill)
	:
	ActivateStrategy(owner_skill)
{}

//------------------------------ test -------------------------------------
//
//  This function checks through cellspaces to find if any collision occurs 
//  within other characters and collision areas.
//-------------------------------------------------------------------------

bool ActivateCollisionToAny::test()
{
	if (collisionToOthersCellSpace(
		_owner_skill, 
		_owner_skill->getWorld()->getGameMap()->getAgentCellSpace(),
		_owner_skill->getShooterId()))
		return true;

	if (collisionToOthersCellSpace(
		_owner_skill,
		_owner_skill->getWorld()->getGameMap()->getCollisionAreaCellSpace()))
		return true;
		
	return false;

}