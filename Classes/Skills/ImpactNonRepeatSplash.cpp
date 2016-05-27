#include "ImpactNonRepeatSplash.h"
#include "../CollisionTestTemplates.h"
#include "../AbstCharacter.h"
#include "../Message/MessageDispatcher.h"

ImpactNonRepeatSplash::ImpactNonRepeatSplash(
	AbstSkill* const owner_skill,
	AbstCharacter* shooter,
	float splash_radius)
	:
	ImpactStrategy(owner_skill),
	_splash_radius(splash_radius),
	_shooter(shooter)
{}


//----------------------------- impact ------------------------------------
//
//  This iterate through agent(character) cellspaces and give messages 
//  to them with damage info if they overlap.
//-------------------------------------------------------------------------

void ImpactNonRepeatSplash::impact()
{
	auto hit_list = getListOfCollisionEntitiesCellSpace(
		_owner_skill, _owner_skill->getWorld()->getGameMap()->getAgentCellSpace().get());

	if (hit_list.size() > 0)
	{
		for (auto hit : hit_list)
		{
			auto iter = _already_hit_agent.find(hit->getId());
			if (iter == std::end(_already_hit_agent))
			{
				int damage = _owner_skill->getDamage() + _shooter->getMDamage();

				//send a message to the bot to let it know it's been hit, and who the
				//shot came from.
				Dispatcher.dispatchMsg(
					SEND_MSG_IMMEDIATELY,
					_owner_skill->getId(),
					hit->getId(),
					MSG_GET_DAMAGED,
					(void*)&damage);

				_already_hit_agent.insert(hit->getId());
			}
		}
	}
}
