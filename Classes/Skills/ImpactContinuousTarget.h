#ifndef IMPACT_CONTINUOUS_TARGET_H
#define IMPACT_CONTINUOUS_TARGET_H
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

class ImpactContinuousTarget : public ImpactStrategy
{
public:

	ImpactContinuousTarget::ImpactContinuousTarget(
		AbstSkill* const owner_skill,
		AbstCharacter* shooter,
		AbstCharacter* target,
		float splash_radius);

	virtual ~ImpactContinuousTarget()
	{}

	virtual void impact() override;

public:

	AbstCharacter* _shooter;
	AbstCharacter* _target;

	//the radius of damage, once the rocket has impacted
	float	_splash_radius;

	//this strategy impacts only once
	bool	_impacted;
};


#endif