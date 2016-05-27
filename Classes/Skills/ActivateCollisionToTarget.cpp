#include "ActivateCollisionToTarget.h"
#include "../AbstCharacter.h"
#include "../CollisionTestTemplates.h"

ActivateCollisionToTarget::ActivateCollisionToTarget(AbstSkill* const owner_skill)
	:
	ActivateStrategy(owner_skill),
	_down_casted_skill(dynamic_cast<AbstTargetSkill*>(owner_skill))
{}

//------------------------------ test -------------------------------------
//
//  This function checks if any collision occurs within target character. 
//-------------------------------------------------------------------------

bool ActivateCollisionToTarget::test()
{

	if ((_owner_skill->getPos() - _down_casted_skill->getTarget()->getPos()).getLength() <
		_owner_skill->getBRadius() + _down_casted_skill->getTarget()->getBRadius())
		return true;

	return false;

}