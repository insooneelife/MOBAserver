#include "ActivateDirect.h"

ActivateDirect::ActivateDirect(AbstSkill* const owner_skill)
	:
	ActivateStrategy(owner_skill)
{}

ActivateDirect::~ActivateDirect()
{}

bool ActivateDirect::test()
{
	return true;
}