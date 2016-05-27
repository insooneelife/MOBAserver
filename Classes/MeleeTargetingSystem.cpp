#include "MeleeTargetingSystem.h"
#include "AbstCharacter.h"
#include "SensoryMemory.h"



//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
MeleeTargetingSystem::MeleeTargetingSystem(AbstCharacter* const owner)
	:
	AbstTargetingSystem(owner)
{}


//----------------------------- update ----------------------------------------
//-----------------------------------------------------------------------------
void MeleeTargetingSystem::update()
{
	double closest_dist_so_far = std::numeric_limits<double>::max();
	_current_target = 0;

	//grab a list of all the opponents the owner can sense
	std::list<AbstCharacter*> sensed_bots;
	sensed_bots = _owner->getSensoryMemory()->getListOfRecentlySensedOpponents();

	std::list<AbstCharacter*>::const_iterator bot = sensed_bots.begin();
	for (bot; bot != sensed_bots.end(); ++bot)
	{
		//make sure the bot is alive and that it is not the owner
		if ((*bot)->isAlive() && (*bot != _owner))
		{
			if (_owner->getTeam() != (*bot)->getTeam())
			{
				double dist = ((*bot)->getPos() - _owner->getPos()).getLengthSq();

				if (dist < closest_dist_so_far)
				{
					closest_dist_so_far = dist;
					_current_target = *bot;
				}
			}
		}
	}
}
