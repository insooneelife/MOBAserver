#ifndef ACTIVATE_COLLISION_TO_ANY_H
#define ACTIVATE_COLLISION_TO_ANY_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ActivateCollisionToAny.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Activation strategy for owner skill.
//			Detect if there's collision to any other characters.
//
//-----------------------------------------------------------------------------

#include "ActivateStrategy.h"
#include "cocos2d.h"

class ActivateCollisionToAny : public ActivateStrategy
{
public:

	ActivateCollisionToAny::ActivateCollisionToAny(AbstSkill* const owner_skill);

	virtual ~ActivateCollisionToAny()
	{}

	virtual bool test() override;
};

#endif