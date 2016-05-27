#ifndef IMPACT_TARGET_H
#define IMPACT_TARGET_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ImpactSplash.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Splash impacting strategy for owner skill.
//
//-----------------------------------------------------------------------------

#include "ImpactStrategy.h"

class ImpactTarget : public ImpactStrategy
{
public:

	ImpactTarget::ImpactTarget(
		AbstSkill* const owner_skill,
		float splash_radius,
		AbstCharacter* shooter,
		AbstCharacter* target);

	virtual ~ImpactTarget()
	{}

	virtual void impact() override;

public:

	//the radius of damage, once the rocket has impacted
	// ¾È¾µµí
	float	_splash_radius;

	AbstCharacter* _shooter;
	AbstCharacter* _target;
	//this strategy impacts only once
	bool	_impacted;
};


#endif