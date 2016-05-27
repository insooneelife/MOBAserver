#ifndef IMPACT_NON_REPEAT_SPLASH_H
#define IMPACT_NON_REPEAT_SPLASH_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ImpactNonRepeatSplash.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Splash impacting strategy for owner skill.
//			But this strategy doesn't impact more than one for entities
//			that already impacted.
//
//-----------------------------------------------------------------------------

#include "ImpactStrategy.h"

class ImpactNonRepeatSplash : public ImpactStrategy
{
public:

	ImpactNonRepeatSplash::ImpactNonRepeatSplash(
		AbstSkill* const owner_skill,
		AbstCharacter* shooter,
		float splash_radius);

	virtual ~ImpactNonRepeatSplash()
	{}

	virtual void impact() override;

public:

	AbstCharacter* _shooter;
	//the radius of damage, once the rocket has impacted
	float	_splash_radius;

	//remembers entities that have alread impacted.
	std::set<int> _already_hit_agent;

};


#endif