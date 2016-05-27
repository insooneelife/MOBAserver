#ifndef ACTIVATE_CONTINUOUS_H
#define ACTIVATE_CONTINUOUS_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   ActivateContinuous.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   
//
//-----------------------------------------------------------------------------

#include "ActivateStrategy.h"
#include "ImpactSplash.h"
#include "cocos2d.h"

class ActivateContinuous : public ActivateStrategy
{
public:

	ActivateContinuous::ActivateContinuous(
		AbstSkill* const owner_skill, 
		AbstCharacter* const shooter, 
		double peroid, 
		double survive_time);

	virtual ~ActivateContinuous()
	{}

	virtual bool test() override;

	double getPeriod();
	void setPeriod(double period);

	double getSurviveTime();
	void setSurviveTime(double time);


public:
	AbstCharacter* _shooter;
	double _period;
	double _survive_time;

};

#endif