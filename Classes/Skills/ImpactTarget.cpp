#include "ImpactTarget.h"
#include "EntityManager.h"
#include "../CollisionTestTemplates.h"
#include "../AbstCharacter.h"
#include "../Message/MessageDispatcher.h"

ImpactTarget::ImpactTarget(
	AbstSkill* const owner_skill,
	float splash_radius,
	AbstCharacter* shooter,
	AbstCharacter* target)
	:
	ImpactStrategy(owner_skill),
	_splash_radius(splash_radius),
	_impacted(false),
	_shooter(shooter),
	_target(target)
{
	CCASSERT(target != nullptr, "target is nullptr!!");
	if (!target->isAlive())
		owner_skill->setDead();
}


//----------------------------- impact ------------------------------------
//
//  This iterate through agent(character) cellspaces and give messages 
//  to them with damage info if they overlap.
//-------------------------------------------------------------------------

void ImpactTarget::impact()
{
	if (!_target->isAlive())
	{
		_owner_skill->setDead();
		return;
	}

	if (_impacted)
		return;

	_impacted = true;


	int damage = _owner_skill->getDamage() + _shooter->getMDamage();

	if (_target != nullptr)
	{
		//send a message to the bot to let it know it's been hit, and who the
		//shot came from.
		Dispatcher.dispatchMsg(
			SEND_MSG_IMMEDIATELY,
			//_owner_skill->getId(),
			_owner_skill->getShooterId(),
			_target->getId(),
			MSG_GET_DAMAGED,
			(void*)&damage);
	}
}
