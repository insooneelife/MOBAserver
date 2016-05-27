#ifndef ACTIVATE_STRATEGY_H
#define ACTIVATE_STRATEGY_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ActivateStrategy.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Base class to define skill's activate strategies
//
//-----------------------------------------------------------------------------

#include "AbstSkill.h"

class ActivateStrategy
{
public:

	ActivateStrategy(AbstSkill* const owner_skill)
		:
		_owner_skill(owner_skill)
	{}

	virtual ~ActivateStrategy() 
	{}

	//must be implemented
	virtual bool test() = 0;

protected:
	
	//a pointer to the skill that owns this activate strategy
	AbstSkill* const		_owner_skill;
};


#endif