#ifndef MELEE_TARGETING_SYSTEM_H
#define MELEE_TARGETING_SYSTEM_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   MeleeTargetingSystem.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to select a target from the opponents currently in a bot's
//          perceptive memory.
//-----------------------------------------------------------------------------
#include "cocos2d.h"
#include <list>
#include "AbstTargetingSystem.h"


class AbstCharacter;

class MeleeTargetingSystem : public AbstTargetingSystem
{

public:

	explicit MeleeTargetingSystem(AbstCharacter* const owner);

	virtual ~MeleeTargetingSystem() override
	{}

	//each time this method is called the opponents in the owner's sensory 
	//memory are examined and the closest  is assigned to _current_target.
	//if there are no opponents that have had their memory records updated
	//within the memory span of the owner then the current target is set
	//to null
	virtual void       update() override;

};




#endif
