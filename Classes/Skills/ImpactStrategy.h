#ifndef IMPACT_STRATEGY_H
#define IMPACT_STRATEGY_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ImpactStrategy.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Base class to define skill's impact strategies.
//
//-----------------------------------------------------------------------------

#include "AbstSkill.h"

class ImpactStrategy
{
public:

	ImpactStrategy(AbstSkill* const owner_skill)
		:
		_owner_skill(owner_skill)
	{}

	virtual ~ImpactStrategy()
	{}

	//must be implemented
	virtual void impact() = 0;

protected:

	//a pointer to the skill that owns this activate strategy
	AbstSkill* const		_owner_skill;
};

#endif