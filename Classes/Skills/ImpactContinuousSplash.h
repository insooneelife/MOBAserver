#ifndef IMPACT_CONTINUOUS_SPLASH_H
#define IMPACT_CONTINUOUS_SPLASH_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ImpactContinuousSplash.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Splash impacting strategy for owner skill.
//
//-----------------------------------------------------------------------------

#include "ImpactStrategy.h"

class ImpactContinuousSplash : public ImpactStrategy
{
public:

	ImpactContinuousSplash::ImpactContinuousSplash(
		AbstSkill* const owner_skill,
		AbstCharacter* shooter,
		float splash_radius);

	virtual ~ImpactContinuousSplash()
	{}

	virtual void impact() override;

public:

	AbstCharacter* _shooter;
	//the radius of damage, once the rocket has impacted
	float	_splash_radius;

	//this strategy impacts only once
	bool	_impacted;
};


#endif