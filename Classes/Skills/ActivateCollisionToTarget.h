#ifndef ACTIVATE_COLLISION_TO_TARGET_H
#define ACTIVATE_COLLISION_TO_TARGET_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ActivateCollisionToTarget.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Activation strategy for owner skill.
//			Detect if there's collision to targeted character.
//
//-----------------------------------------------------------------------------

#include "cocos2d.h"
#include "ActivateStrategy.h"
#include "AbstTargetSkill.h"


class ActivateCollisionToTarget : public ActivateStrategy
{
public:

	ActivateCollisionToTarget::ActivateCollisionToTarget(AbstSkill* const owner_skill);

	virtual ~ActivateCollisionToTarget()
	{}

	virtual bool test() override;

private:

	AbstTargetSkill* const _down_casted_skill;
};

#endif