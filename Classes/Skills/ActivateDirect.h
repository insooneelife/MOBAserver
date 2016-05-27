#ifndef ACTIVATE_DIRECT_H
#define ACTIVATE_DIRECT_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ActivateDirect.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Direct activation strategy for owner skill.
//
//-----------------------------------------------------------------------------

#include "ActivateStrategy.h"
#include "cocos2d.h"

class ActivateDirect : public ActivateStrategy
{
public:

	ActivateDirect::ActivateDirect(AbstSkill* const owner_skill);

	virtual ~ActivateDirect();

	virtual bool test() override;
};

#endif