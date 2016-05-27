#include "Feature.h"
#include "../AbstCharacter.h"



//------------------------------- HealthScore ---------------------------------
//
//-----------------------------------------------------------------------------
double Feature::healthProp(AbstCharacter* const bot)
{
	return (double)bot->getHealth() / (double)bot->getMaxHealth();
}